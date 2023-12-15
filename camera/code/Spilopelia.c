/*
 * Spilopelia.c
 *
 *  Created on: 2023年12月14日
 *      Author: Misac
 */
#include "zf_common_headfile.h"

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
	unsigned char k;
    for(int i = 0; i < GrayImg_ValMax; i++)
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
			k = i;
		}  
	}
	return k;
}

void Image_Nor(unsigned char image_nor[MT9V03X_H][MT9V03X_W],unsigned char k)
{
	for (int i = 0; i < MT9V03X_H; i++) 
	{
        for (int j = 0; j < MT9V03X_W; j++) 
		{
            image_nor[i][j] = (mt9v03x_image[i][j] > k )? 255:0;
        }
    }
}