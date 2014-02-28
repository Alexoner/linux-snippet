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
 * Existing project dealing with linux event are: acpid,systemd,X11.
 *
 * POLL:
 *
 * inotify:
 *
 */

#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct connection
{
    /* file descriptor */
    int fd;

    /* process incoming data on the connection */
    /* ??? suggest passing a pointer to this connection struct */
    void (* process)(int fd);

    /* Optional.  Used by find_connection_name() to find the connection for a
       specific file.  Set to NULL if not specified.  Memory will be freed
       with free() when connection is deleted. */
    char *pathname;

    /* 0 indicates this is probably not a keyboard device */
    int kybd;
};


/*-----------------------------------------------------------------*
 * open a single input layer file for input  */
int open_inputfile(const char *filename)
{
    int fd;
    struct connection c;

    /* O_CLOEXEC: Make sure scripts we exec() (in event.c) don't get our file
       descriptors. */
    fd = open(filename, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
    /*fd = open(filename, O_RDONLY);*/

    if (fd >= 0)
    {
        char evname[256];

        /* if this file doesn't have events we need, indicate failure */
        /*if (!has_event(fd))*/
        /*{*/
        /*close(fd);*/
        /*return -1;*/
        /*}*/

        /* get this event file's name for debugging */
        strcpy(evname, "Unknown");
        printf("opened file %s (%s)\n", filename, evname);
        ioctl(fd, EVIOCGNAME(sizeof(evname)), evname);

        /*acpid_log(LOG_DEBUG, "input layer %s (%s) "*/
        printf( "input layer %s (%s) "
                "opened successfully, fd %d\n", filename, evname, fd);

        /* add a connection to the list */
        c.fd = fd;
        /*c.process = process_input;*/
        /* delete_connection() will free */
        c.pathname = malloc(strlen(filename) + 1);
        if (c.pathname)
            strcpy(c.pathname, filename);
        /* assume not a keyboard until we see a scancode */
        c.kybd = 0;

        /*if (add_connection(&c) < 0)*/
        /*{*/
        /*close(fd);*/
        /*acpid_log(LOG_ERR,*/
        /*"can't add connection for input layer %s (%s)",*/
        /*filename, evname);*/
        /*return -1;*/
        /*}*/

        return fd;  /* success */
    }

    /* open unsuccessful */
    return 0;
}


int main(int argc, char **argv)
{
    int fd;
    if (argc < 2)
    {
        printf("usage: %s <device>\n", argv[0]);
        return 1;
    }
    /*fd = open(argv[1], O_RDONLY);*/
    fd = open_inputfile(argv[1]);
    if (fd < 0)
    {
        perror(argv[1]);
        exit(-1);
    }
    struct input_event ev;

    while (1)
    {
        read(fd, &ev, sizeof(struct input_event));

        if (ev.type == EV_KEY)
        {
            printf("[%ld.%06ld] key %i state %i\n",
                   ev.time.tv_sec,
                   ev.time.tv_usec,
                   ev.code, ev.value);
            printf("The pressed key is: %s\n",
                   ev.code == KEY_POWER ? "power" : "suspend");
        }

    }
}
