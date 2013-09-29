/*************************************************************************
    > File Name: v4l.h
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Tue 08 Jan 2013 04:52:18 AM CST
 ************************************************************************/
#include <stdlib.h>

#ifndef V4L_H
#define V4L_H

#define CLEAR(x) memset(&(x), 0, sizeof(x))


struct buffer
{
    void *start;
    size_t length;
};

struct head_buffer
{
  struct buffer *buffers;
  int n_buffers;
};

enum io_method
{
    IO_METHOD_READ,
    IO_METHOD_MMAP,
    IO_METHOD_USERPTR,
};

int xioctl(int fd,int request,void *arg);
int query_cap(int fd);
int set_input(int fd);
int set_std(int fd);
int get_dv_timings(int fd);
int query_crop(int fd);
int set_fmt(int fd);
int  set_rate(int fd,int rate);
void * init_mmap(int fd,struct head_buffer *hbuffer);
void init_userp(int fd,unsigned int buffer_size,
                struct head_buffer *hbuffer);
void * init_read(int fd,unsigned int buffer_size,
                 struct head_buffer *hbuffer);
void process_image(const void *p,int size);
int read_frame(int fd,int io,
               struct head_buffer *hbuffer);
void start_capturing(int fd,int io,struct head_buffer *hbuffer);
void stop_capturing(int fd,enum io_method io);

#endif
