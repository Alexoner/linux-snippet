/*************************************************************************
    > File Name: printtid.c
    > Author: onerhao
	# mail: onerhao@gmail.com
    > Created Time: Sat 26 Oct 2013 04:30:32 PM CST
 ************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

pthread_t ntid;

void printtids(const char *s)
{
	pid_t pid;
	pthread_t tid;

	pid=getpid();
	tid=pthread_self();
	printf("%s pid %lu tid %lu (0x%lx)\n",s,(unsigned long)pid,
			(unsigned long)tid,(unsigned long)tid);
}

void *thr_fn(void *arg)
{
	printtids("new thread: ");
	return ((void*)0);
}

int main(void)
{
	int err;
	err=pthread_create(&ntid,NULL,thr_fn,NULL);
	if(err!=0)
	{
		fprintf(stderr,"can't create thread\n");
		exit(-1);
	}
	printtids("main thread: ");
	sleep(1);
	exit(0);
}

