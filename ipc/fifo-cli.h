/*************************************************************************
    > File Name: fifo-cli.h
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Mon 05 Aug 2013 05:16:49 PM CST
 ************************************************************************/
#ifndef FIFO_CLI_H
#define FIFO_CLI_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SERVER_FIFO_PATH            "/tmp/serv_fifo"
#define CLIENT_FIFO_PATH            "/tmp/cli_%d_fifo"

#define BUFFER_SIZE                 20

struct data
{
    pid_t client_pid;
    char data[BUFFER_SIZE-1];
};

#endif
