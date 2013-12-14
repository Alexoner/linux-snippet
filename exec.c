#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
    char *const ps_argv[]={"ps","-o","pid,ppid,pgrp,session,tpgid,comm",NULL};//arguments
    char *const ps_envp[]={"PATH=/bin:/usr/bin","TERM=console",NULL};//environment varible
    execl("/bin/ps","ps","-o","pid,ppid,pgrp,session,tpgid,comm",NULL);//list the arguments
    /*execv("/bin/ps",ps_argv);//contains a vector to arguments
    execle("/bin/ps","ps","-o","pid,ppid,pgrp,session,tpgid,comm",NULL,ps_envp);//environment varible
    execve("/bin/ps",ps_argv,ps_envp);
    execlp("ps","ps","-o","pid,ppid,pgrp,session,tpgid,comm",NULL);//no need to specify the path
    execvp("ps",ps_argv);*/
    perror("exec ps");
    exit(1);
    return 0;
}
