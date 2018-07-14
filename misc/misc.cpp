/*
 * misc.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: xiao
 *  Changed on: Aug 10,2017
 *			by: huang
 */
#include <stdio.h>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
//#include <sys/reboot.h>
#include "misc.h"

/*
 * for message print
 */


using namespace std;

void msleep(int ms)
{
    struct timeval delay = {
		.tv_sec = 0,
		.tv_usec = ms * 1000
    };

    select(0, NULL, NULL, NULL, &delay);
}

/*
 * echo mem >  /sys/power/state
 */
void gotoSleep()
{
	const char *standby = "mem";

	int fd = open("/sys/power/state", O_WRONLY, 0);
	if (fd == -1) {
		printf("Could not open \n");
		return ;
	}

	write(fd, standby, strlen(standby));

	close(fd);
}

/*
 * echo recovery > /proc/jz/reset
 */
int Recovery()
{
	const char *recovery = "recovery";

	int fd = open("/proc/jz/reset/reset", O_WRONLY, 0);
	if (fd == -1) {
		printf("Could not open \n");
		return -1;
	}

	write(fd, recovery, strlen(recovery));

	close(fd);

	return 0;
}

