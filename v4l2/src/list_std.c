/*************************************************************************
    > File Name: list_std.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Sun 06 Jan 2013 11:43:15 PM CST
 ************************************************************************/

#include <linux/videodev2.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int main(int argc,char **argv)
{
    struct v4l2_input input;
    struct v4l2_standard standard;
    int fd=open("/dev/video0",O_RDWR);

    memset(&input,0,sizeof(input));
    if(-1==ioctl(fd,VIDIOC_G_INPUT,&input.index))
    {
        perror("VIDIOC_G_INPUT");
        exit(EXIT_FAILURE);
    }

    if(-1==ioctl(fd,VIDIOC_ENUMINPUT,&input))
    {
        perror("VIDIOC_ENUM_INPUT");
        exit(EXIT_FAILURE);
    }

    printf("Current input %s supports:\n",input.name);

    memset(&standard,0,sizeof(standard));
    standard.index=0;

    while(0==ioctl(fd,VIDIOC_ENUMSTD,&standard))
    {
        if(standard.id& input.std)
            printf("%s \n",standard.name);

        standard.index++;
    }

    /*EINVAL indicates the end of the enumeration,which cannot be empty
     * unless this device falls under the USB execption*/

    if(errno!=EINVAL||standard.index==0)
    {
        perror("VIDIOC_ENUMSTD");
        exit(EXIT_FAILURE);
    }
    return 0;
}
