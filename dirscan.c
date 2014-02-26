#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void printdir(char *dir,int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if((dp=opendir(dir))==NULL)
    {
        fprintf(stderr,"cannot open directory:%s\n",dir);
        return ;
    }
    chdir(dir);
    while((entry=readdir(dp))!=NULL)
    {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode))
        {
            /*Found a directory*/
            if(strcmp(".",entry->d_name)==0||
                    strcmp("..",entry->d_name)==0)
                continue;
            else
            {
                printf("%*s%s/\n",depth,"",entry->d_name);
            }
            printdir(entry->d_name,depth+4);//indent
        }
        else
        {
             printf("%*s%s\n",depth,"",entry->d_name);
        }
    }
    chdir("..");//essential
    closedir(dp);
}

int main(int argc,char **argv)
{
    if(argc>=2)
    {
        printf("Directory scan of %s:\n",*(argv+1));
        printdir(*(argv+1),0);
    }
    else
    {
        printf("Directory scan of /home:\n");
        printdir("/home",0);
    }
    printf("done.\n");
    exit(0);
}
