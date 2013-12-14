#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#define _GNU_SOURCE

int main(int argc,char **argv)
{
    int opt;/*take the return value of getopt_long*/
    struct option longopts[]=
    {/*the long option sructure*/
        {"initialize",0,NULL,'i'},/*last member:value*/
        {"file",1,NULL,'f'},
        {"list",0,NULL,'l'},
        {"restart",0,NULL,'r'},
        {0,0,0,0}
    };
    while((opt=getopt_long(argc,argv,":if:lr",longopts,NULL))!=-1)
    {
        switch(opt)
        {
            case 'i':
            case 'l':
            case 'r':
                printf("option:%c\n",opt);
                break;
            case 'f':
                printf("filename :%s\n",optarg);
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option:%c\n",optopt);
                break;
        }
    }
    for(;optind<argc;optind++)
        printf("non-option argument:%s\n",argv[optind]);
    exit(0);
}
