#include <stdio.h>
#define MAX 100
int main()
{
    FILE *fp;
    long origin=0;
    char line[MAX];
    fp=fopen("./qwer","r");
    fgets(line,MAX,fp);
    origin=ftell(fp);
    fseek(fp,0,origin);
    fputs("this is modified!",fp);
    fp=fopen("./test","r+");
    fputs("this is newly added!",fp);
    return 0;
}

    
