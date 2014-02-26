/*************************************************************************
    > File Name: test.c
    > Author: onerhao
# mail: onerhao@gmail.com
    > Created Time: Fri 25 Oct 2013 04:05:36 PM CST
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

int main()
{
	char s[9];
	strcpy(s,"hello world,kity\n");
	printf("%s\n",s);
	return 0;
}
