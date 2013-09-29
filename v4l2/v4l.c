/*************************************************************************
    > File Name: v4l.c
    > Author: onerhao
    > Mail: haodu@hustunique.com
    > Created Time: Tue 08 Jan 2013 07:38:11 PM CST
    the V4L2 utility
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>
#include "v4l.h"


int xioctl(int fd,int request,void *arg)
{
    int r;
    do
    {
        r=ioctl(fd,request,arg);
    } while(-1==r&&errno==EINTR);//interupted function call

    return r;
}

int query_cap(int fd)
{
    struct v4l2_capability cap;
    int ret=xioctl(fd,VIDIOC_QUERYCAP,&cap);

    if(ret==-1)
    {
        perror("VIDIOC_QUERYCAP");
        return ret;
    }
    if(!(cap.capabilities&V4L2_CAP_VIDEO_CAPTURE))
    {
        fprintf(stdout,"not a video capture device\n");
        return ret;
    }
    if(!(cap.capabilities&V4L2_CAP_STREAMING))
    {
        fprintf(stdout,"doesn't support streaming i/o");
        return ret;
    }
    return 0;
}

int set_input(int fd)
{
    struct v4l2_input input;
    int std_cap=0,dv_timings_cap=0;
    int ret,i;

    fprintf(stdout,"enumerating input\n");
    for(i=0;; i++)
    {
        input.index=i;
        ret=xioctl(fd,VIDIOC_ENUMINPUT,&input);
        if(ret<0)
        {
            break;
        }
        fprintf(stdout,"input[%d]: %s\n",input.index,input.name);
    }

    i=0;
    ret=xioctl(fd,VIDIOC_S_INPUT,&i);
    if(ret<0)
    {
        perror("VIDIOC_S_INPUT");
        return ret;
    }
    input.index=i;

    if (input.capabilities & V4L2_IN_CAP_STD)
    {
        std_cap = 1;
        fprintf(stdout,"input capability:std ");
    }
    if (input.capabilities & V4L2_IN_CAP_CUSTOM_TIMINGS)
    {
        dv_timings_cap = 1;
        fprintf(stdout,
                "input capability: dv_timings ");
    }

    sleep(1);
    return 0;
}



int set_std(int fd)
{
    v4l2_std_id std;
    int ret;
    ret=xioctl(fd,VIDIOC_QUERYSTD,&std);
    if(!ret)
    {
        printf("sensed video standard is: %x\n",
               (unsigned int)std);
        ret=xioctl(fd,VIDIOC_S_STD,&std);
        if(ret==-1)
        {
            perror("VIDIOC_S_STD");
        }
    }
    return ret;
}


int get_dv_timings(int fd)
{
    struct v4l2_dv_timings dv_timings;
    struct v4l2_bt_timings bt;
    int ret;

    ret = ioctl(fd, VIDIOC_G_DV_TIMINGS, &dv_timings);
    if (ret) {
        printf("ioctl VIDIOC_G_DV_TIMINGS error\n");
        return ret;
    }
    bt = dv_timings.bt;
    fprintf(stdout,
            "dv_timings: %s,width=%d,height=%d\n"
            "hfrontporch=%d,hsync=%d,hbackporch=%d,"
            "vfrontporch=%d,vsync=%d,vbackporch=%d\n",
            bt.interlaced ? "interlaced" : "progressive",
            bt.width, bt.height,
            bt.hfrontporch, bt.hsync, bt.hbackporch,
            bt.vfrontporch, bt.vsync, bt.vbackporch);
    if (bt.interlaced)
        printf("il_vfrontporch=%d,il_vsync=%d,il_vbackporch=%d\n",
               bt.il_vfrontporch, bt.il_vsync,
               bt.il_vbackporch);
    return 0;
}


int query_crop(int fd)
{
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    int ret;
    CLEAR(cropcap);
    crop.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret=xioctl(fd,VIDIOC_CROPCAP,&cropcap);
    if(ret==0)
    {
        crop.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c=cropcap.defrect;//reset to default

        if(-1==xioctl(fd,VIDIOC_S_CROP,&crop))
        {
            switch(errno)
            {
            case EINVAL:
                fprintf(stderr,"Cropping not supported\n");
                break;
            default:
                break;
            }
        }
    }
    else
    {
        //errors ignored
    }
    return ret;
}


int set_fmt(int fd)
{
    struct v4l2_format fmt;
    struct v4l2_fmtdesc fmtdesc;
    int ret,i,image_size;

    fprintf(stdout,"enum format\n");

    for(i=0;; i++)
    {
        fmtdesc.index=i;
        fmtdesc.type=V4L2_CAP_VIDEO_CAPTURE;
        ret=xioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc);
        if(ret<0)
            break;
        fprintf(stdout,"format[%d]:%s\n",fmtdesc.index,fmtdesc.description);
    }

    fmt.type=V4L2_CAP_VIDEO_CAPTURE;
    //fmt.fmt.pix.width=640;//replace
    fmt.fmt.pix.width=1366;//replace
    //fmt.fmt.pix.height=480;//replace
    fmt.fmt.pix.height=768;//replace
    fmt.fmt.pix.pixelformat=fmtdesc.pixelformat;
    fmt.fmt.pix.field=V4L2_FIELD_ANY;

    ret=xioctl(fd,VIDIOC_S_FMT,&fmt);
    if(ret==-1)
    {
        perror("VIDIOC_S_FMT");
        return ret;
    }
    printf("Negotiated formats:%dx%u\n",
           fmt.fmt.pix.width,fmt.fmt.pix.height);
    printf("field order:");
    switch(fmt.fmt.pix.field)
    {
    case V4L2_FIELD_NONE:
        printf("progressive\n");
        break;
    case V4L2_FIELD_INTERLACED:
        printf("interlaced\n");
        break;
    case V4L2_FIELD_SEQ_TB:
        printf("sequential TB\n");
        break;
    case V4L2_FIELD_SEQ_BT:
        printf("sequential BT\n");
        break;
    default:
        printf("unknown field order\n");
        break;
    }

    printf("size = %u\n", fmt.fmt.pix.sizeimage);
    printf("bytesperline = %u\n", fmt.fmt.pix.bytesperline);
    printf("pixelformat: ");
    switch (fmt.fmt.pix.pixelformat)
    {
        case V4L2_PIX_FMT_MJPEG:
            fprintf(stdout,"MJPEG\n");
            break;
        case V4L2_PIX_FMT_UYVY:
            printf("UYVY\n");
            break;
        case V4L2_PIX_FMT_YUYV:
            printf("YUYV\n");
            break;
        case V4L2_PIX_FMT_RGB565:
            printf("RGB565\n");
            break;
        case V4L2_PIX_FMT_RGB444:
            printf("RGB444\n");
            break;
        default:
            printf("unknown pixel format\n");
            break;
    }
    image_size = fmt.fmt.pix.sizeimage;

    return 0;
}


int set_rate(int fd,int rate)
{
    struct v4l2_streamparm param;
    struct v4l2_fract *tpf=&param.parm.capture.timeperframe;
    int ret;

    param.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret=xioctl(fd,VIDIOC_G_PARM,&param);
    if(ret<0)
    {
        fprintf(stdout,"gettting frame setting is not supported\n");
    }
    else if(rate)
    {
        fprintf(stdout,"default rate = %d\n",
                tpf->denominator/tpf->numerator);
        tpf->numerator=1;
        tpf->denominator=rate;
        ret=xioctl(fd,VIDIOC_S_PARM,&param);
        if(ret==-1)
        {
            perror("VIDIOC_S_PARM");
            return ret;
        }
        fprintf(stdout,"set frame rate to %d\n",
                tpf->denominator/tpf->numerator);
    }
    return 0;
}


void * init_mmap(int fd,struct head_buffer *hbuffer)
{
    struct v4l2_requestbuffers req;
    int i,ret;
    CLEAR(req);

    req.count=4;
    req.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory=V4L2_MEMORY_MMAP;

    ret=xioctl(fd,VIDIOC_REQBUFS,&req);
    if(-1==ret)
    {
        if(EINVAL==errno)
        {
            fprintf(stderr,"memory mapping not supported\n");
        }
        return NULL;
    }
    else
    {
        fprintf(stdout,"request %d buffers success\n",req.count);
    }

    if(req.count<2)
    {
        fprintf(stderr,"Insufficient buffer memory on device\n");
        return NULL;
    }

    hbuffer->buffers=calloc(req.count,sizeof(*hbuffer->buffers));
    if(!hbuffer->buffers)
    {
        fprintf(stderr,"Out of memory\n");
    }
    hbuffer->n_buffers=req.count;
    for(i=0; i<req.count; ++i)
    {
        struct v4l2_buffer buf;
        CLEAR(buf);

        buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory=V4L2_MEMORY_MMAP;
        buf.index=i;

        ret=xioctl(fd,VIDIOC_QUERYBUF,&buf);
        if(-1==ret)
        {
            perror("VIDIOC_QUERYBUF");
            free(hbuffer->buffers);
            return NULL;
        }

        (hbuffer->buffers)[i].length=buf.length;
        (hbuffer->buffers)[i].start=
            mmap(NULL,//start anywhere
                 buf.length,
                 PROT_READ|PROT_WRITE,//required
                 MAP_SHARED,//recommended
                 fd,
                 buf.m.offset);

        if(MAP_FAILED==(hbuffer->buffers)[i].start)
        {
            perror("mmap");
            free(hbuffer->buffers);
            return NULL;
        }
    }
    return hbuffer;
}

void init_userp(int fd,unsigned int buffer_size,
        struct head_buffer *hbuffer)
{
    struct v4l2_requestbuffers req;
    int i;

    CLEAR(req);

    req.count  = 4;
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_USERPTR;

    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            fprintf(stderr, "%s does not support "
                    "user pointer i/o\n", "/dev/video0");
            exit(EXIT_FAILURE);
        }
        else
        {
            perror("VIDIOC_REQBUFS");
            return ;
        }
    }

    hbuffer->buffers = calloc(4, sizeof(*hbuffer->buffers));

    if (!hbuffer->buffers) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    hbuffer->n_buffers=req.count;

    for (i = 0; i < 4; ++i) {
        hbuffer->buffers[i].length = buffer_size;
        hbuffer->buffers[i].start = malloc(buffer_size);

        if (!hbuffer->buffers[i].start) {
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
}



void * init_read(int fd,unsigned int buffer_size,
        struct head_buffer *hbuffer)
{
    hbuffer->buffers = calloc(1, sizeof(*hbuffer->buffers));

    if (!hbuffer->buffers) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    hbuffer->buffers[0].length = buffer_size;
    hbuffer->buffers[0].start = malloc(buffer_size);

    if (!hbuffer->buffers[0].start) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    return hbuffer->buffers;
}

int init_device(int fd)
{
    query_crop(fd);
    query_cap(fd);
    set_input(fd);
    set_fmt(fd);
    return 0;
}

int uninit_device(int fd,enum io_method io,
        struct head_buffer *hbuffer)
{
    int i,ret;
    switch(io)
    {
        case IO_METHOD_READ:
            break;
        case IO_METHOD_MMAP:
            for(i=0;i<hbuffer->n_buffers;i++)
            {
                ret=munmap(hbuffer->buffers[i].start,
                        hbuffer->buffers[i].length);
                if(ret==-1)
                {
                    perror("munmap");
                    return ret;
                }
            }
            break;
        case IO_METHOD_USERPTR:
            break;
    }
    return 0;
}


void process_image(const void *p,int size)
{
    static int frame_number=0;
    char filename[20];
    FILE *fp=NULL;
    if(!p)
    {
        fprintf(stdout,"no frame to process\n");
        return;
    }
    frame_number++;
    sprintf(filename,"./image/frame-%d.raw",frame_number);
    fp=fopen(filename,"w");
    fwrite(p,size,1,fp);
    fprintf(stdout,"frame:%d,address: %p,size: %d\n",
            frame_number,
            p,
            size);
    fflush(fp);
    fclose(fp);
}

int read_frame(int fd,int io,
        struct head_buffer *hbuffer)
{
    struct v4l2_buffer buf;
    //struct head_buffer *hbuffer=NULL;
    unsigned int i,ret=0;

    //hbuffer=(struct head_buffer*)malloc(sizeof(struct head_buffer));
    //memset(hbuffer,0,sizeof(struct head_buffer));

    switch (io) {
    case IO_METHOD_READ:
        //ret=read(fd,buffers[0].start,buffers[0].length);
        if (-1 == ret) {
            switch (errno) {
            case EAGAIN:
                return ret;

            case EIO:
                /* Could ignore EIO, see spec. */

                /* fall through */

            default:
                perror("read");
                return ret;
            }
        }

        //process_image(buffers[0].start, buffers[0].length);
        break;

    case IO_METHOD_MMAP:
        //hbuffer=init_mmap(fd,hbuffer);
        //start_capturing(fd,io,hbuffer);
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
            switch (errno) {
            case EAGAIN:
                return 0;

            case EIO:
                /* Could ignore EIO, see spec. */

                /* fall through */

            default:
                //perror("VIDIOC_DQBUF");
                break;
            }
        }

        //assert(buf.index < n_buffers);

        process_image(hbuffer->buffers[buf.index].start, buf.bytesused);

        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
            perror("VIDIOC_QBUF");
        break;

    case IO_METHOD_USERPTR:
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_USERPTR;

        if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
            switch (errno) {
            case EAGAIN:
                return 0;

            case EIO:
                /* Could ignore EIO, see spec. */

                /* fall through */

            default:
                perror("VIDIOC_DQBUF");
            }
        }

        for (i = 0; i < hbuffer->n_buffers; ++i)
            if (buf.m.userptr == (unsigned long)(hbuffer->buffers[i].start)
                    && buf.length == hbuffer->buffers[i].length)
                break;

        assert(i < hbuffer->n_buffers);

        process_image((void *)buf.m.userptr, buf.bytesused);

        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
            perror("VIDIOC_QBUF");
        break;
    }//switch io

    return 1;
}



void start_capturing(int fd,int io,
        struct head_buffer *hbuffer)
{
    unsigned int i;
    enum v4l2_buf_type type;

    switch (io) {
    case IO_METHOD_READ:
        /* Nothing to do. */
        break;

    case IO_METHOD_MMAP:
        for (i = 0; i < hbuffer->n_buffers; ++i) {
            struct v4l2_buffer buf;

            CLEAR(buf);
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index = i;

            if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                perror("VIDIOC_QBUF");
        }
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
            perror("VIDIOC_STREAMON");
        else
            fprintf(stdout,"stream on success\n");
        break;

    case IO_METHOD_USERPTR:
        for (i = 0; i < hbuffer->n_buffers; ++i) {
            struct v4l2_buffer buf;

            CLEAR(buf);
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_USERPTR;
            buf.index = i;
            buf.m.userptr = (unsigned long)hbuffer->buffers[i].start;
            buf.length = hbuffer->buffers[i].length;

            if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                perror("VIDIOC_QBUF");
        }
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
            perror("VIDIOC_STREAMON");
        break;
    }
}


void stop_capturing(int fd,enum io_method io)
{
    enum v4l2_buf_type type;

    switch (io) {
    case IO_METHOD_READ:
        /* Nothing to do. */
        break;

    case IO_METHOD_MMAP:
    case IO_METHOD_USERPTR:
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
            perror("VIDIOC_STREAMOFF");
        break;
    }
}
