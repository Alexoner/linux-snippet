/*************************************************************************
    > File Name: info_vstd.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Sun 06 Jan 2013 08:57:51 PM CST
 ************************************************************************/

#include <linux/videodev2.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int main(int argc,char **argv)
{
    v4l2_std_id std_id;
    struct v4l2_standard standard;
    int fd=open("/dev/video0",O_RDWR);

    if(-1==ioctl(fd,VIDIOC_G_STD,&std_id))
    {
        perror("VIDIOC_G_STD");
        exit(EXIT_FAILURE);
    }

    memset(&standard,0,sizeof(standard));
    standard.index=0;

    while(0==ioctl(fd,VIDIOC_ENUMSTD,&standard))
    {
        if(standard.id & std_id)
        {
            printf("Current video standard: %s\n",standard.name);
            exit(EXIT_SUCCESS);
        }

        standard.index++;
    }

    /*EINVAL indicates the end of the enumeration,which canot be emptey
     * unless this device falls under the USB extension.*/
    if(/*errno==EINVAL||*/standard.index==0)
    {
        perror("VIDIOC_ENUMSTD");
        exit(EXIT_FAILURE);
    }
    return 0;
}


