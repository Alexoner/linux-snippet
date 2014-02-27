/*************************************************************************
    > File Name: keylog.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Thu 17 Jan 2013 01:52:00 PM CST
 ************************************************************************/

#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int fd;
    if (argc < 2)
    {
        printf("usage: %s <device>\n", argv[0]);
        return 1;
    }
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror(argv[1]);
        exit(-1);
    }
    struct input_event ev;

    while (1)
    {
        read(fd, &ev, sizeof(struct input_event));

        if (ev.type == 1)
            printf("key %i state %i\n", ev.code, ev.value);

    }
}
