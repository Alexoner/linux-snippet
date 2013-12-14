/*************************************************************************
    > File Name: dlopen.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: 2013年10月11日 星期五 16时27分54秒
 ************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

int main(int argc,char **arv)
{
	void *handle;
	double (*cosine)(double);
	char *error;

	handle = dlopen("/usr/lib/x86_64-linux-gnu/libm.so",RTLD_LAZY);
	if(!handle)
	{
		fputs(dlerror(),stderr);
		exit(1);
	}
	cosine=dlsym(handle,"cos");
	if((error=dlerror())!=NULL)
	{
		fputs(error,stderr);
		exit(1);
	}
	printf("%f\n",(*cosine)(2.0));
	dlclose(handle);
}
