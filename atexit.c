/*************************************************************************
    > File Name: atexit.c
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Sat 05 Jan 2013 06:52:26 PM CST
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>


void foo()
{
    int uid;
    uid=getuid();
    uid=setuid(0);
    printf("hello world,%d\n",uid);
}

int main()
{
    atexit(foo);
    return 0;
}

