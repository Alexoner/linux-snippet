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

#define FILE_REG            "\033[4;32m"
#define FILE_DIR            "\033[1;34m"
#define FILE_CHR brown
#define FILE_BLK black
#define FILE_FIFO magenta
#define FILE_LINK gray
#define FILE_SOCK cyan

void lsdir(char *dirname)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if ((dp = opendir(dirname)) == NULL)
    {
        perror("open directory");
        exit(-1);
    }
    chdir(dirname);
    while ((entry = readdir(dp)))
    {
        if (strcmp(".", entry->d_name) == 0 ||
                strcmp("..", entry->d_name) == 0)
        {
            continue;
        }
        else
        {
            lstat(entry->d_name, &statbuf);
            if (S_ISREG(statbuf.st_mode))
            {
                printf("%s", FILE_REG);
            }
            else if (S_ISDIR(statbuf.st_mode))
            {
                printf("%s", FILE_DIR);
            }
            else if (S_ISLNK(statbuf.st_mode))
            {
                printf("%s", FILE_LINK);
            }
            printf("%s\t", entry->d_name);
        }
        printf("%s", none);
    }
    printf("%s\n", none);

}

int
main(int argc, char **argv)
{
    /*printf("%sHello, %sworld!%s\n", red, blue, none);*/
    /*printf("%sHello%s, %sworld!\n", green, none, cyan);*/
    /*printf("%s", none);*/

    if (argc >= 2)
    {
        printf("Directory scan of %s\n", *(argv + 1));
        lsdir(*(argv + 1));
    }
    else
    {
        printf("scan of /tmp\n");
        lsdir("/tmp");
    }

    return 0;
}
