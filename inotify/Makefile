VPATH=../

CC=gcc
CFLAGS=-Wall -g
SRCS=inotify_test.c inotify.c event_queue.c apue.c 
OBJS=inotify_test.o inotify.o event_queue.o apue.o

.c.o:
	$(CC) $(CFLAGS) -c $<

all: inotify_test

inotify_test: $(OBJS)
	$(CC) $(CFLAGS) inotify.o inotify_test.o event_queue.o apue.o -o inotify_test

clean:
	rm -f $(OBJS) *.bak inotify_test
