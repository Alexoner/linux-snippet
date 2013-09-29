/*************************************************************************
    > File Name: alarm.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Sat 07 Sep 2013 09:41:30 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void my_alarm_handler(int sig)
{
	printf("my_alarm_handler\n");
	alarm(2);
}

int main(int argc,char **argv)
{
	signal(SIGALRM,my_alarm_handler);
	alarm(1);
	/*while(1)
	{
		char c=getchar();
		if(c=='0')
		{
			printf("0 is entered,breaking while loop");
			break;
		}
		else
		{
			printf("Received char : %c\n",c);
		}
	}*/
	printf("while loop is breaked\n");
	return 0;
}

