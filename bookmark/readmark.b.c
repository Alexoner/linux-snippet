#include <stdio.h>
#define MAX 100
char line[MAX], bookmark[MAX],path[MAX];
int getlines(char *s,FILE *t);
int compr(char *s,char *t);
int strlen(char *s);

int main (int argc,char *argv[])
{
    FILE *fp;
    char c;
    int n,i;
    char tmp[MAX]="abjjl";
    fp=fopen("./bkmk.log","r");
    while(fgets(line,MAX,fp)!=NULL)
       if(n=compr(line,argv[1]))
           while((c=line[n])==' '||c=='\t'||c=='\n')
               n++;
              for(i=0;(c=line[n])!='\0'&&c!='\n';n++,i++)
               path[i]=c;
              if(path!=NULL)
                  printf("%s",path);
              return 0;
}


int compr(char *s,char *t)
{
    int i,j,k;
    for (i=0;*(s+i)!='\0';i++)
    {
        for (j=0,k=i;s[k]==t[j]&&t[j]!='\0';k++,j++)
            if((s[k]==' '||s[k]=='\t')&&t[j]=='\0')
                return i;
    }
    return 0;
}

int strlen(char *s)
{
    int i=0;
    while(*(s+i)!='\0')
        i++;
    return i;
}














