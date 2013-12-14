#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
    int fd;
    if(argc<2)
    {
        printf("this file is used to show the content of a file into upper\n");
        exit(1);
    }
    else
    {
        while(--argc)
        {
            if((fd=open(*++argv,O_RDONLY))<0)
                        {
                            perror("open");
                            exit(1);
                        }
            dup2(fd,STDIN_FILENO);//duplicate a file descriper
            close(fd);
            execl("./upper","upper",NULL);
            perror("exec ./upper");
        }
    }
    return 0;
}


