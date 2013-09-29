/*************************************************************************
    > File Name: test.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Tue 08 Jan 2013 11:56:24 PM CST
 ************************************************************************/

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <linux/videodev2.h>

int main()
{
struct v4l2_requestbuffers reqbuf;
int fd=open("/dev/video0",O_RDWR);

memset(&reqbuf, 0, sizeof (reqbuf));
reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
reqbuf.memory = V4L2_MEMORY_DMABUF;
reqbuf.count = 1;

if (ioctl(fd, VIDIOC_REQBUFS, &reqbuf) == -1) {
	if (errno == EINVAL)
		printf("Video capturing or DMABUF streaming is not supported\n");
	else
		perror("VIDIOC_REQBUFS");

	exit(EXIT_FAILURE);
}
}
