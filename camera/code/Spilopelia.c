/*
 * Spilopelia.c
 *
 *  Created on: 2023年12月14日
 *      Author: Misac
 */
#include "zf_common_headfile.h"

//寻找阈值
unsigned char Ostu_find()
{
    int list_val[GrayImg_ValMax] = {0};
    float list_nor[GrayImg_ValMax] = {0};

	//统计直方图
    for (int i = 0; i < MT9V03X_H; i++)
    {
        for (int j = 0; j < MT9V03X_W; j++) 
        {
            list_val[mt9v03x_image[i][j]]++;
        }
    }
	//比例化
	int sum = MT9V03X_H * MT9V03X_W;
    for (int i = 0; i < GrayImg_ValMax; i++) 
    {
        list_nor[i] = 1.0 * list_val[i] / sum;
    }

	//寻找阈值k
	float p0=0,p0_a=0,p1=0,p1_a=0,o=0,o_max=0;
	unsigned char threshold;
    for(int i = 0; i < 220; i++)
	{
		for(int j = 0; j < GrayImg_ValMax; j++)
		{
			if(j <= i)	//类1
			{
				p0 += list_nor[j];
				p0_a += j * list_nor[j]; 
			}
			else		//类2
			{
				p1 += list_nor[j];
				p1_a += j * list_nor[j];
			}
		}
		p0 = p0_a / p0;
		p1 = p1_a / p1;
		o = p0 * p1 * (p0_a - p1_a) * (p0_a - p1_a);
		if(o > o_max) 
		{
			o_max = o;
			threshold = i;
		}  
	}

	return threshold;
}

void Image_Nor(unsigned char image_nor[ImageNor_H][ImageNor_W],unsigned char threshold)
{
	for (int i = 0; i < ImageNor_H; i++)
	{
        for (int j = 0; j < ImageNor_W; j++)
		{
            image_nor[i][j] = (mt9v03x_image[i][j<<1] > threshold )? 255:0;
            // if(j==0 || j==93)
            //     image_nor[i][j] = 0;
        }
    }
}

//返回最大白列的首末项均值
unsigned char Image_handle(unsigned char image_nor[ImageNor_H][ImageNor_W],unsigned char threshold)
{
	unsigned char*	image_nor_p = (unsigned char*)image_nor;
	unsigned char*	mt9v03x_image_p = (unsigned char*)mt9v03x_image;
	unsigned char 	column_white[ImageNor_W] = {0};

	unsigned char   column_white_head = 0, column_white_end = 0;
	unsigned short  i,j;

	//处理图像
	for(i=0; i<ImageNor_S; i++)
	{
		//二值化，并压缩图像至0.5倍，mt9v03x_image应当是188*60
		*(image_nor_p + i) = ( *(mt9v03x_image_p + (i<<1) ) > threshold)? 255:0;

		//统计每列低于阈值的像素个数
		j = i % ImageNor_W;
		if( (*(image_nor_p + i)) )
		{
			column_white[j]++;
		}
	}

	//计算首末项均值并返回
	for(i=0; i<ImageNor_W; i++)
	{
		if(column_white[i] > ImageNor_H_approach)
		{
			if(!column_white_head){column_white_head = i;}
			column_white_end = i;
		}
	}
	return (column_white_head + column_white_end) >> 1;
}
