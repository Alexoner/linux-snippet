#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char **argv)
{
    int opt;
    while((opt=getopt(argc,argv,":if:lr"))!=-1)
    {
        switch(opt)
        {
        case 'i':
        case 'l':
        case 'r':
            printf("option:%c\n",opt);
            break;
        case 'f':
            printf("filename:%s\n",optarg);
            break;
        case ':':
            printf("option requires a value\n");
            break;
        case '?':
            printf("unknown option:%c\n",optopt);
            break;
        }
    }
    for(; optind<argc; optind++) /*argv is rewitten by getopt()*/
        printf("non-option argument:%s\n",argv[optind]);
    exit(0);
}
