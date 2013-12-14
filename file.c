/*************************************************************************
    > File Name: ../file.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Sun 06 Jan 2013 05:03:08 PM CST
 ************************************************************************/

#include<stdio.h>

int main(int argc,char **argv)
{
    FILE *fp=fopen("log1","r+");
    if(!fp)
        return 0;
	printf("%ld\n",ftell(fp));
	fseek(fp,-2,SEEK_END);
	fputc('@',fp);
	printf("%ld\n",ftell(fp));
	fclose(fp);
    return 1;
}

