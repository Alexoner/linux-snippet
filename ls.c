#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>



#define red   "\033[0;31m"        /* 0 -> normal ;  31 -> red */
#define cyan  "\033[1;36m"        /* 1 -> bold ;  36 -> cyan */
#define green "\033[4;32m"        /* 4 -> underline ;  32 -> green */
#define blue  "\033[9;34m"        /* 9 -> strike ;  34 -> blue */

#define black  "\033[0;30m"
#define brown  "\033[0;33m"
#define magenta  "\033[0;35m"
#define gray  "\033[0;37m"

#define none   "\033[0m"        /* to flush the previous property */

#define FILE_REG            "\033[4;31m"
#define FILE_DIR            "\033[1;34m"
#define FILE_CHR brown
#define FILE_BLK black
#define FILE_FIFO magenta
#define FILE_LINK gray
#define FILE_SOCK           "\033[0;30m"

#define MAX_LENGTH 80

int All = 0;
int Long = 0;

//compare
int cmp(const void *x, const void *y)
{
    const struct dirent* dx = *(struct dirent**)x;
    const struct dirent* dy = *(struct dirent**)y;
    /*printf("%s,%s,%d\n", dx->d_name, dy->d_name,*/
    /*strcmp(dx->d_name, dy->d_name));*/
    return strcmp(dx->d_name, dy->d_name);
}

//ls a directory
void lsdir(char *dirname)
{
    //directory pointer
    DIR *dp;

    //directory entry pointer
    struct dirent *entry;
    struct stat statbuf;
    //entry entry
    struct dirent* entries[65536];
    int top = -1;
    int l = 0;

    //open directory
    if ((dp = opendir(dirname)) == NULL)
    {
        perror("open directory");
        exit(-1);
    }

    //change directory
    chdir(dirname);
    //read a directory,iterate
    while ((entry = readdir(dp)))
    {
        if (strcmp(".", entry->d_name) == 0 ||
                strcmp("..", entry->d_name) == 0)
        {
            continue;
        }
        else if (All == 0 && entry->d_name[0] == '.')
        {
            continue;
        }
        else
        {
            entries[++top] = entry;
        }
    }

    //quick sort entries
    qsort(entries, top + 1, sizeof(struct entries*), cmp);

    int i;
    for (i = 0; i <= top; i++)
    {
        l += strlen(entries[i]->d_name);
        if (l > MAX_LENGTH)
        {
            printf("\n");
            l = 0;
        }

        lstat(entries[i]->d_name, &statbuf);
        //regular file
        if (S_ISREG(statbuf.st_mode))
        {
            printf("%s", FILE_REG);
        }
        else if (S_ISDIR(statbuf.st_mode))
        {
            //directory
            printf("%s", FILE_DIR);
        }
        else if (S_ISLNK(statbuf.st_mode))
        {
            printf("%s", FILE_LINK);
        }
        if (Long)
        {
            //argument,long information
            printf("%ld\t", statbuf.st_size);
        }
        printf("%s\t", entries[i]->d_name);
        if (Long)
        {
            printf("\n");
        }
        /*top--;*/
    }
    printf("%s\n", none);

}

int
main(int argc, char **argv)
{
    /*printf("%sHello, %sworld!%s\n", red, blue, none);*/
    /*printf("%sHello%s, %sworld!\n", green, none, cyan);*/
    /*printf("%s", none);*/

    int opt;

    char dirname[256];
    printf("All: %d,Long: %d\n", All, Long);
    while ((opt = getopt(argc, argv, ":al")) != -1)
    {
        switch (opt)
        {
        case 'a':
            All = 1;
            /*printf("A");*/
            break;
        case 'l':
            Long = 1;
            break;
        default:
            break;
        }
    }
    if (optind < argc)
    {
        strcpy(dirname, *(argv + optind ));
        printf("Directory scan of %s\n", dirname);
        lsdir(dirname);
    }
    else
    {
        lsdir(".");
    }

    /*else*/
    /*{*/
    /*printf("scan of /tmp\n");*/
    /*lsdir("/tmp");*/
    /*}*/

    return 0;
}
