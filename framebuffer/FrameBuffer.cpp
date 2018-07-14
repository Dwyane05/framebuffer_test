/*
 * FrameBuffer.cpp
 *
 *  Created on: Dec 4, 2017
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


static int FbMmap(struct FrameBuffer *fb)
{
	int screensize ;	/*计算屏幕缓冲区大小*/

	// if(fb->fd <= 0) {
	// 	printf("Fb Device was not opened\n");
	// 	return -EINVAL;
	// }

	if(fb->fb_mem != NULL) {
		printf("Fb Device was not mmaped\n");
		return -EINVAL;
	}

	screensize = fb->width *  fb->height * fb->depth / 8;

	printf("Cal frame size is:%d\n", screensize);

	fb->fb_mem = mmap(0, screensize, PROT_READ | PROT_WRITE,
			MAP_SHARED, fb->fd, 0);
	if(MAP_FAILED == fb->fb_mem) {
		printf("FB mmap framebuffer fail.\n%d, %s\n",
				errno, strerror(errno));
		return -1;
	}

	return 0;
}

static int FbMunmap(struct FrameBuffer *fb)
{
	int screensize = fb->width *  fb->height * fb->depth / 8;

	if(fb->fb_mem == NULL){
		printf("Invalid parameters\n");
		return -1;
	}

	if(0 !=  munmap(fb->fb_mem, screensize))  {
	   printf("munmap was fault.\n");
	   return -1;
	}

	fb->fb_mem = NULL;
	return 0;
}

int FbOpen(struct FrameBuffer *fb, const char *pathname)
{
	int ret;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;

	if(fb->fd > 0 || !pathname) {
		printf("Invalid parameters\n");
		return -EINVAL;
	}

	fb->fd  = open(pathname, O_RDWR);
	if(fb->fd  < 0) {
		printf("open %s fail, %s (%d)\n", pathname, strerror(errno), errno);
		return -1;
	}

	printf("open %s OK, fd = %d\n", pathname, fb->fd);

	if(ioctl(fb->fd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Reading fixed info.\n");
	    goto error_exit;
	}
	
	fb->fb_line_len = finfo.line_length;
	fb->fb_size = finfo.smem_len;

	printf("Get frame fb_line_len is :%d\n", fb->fb_line_len);
	printf("Get frame size is :%d\n",fb->fb_size);
	printf("Get smem_start is:%lu\n", finfo.smem_start); 
	printf("Get mmio_len is:%u\n", finfo.mmio_len); 
	printf("Get mmio_start is:%lu\n", finfo.mmio_start); 
	printf("Get type is:%u\n", finfo.type); 

	switch(finfo.type) {
	case FB_TYPE_PACKED_PIXELS:
		printf("fb Device type is packed\n"); 
		break;
	case FB_TYPE_PLANES:
		printf("fb Device type is planes\n"); 
		break;
	case FB_TYPE_INTERLEAVED_PLANES:
	case FB_TYPE_TEXT:
	case FB_TYPE_VGA_PLANES:
	case FB_TYPE_FOURCC:
	default:
		printf("Device fb type is unknow\n"); 
		break;
	}

	if(ioctl(fb->fd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading var info.\n");
	    goto error_exit;
	}

	fb->width 			= vinfo.xres;
	fb->height 			= vinfo.yres;
	fb->depth 			= vinfo.bits_per_pixel;
	fb->red_offset 		= vinfo.red.offset;
	fb->red_length 		= vinfo.red.length;
	fb->green_offset 	= vinfo.green.offset;
	fb->green_length 	= vinfo.green.length;
	fb->blue_offset 	= vinfo.blue.offset;
	fb->blue_length 	= vinfo.blue.length;
	fb->transp_offset 	= vinfo.transp.offset;
	fb->transp_length 	= vinfo.transp.length;

	printf("Get frame width is :%d\n", fb->width);
	printf("Get frame height is :%d\n", fb->height);
	printf("Get frame depth is :%d\n", fb->depth);

	printf("red offset:%d, len %d\n", vinfo.red.offset, vinfo.red.length);
	printf("red offset:%d, len %d\n", fb->red_offset, fb->red_length );

	printf("green offset:%d, len %d\n", vinfo.green.offset, vinfo.green.length);
	printf("green offset:%d, len %d\n", fb->green_offset, fb->green_length);
	
	printf("blue offset:%d, len %d\n", vinfo.blue.offset, vinfo.blue.length);
	printf("blue offset:%d, len %d\n",fb-> blue_offset, fb->blue_length );
	
	printf("transp offset:%d, len %d\n", vinfo.transp.offset, vinfo.transp.length);
	printf("transp offset:%d, len %d\n", fb->transp_offset,fb->transp_length);

	ret = FbMmap(fb);
	if(ret < 0) {
		printf("MMap fail.\n");
		goto error_exit;
	}

	return 0;

error_exit:
	close(fb->fd);
	fb->fd = -1;
	return -1;
}

void FbClose(struct FrameBuffer *fb)
{
	if(fb->fd > 0) {
		FbMunmap(fb);

		close(fb->fd);
		fb->fd = -1;
	}
}

void FbShow(struct FrameBuffer *fb, unsigned char *data)
{
	int screensize;
	unsigned char *p = (unsigned char *) fb->fb_mem;

	if(fb->fd <= 0) {
		return;
	}

	screensize  = fb->width *  fb->height * fb->depth / 8;

	memcpy(p, data, screensize);
}

void FbShow_RGBpoint(struct FrameBuffer *fb, struct RGBPoint *data)
{
	int screensize;
	unsigned char *p = (unsigned char *) fb->fb_mem;

	if(fb->fd <= 0) {
		return;
	}

}
