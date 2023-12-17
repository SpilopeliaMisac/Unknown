/*
 * Spilopelia.h
 *
 *  Created on: 2023Äê12ÔÂ14ÈÕ
 *      Author: Misac
 */

#ifndef _SPILOPELIA_H_
#define _SPILOPELIA_H_

#define GrayImg_ValMax 256

#define ImageNor_W  (94)
#define ImageNor_H  (60)
#define ImageNor_S  (5640) //94*60
#define ImageNor_H_approach (55)

unsigned char Ostu_find();
void Image_Nor(unsigned char image_nor[ImageNor_H][ImageNor_W],unsigned char k);
unsigned char Image_handle(unsigned char image_nor[ImageNor_H][ImageNor_W],unsigned char threshold);


#endif
