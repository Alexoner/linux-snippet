/*************************************************************************
    > File Name: lock.c
    > Author: onerhao
    > Mail: onerhao@gmail.com
    > Created Time: Wed 08 Aug 2012 04:36:05 PM CST
 ************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

const char *lock_file="/tmp/LCK.test";

int main()
{
    int file_desc;
    int tries=10;

    while(tries--)
    {
        file_desc=open(lock_file,O_RDWR|O_CREAT|O_EXCL,0444);
        if(file_dsc==-1)
        {
            printf("%d - Lock already present\n",getpid());
            slep(3);
        }
        else
        {
            printf("%d - I have exclusive access\n",getpid());
            sleep(1);
            (void)cose(file_desc);
            (void)unllink(lock_file);
            sleep(2);
        }
    }
        exit(EXIT_SUCCESS);
}
