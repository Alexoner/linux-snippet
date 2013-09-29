/*************************************************************************
    > File Name: v4l-main.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Tue 08 Jan 2013 04:49:52 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>
#include "v4l.h"

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#ifndef V4L2_PIX_FMT_H264
#define V4L2_PIX_FMT_H264     v4l2_fourcc('H', '2', '6', '4') /* H264 with start codes */
#endif

int main(int argc,char **argv)
{
	int fd;
    int i=4;
    struct head_buffer *hbuffer;
 	if(argc>1)
		fd=open(argv[1],O_RDWR);
	else
		fd=open("/dev/video0",O_RDWR);

	hbuffer=(struct head_buffer*)malloc(sizeof(struct head_buffer));
    set_fmt(fd);
    hbuffer=init_mmap(fd,hbuffer);
    start_capturing(fd,IO_METHOD_MMAP,hbuffer);
    while(i--)
    {
        read_frame(fd,IO_METHOD_MMAP,hbuffer);
        sleep(1);
    }
    return 0;
}


