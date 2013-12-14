/*************************************************************************
    > File Name: cdromeject.c
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Sat 05 Jan 2013 12:00:23 AM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/cdrom.h>
#include <stdio.h>

int main(int argc,char **argv)
{
    int fd,ret;

    if(argc<2)
    {
        fprintf(stderr,
                "usage:%s <device to eject>\n",
                argv[0]);
        return 1;
    }

    fd=open(argv[1],O_RDONLY|O_NONBLOCK);
    if(fd<0)
    {
        perror("open");
        return 0;
    }

    ret=ioctl(fd,CDROMEJECT,0);
    //ret=ioctl(fd,CDROMSTART,0);
    if(ret)
    {
        perror("ioctl");
        return 1;
    }
    ret=close(fd);
    if(ret)
    {
        perror("close");
        return 1;
    }
    return 0;
}

