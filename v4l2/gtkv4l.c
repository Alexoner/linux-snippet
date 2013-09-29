/*************************************************************************
    > File Name: gtkv4l.c
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Sat 05 Jan 2013 01:43:06 AM CST
 ************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <linux/videodev2.h>
#include<stdio.h>
int main(int argc,char **argv)
{
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    struct v4l2_requestbuffers reqbuf;
    struct
    {
        void *start;
        size_t length;
    }*buffers;
    int fd=open("/dev/video0",O_RDWR);
    if(fd<0)
    {
        printf("Open failed!\n");
        exit(-1);
    }
    if(-1==ioctl(fd,VIDIOC_QUERYCAP,&cap))
    {
        printf("Can't get cap!\n");
        exit(-1);
    }
    if(cap.capabilities&V4L2_CAP_STREAMING)
        printf("It support streaming i/O\n");

    fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width=640;
    fmt.fmt.pix.height=480;
    fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_YYUV;
    if(-1==ioctl(fd,VIDIOC_S_FMT,&fmt))
    {
        printf("FMT MON failed\n");
        exit(-1);
    }

    reqbuf.type=V4L2_MEMORY_MMAP;
    if(-1==ioctl(fd,VIDIOC_REQBUFS,&reqbuf))
    {
        printf("REQUES failed \n");
        exit(-1);
    }


    buffers=calloc(reqbuf.count,sizeof(*buffers));

    int i;
    for(i=0; i<reqbuf.count; i++)
    {
        struct v4l2_buffer buffer;
        buffer.type=reqbuf.type;
        buffer.memory=V4L2_MEMORY_MMAP;
        buffer.index=i;
        if (ioctl(fd,VIDIOC_QUERYBUF,&buffer)==-1)
        {
            printf("QUERYBUF failes\n");
            exit(-1);
        }

        buffers[i].start=mmap(NULL,buffer.length,PROT_READ|PROT_WRITE,
                              MAP_SHARED,fd,buffer.m.offset);
    }

    for(i=0; i<reqbuf.count; i++)
    {
        struct v4l2_buffer buf;
        buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory=V4L2_MEMORY_MMAP;
        buf.index=i;
        if (-1 ==ioctl (fd,VIDIOC_QBUF,&buf)) printf ("QBUF failed\n");
    }
    int type =V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1==ioctl(fd,VIDIOC_STREAMON,&type)) printf("STREAMON failed\n");
    while(cam_lock!=-1)
    {
        struct v4l2_buffer buf;
        buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory=V4L2_MEMORY_MMAP;
        if (-1==ioctl (fd,VIDIOC_DQBUF,&buf)) printf("DQUBUF failed\n");
        if(-1==ioctl(fd,VIDIOC_QBUF,&buf)) printf("Go to QBUF failed\n");
    }
    GtkWidget *window ,*drawarea;
    gtk_init (&argc,&argv);
    window=gtk_window_new (GTK_WINDOW_TOPLEVEL);
    drawarea =gtk_drawing_area_new();
    gtk_container_add (GTK_CONTAINER(window),drawarea);
    /*gtk_drawing_area_size (GTK_DRAWING_AREA (drawarea),
                           COLS,ROWS);
    gdk_draw_rgb_image (widget->window,widget->style->fg_gc
                        [GTK_STATE_NORMAL],0,0,COLS,ROWS,
                        GDK_RGB_DITHER_NONE,buffers,COLS*3);*/
    gtk_widget_show_all(window);
    close (fd);
    munmap(buffers, buffers->length);
    free (buffers);
    return 0;
}
