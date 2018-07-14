/*
 * Frameshow.cpp
 *
 *  Created on: Jan 17, 2018
 *      Author: xiao
 */
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "FrameBuffer.h"



struct FrameBuffer g_fb;
struct FrameBuffer *p_gfb = &g_fb;

struct PrvBuffer {
    void   *start;
    size_t  length;
};

struct PrvBuffer g_buf;
struct PrvBuffer *p_gbuf = &g_buf;

int FbInit()
{
	int ret = -1;

	ret = FbOpen(&g_fb, "/dev/fb0");
	if(ret < 0) {
		return -1;
	}

	p_gbuf->length = p_gfb->width * p_gfb->height * p_gfb->depth / 8;
	p_gbuf->start = malloc(p_gbuf->length);
	if(!p_gbuf->start) {
		printf("No memory for framebuffer\n");
		return -1;
	}

	return 0;
}

void FbExit()
{
	if(p_gbuf->start) {
		free(p_gbuf->start);
		p_gbuf->start = NULL;
	}

	FbClose(&g_fb);
}

void Fb_showcolors(unsigned char red,
		unsigned char green,
		unsigned char blue)
{
	if(p_gbuf->start == NULL) return ;

	memset(p_gbuf->start, 0, p_gbuf->length);

	if(p_gfb->depth == 32) {
		int *p = (int *) p_gbuf->start;

		int color = (red << p_gfb->red_offset) \
			| (green << p_gfb->green_offset) \
			| (blue << p_gfb->blue_offset);

		for(unsigned int i = 0; i < p_gfb->width; i++)
		{
			for(unsigned int j = 0; j < p_gfb->height ; j++)
			{
				*p = color;
				p++;
			}
		}
	} else if(p_gfb->depth == 16) {
		short *q = (short *)p_gbuf->start;

		unsigned short color = (unsigned short) (
			(((blue & 0xf8) >> 3) << p_gfb->blue_offset) \
			| (((green & 0xfc) >> 2) << p_gfb->green_offset ) \
			| ((red & 0xf8 >> 3) << p_gfb->red_offset));

		for(unsigned int i = 0; i < p_gfb->width; i++)
		{
			for(unsigned int j = 0; j < p_gfb->height ; j++)
			{
				*q = color;
				q++;
			}
		}
	} else {
		return;
	}

	FbShow(p_gfb, (unsigned char *) p_gbuf->start);
}

void Fb_showred()
{
	unsigned char red   = 0xff;
	unsigned char green = 0;
	unsigned char blue  = 0;

	printf("%s\n" , __func__);

	Fb_showcolors(red, green, blue);
}

void Fb_showgreen()
{
	unsigned char red   = 0;
	unsigned char green = 0xff;
	unsigned char blue  = 0;

	printf("%s\n" , __func__);

	Fb_showcolors(red, green, blue);
}

void Fb_showblue()
{
	unsigned char red   = 0;
	unsigned char green = 0;
	unsigned char blue  = 0xff;

	printf("%s\n" , __func__);

	Fb_showcolors(red, green, blue);
}

void Fb_showthreecolors(void)
{
	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;
	int *p = (int *) p_gbuf->start;
	short *q = (short *) p_gbuf->start;

	if(p_gbuf->start == NULL) return;

	memset(p_gbuf->start, 0, p_gbuf->length);

	for(unsigned int i = 0; i < p_gfb->height; i++)
	{
		if(i < (p_gfb->height / 3)) {
			red = 0xff;
			green = 0;
			blue = 0;
		} else if(i < (p_gfb->height / 3) * 2) {
			red = 0;
			green = 0xff;
			blue = 0;
		} else {
			red = 0;
			green = 0;
			blue = 0xff;
		}

		for(unsigned int j = 0; j < p_gfb->width ; j++)
		{

			if(p_gfb->depth == 32) {
				int color = (red << p_gfb->red_offset) \
					| (green << p_gfb->green_offset) \
					| (blue << p_gfb->blue_offset);

				*p = color;
				p++;
			} else if(p_gfb->depth == 16) {
				short *q = (short *)p_gbuf->start;

				unsigned short color = (unsigned short) (
					(((blue & 0xf8) >> 3) << p_gfb->blue_offset) \
					| (((green & 0xfc) >> 2) << p_gfb->green_offset ) \
					| ((red & 0xf8 >> 3) << p_gfb->red_offset));
				*q = color;
				q++;
			}
		}
	}

	FbShow(p_gfb, (unsigned char *) p_gbuf->start);
}

void Fb_showYUV(void *img, unsigned int width, unsigned int height)
{
	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;
	int *p = (int *) p_gbuf->start;
	int *q = (int *) p_gbuf->start;
	unsigned char *pImg =(unsigned char *) img;
	
	if(p_gbuf->start == NULL) return;

	if(!img || 0 == width || 0 == height) return;

    printf("%s ,width:%d ,height:%d\n", __func__, width, height);

	for(unsigned int j = 0; j <  p_gfb->width; j++)
	{
	  for(int i = p_gfb->height - 1; i >= 0; i--)
	  {
		red = 1.164 *(* pImg - 16) + 1.159 * (* (pImg  +3) - 128); 
		green = 1.164 *(*pImg - 16) - 0.380 * ( *(pImg + 1) -128) + 0.813 * (*(pImg + 3) - 128); 
		blue = 1.164 * (*pImg - 16) + 2.018 * (*(pImg + 1) - 128);  

		pImg += 4;

		if(p_gfb->depth == 32) {
			*(q + j + i * width) = (int) ( ((blue & 0xf8) << 0) | \
				((green & 0xfc) << 8)  | ((red & 0xf8) << 16));
		}
	  }

	  pImg += width * 2;
	}

	FbShow(p_gfb, (unsigned char *) p_gbuf->start);
}

void Fb_showDirect(void *img, unsigned int width, unsigned int height)
{
	FbShow(p_gfb, (unsigned char *) img);
}
