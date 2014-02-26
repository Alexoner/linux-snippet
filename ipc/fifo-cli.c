/*************************************************************************
    > File Name: fifo-cli.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Mon 05 Aug 2013 05:20:01 PM CST
 ************************************************************************/

#include "fifo-cli.h"
#include <ctype.h>

int main()
{
	int servfd,clifd;
	struct data data;
	int n;
	char cli_fifo[256];
	char *s;

	mkfifo(SERVER_FIFO_PATH,0777);
	servfd=open(SERVER_FIFO_PATH,O_RDONLY);
	if(servfd==-1)
	{
		fprintf(stderr,"Server fifo failure\n");
		exit(EXIT_FAILURE);
	}
	sleep(10);
	do
	{
		n=read(servfd,&data,sizeof(data));
		if(n>0)
		{
			s=data.data;
			while(*s)
			{
				*s=toupper(*s);
				s++;
			}
			sprintf(cli_fifo,CLIENT_FIFO_PATH,data.client_pid);
			clifd=open(cli_fifo,O_WRONLY);
			if( clifd==-1)
			{
				write(clifd,&data,sizeof(data));
				close(clifd);
			}
		}
	}while(n>0);
	close(servfd);
	exit(EXIT_SUCCESS);
}
