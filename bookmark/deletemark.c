#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 100

/*this c source file is written to read the bookmark file
 "bkmk.dat" and to analyse whether there is a match bookmark name
 for argv[1],which applies the same method as in "readmark.c"
 and then,write lines into a new file "tmp" without the matched
 line.Rename "tmp" . Done!
 */

int main(int argc,char **argv)
{
    FILE *fpin,*fpout;
    char line[MAX];
    int i;
    if((fpin=fopen("/home/administrator/.bkmk/bkmk.dat","r"))==NULL)
    {
        printf("error in opening file bkmk.dat\n");
        return 0;
    }
    if((fpout=fopen("/home/administrator/.bkmk/tmp","w"))==NULL)
    {
        printf("error in opening file tmp \n");
        return 0;
    }
    fgets(line,MAX,fpin);
    fputs(line,fpout);
    while(fgets(line,MAX,fpin))
    {
        if(strstr(line,argv[1]))
        {
            i=strlen(argv[1]);
            if(line[0]!=*argv[1]||*(line+i)!=':')
                fputs(line,fpout);
            else
                printf("success\n");
        }
        else
            fputs(line,fpout);
    }
    fclose(fpin);
    fclose(fpout);
    rename("/home/administrator/.bkmk/bkmk.dat","/home/administrator/.bkmk/bkmk.dat.bak");
    //system("mv /home/administrator/.bkmk/tmp /home/administrator/.bkmk/bkmk.dat");
    if(rename("/home/administrator/.bkmk/tmp","/home/administrator/.bkmk/bkmk.dat"))
        printf("error in creating file bkmk.dat\n");//why error?
    return 0;
}



