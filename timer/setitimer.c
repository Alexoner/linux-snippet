/*************************************************************************
    > File Name: setitimer.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Sat 07 Sep 2013 09:01:02 PM CST
 ************************************************************************/

/*setitimer() provides up to 3 timer per process in linux
 */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>
int count = 0;
void set_timer()
{
        struct itimerval itv, oldtv;
        itv.it_interval.tv_sec = 5;
        itv.it_interval.tv_usec = 0;
        itv.it_value.tv_sec = 5;
        itv.it_value.tv_usec = 0;

        setitimer(ITIMER_REAL, &itv, &oldtv);
}

void sigalrm_handler(int sig)
{
        count++;
        printf("timer signal.. %d\n", count);
}

int main()
{
        signal(SIGALRM, sigalrm_handler);
        set_timer();
        while (count < 1000)
        {
			char c=getchar();
			if(c=='0')
			{
				printf("0 is entered,now stopping\n");
				//exit(1);
				break;
			}
		}
        exit(0);
}

