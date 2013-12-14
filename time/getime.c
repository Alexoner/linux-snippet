/*************************************************************************
    > File Name: getime.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Sat 31 Aug 2013 11:52:24 PM CST
 ************************************************************************/

#include <stdio.h>
#include <time.h>

int main()
{
	time_t time_s;
	struct tm *time_tm;
	char *time_char;
	time_s=time(NULL);
	time_tm=localtime(&time_s);
	time_char=ctime(&time_s);
	printf("%ld,%s\n%s\n",time_s,time_char,asctime(time_tm));
	return 0;
}
