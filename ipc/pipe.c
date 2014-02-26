#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA_LEN 256
#define DELAY_TIME 1

int main()
{
    pid_t pid;
    int pipe_fd[2];//pipe file descriper;
    char buf[MAX_DATA_LEN];
    const char data[]="Pipe Test Progeam";
    int real_read,real_write;
    memset((void *)buf,0,sizeof(buf));/*fill the first sizeof(buf) bytes of  memory buf with 0*/
    if(pipe(pipe_fd)<0)//creat pipe
    {
        printf("pipie create error\n");
        exit(1);
    }

    if((pid=fork())==0)//creat child process
    {
        //close(pipe_fd[1]);
        sleep(DELAY_TIME*3);
        /*read up to MAX_DATA_LEN bytes into buffer starting at buf*/
        if((real_read=read(pipe_fd[0],buf,MAX_DATA_LEN))>0)
        {
            printf("%d bytes read from the pipe is '%s'\n",real_read,buf);
        }//read from a file descriper into buf
        close(pipe_fd[0]);
        exit(0);
    }

    else if (pid>0)//parent process
    {
        //close(pipe_fd[0]);//close the read descriper
        sleep(DELAY_TIME);
        if((real_write=write(pipe_fd[1],data,strlen(data)))!=-1)
        {//write from data
            printf("Parent wrote %d bytes :'%s'\n",real_write,data);
        }
        close(pipe_fd[1]);
        waitpid(pid,NULL,0);
        exit (0);
    }
    return 0;
}


