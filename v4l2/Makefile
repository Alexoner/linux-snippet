CC=gcc-4.7
#CC=bfin-uclinux-gcc
CFLAGS=-g -Wall
GTKFLAGS=`pkg-config gtk+-2.0 --libs --cflags`

capture_raw_frames:capture_raw_frames.c
	$(CC) $(CFLAGS) -o $@ $^
videoCapture:videoCapture.c
	$(CC) $(CFLAGS) -o $@ $^
gtkv4l:gtkv4l.c
	$(CC) $(CFLAGS) $(GTKFLAGS) $^ -o $@ 
v4l:v4l.o v4l-main.o
	$(CC) $(CFLAGS) $^ -o $@

.PYONY:clean
	
clean:
	rm *.o

v4l.o:v4l.c
	$(CC) $(CFLAGS) -c $^
v4l-main.o:v4l-main.c
	$(CC) $(CFLAGS) -c $^
