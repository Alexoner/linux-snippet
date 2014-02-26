/*************************************************************************
    > File PATH: fifo-serv.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Mon 05 Aug 2013 05:27:54 PM CST
 ************************************************************************/

#include "fifo-cli.h"
#include <ctype.h>
int main()
{
    int server_fifo_fd, client_fifo_fd;
    struct data my_data;
    int times_to_send;
    char client_fifo[256];
    server_fifo_fd = open(SERVER_FIFO_PATH, O_WRONLY);
    if (server_fifo_fd == -1)
    {
        fprintf(stderr, "Sorry, no server\n");
        exit(EXIT_FAILURE);
    }
    my_data.client_pid = getpid();
    sprintf(client_fifo, CLIENT_FIFO_PATH, my_data.client_pid);
    if (mkfifo(client_fifo, 0777) == -1)
    {
        fprintf(stderr, "Sorry, can’t make %s\n", client_fifo);
        exit(EXIT_FAILURE);
    }
    for (times_to_send = 0; times_to_send < 5; times_to_send++)
    {
        sprintf(my_data.data, "Hello from %d", my_data.client_pid);
        printf("%d sent %s, ", my_data.client_pid, my_data.data);
        write(server_fifo_fd, &my_data, sizeof(my_data));
        client_fifo_fd = open(client_fifo, O_RDONLY);
        if (client_fifo_fd != -1)
        {
            if (read(client_fifo_fd, &my_data, sizeof(my_data)) > 0)
            {
printf("received: %s\n", my_data.data);
            }
            close(client_fifo_fd);
        }
    }
    close(server_fifo_fd);
    unlink(client_fifo);
    exit(EXIT_SUCCESS);
}
