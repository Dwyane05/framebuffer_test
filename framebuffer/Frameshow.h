/*
 * FrameBuffer.h
 *
 *  Created on: Jan 17, 2018
 *      Author: xiao
 */
#ifndef __FRAMESHOW_H_
#define __FRAMESHOW_H_

int FbInit();

void FbExit();

void Fb_showred();

void Fb_showgreen();

void Fb_showblue();

void Fb_showthreecolors(void);

void Fb_showYUV(void *img, unsigned int width, unsigned int height);

void Fb_showDirect(void *img, unsigned int width, unsigned int height);


#endif