/*************************************************************************
    > File Name: info_input.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Sun 06 Jan 2013 08:24:12 PM CST
 ************************************************************************/
#include <linux/videodev2.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CLEAR(x) memset(&(x),0,sizeof(x))

int main(int argc,char **argv)
{
    struct v4l2_input input;
    struct v4l2_tuner tuner;
    int index;
    int fd=open("/dev/video0",O_RDWR);

    if(-1==ioctl(fd,VIDIOC_G_INPUT,&index))
    {
        perror("VIDIOC_G_INPUT");
        exit(EXIT_FAILURE);
    }

    memset(&input,0,sizeof(input));
    input.index=index;

    if(-1==ioctl(fd,VIDIOC_ENUMINPUT,&input))
    {
        perror("VIDIOC_ENUMINPUT");
        exit(EXIT_FAILURE);
    }
    printf("Current input: %s\n",input.name);
    if(-1==ioctl(fd,VIDIOC_S_TUNER,&index))
    {
        perror("VIDIOC_S_TUNER");
        exit(EXIT_FAILURE);
    }
    return 0;
}
