/*************************************************************************
    > File Name: keylog.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Thu 17 Jan 2013 01:52:00 PM CST
 ************************************************************************/

/**
 * Check /var/log/Xorg.0.log for correct device file,
 * corresponding to the event you are interested in, to read from.
 * An entry fro /var/log/Xorg.0.log as follows indicates that you should
 * watch /dev/input/event1 for Sleep Button press event.
 * Sleep Button entry:
 * [    34.744] (II) config/udev: Adding input device Sleep Button (/dev/input/event1)
 *
 */

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
            printf("[%ld.%06ld] key %i state %i\n",
                   ev.time.tv_sec,
                   ev.time.tv_usec,
                   ev.code, ev.value);

    }
}
