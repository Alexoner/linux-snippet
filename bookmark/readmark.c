#include <stdio.h>
#include <string.h>
#define MAX 128
FILE *fp;
char line[MAX];
char path[MAX];
int main(int argc,char *argv[])
{
    int i;
    char *c;
    fp=fopen("/home/administrator/.bkmk/bkmk.dat","r");
    if(fp)
    {
        fgets(line,MAX,fp);
        while(fgets(line,MAX,fp)!=NULL)
        {
            if((c=strstr(line,argv[1])))
            {
                i=strlen(argv[1]);
                if(*(c+i)==':'&&line[0]==argv[1][0])
                printf("%s",line+(i+1));
            }
        }
    }
       return 0;
}

