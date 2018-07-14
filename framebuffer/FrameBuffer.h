/*
 * FrameBuffer.h
 *
 *  Created on: Dec 4, 2017
 *      Author: xiao
 */

#ifndef __FRAMEBUFFER_H_
#define __FRAMEBUFFER_H_

struct FrameBuffer {
	int fd;

	void *fb_mem;

	unsigned int width;
	unsigned int height;
	unsigned int depth;

	unsigned int fb_line_len;
	unsigned int fb_size;
	unsigned int red_offset;
	unsigned int red_length;

	unsigned int green_offset;
	unsigned int green_length;

	unsigned int blue_offset;
	unsigned int blue_length;

	unsigned int transp_offset;
	unsigned int transp_length;
};


struct RGBPoint {
	union {
		struct rgb32 {
			unsigned char red;
			unsigned char green;
			unsigned char blue;
			unsigned char transparency;
		} rgb32;

		struct rgb16 {
			unsigned char red;
			unsigned char green;
			unsigned char blue;
		} rgb16;
	} data;
};
int FbOpen(struct FrameBuffer *fb, const char *pathname);

void FbClose(struct FrameBuffer *fb);

void FbShow(struct FrameBuffer *fb, unsigned char *data);

void FbShow_RGBpoint(struct FrameBuffer *fb, struct RGBPoint *data);


#endif /* FRAMEBUFFER_H_ */
