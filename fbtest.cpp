#include "stdio.h"
#include <unistd.h>
#include "FrameBuffer.h"
#include "Frameshow.h"
#include "misc.h"

#define DBG_EN 	1
#if (DBG_EN == 1)
#define printf_dbg(x,arg...) printf("[main_debug]"x,##arg)
#else
#define printf_dbg(x,arg...)
#endif

#define printf_info(x,arg...) printf("[main_info] "x,##arg)
#define printf_warn(x,arg...) printf("[main_warnning] "x,##arg)
#define printf_err(x,arg...) printf("[main_error] "x,##arg)


int main()
{
	int ret = -1;

	ret = FbInit();
	if(ret < 0) {
		return -1;
	}

	do {
		Fb_showred();
		msleep(500);

		Fb_showgreen();
		msleep(500);

		Fb_showblue();
		msleep(500);

		Fb_showthreecolors();
		msleep(500);
	} while(1);

	FbExit();
	return 0;
}
