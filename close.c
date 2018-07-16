/*
 * close.c - what happens if we close a file before open it
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern int errno;

int main()
{
    int status, fd = 3;

    status = close(fd);
    if (status = 0)
        printf("we can close a file before open it\n");
    else {
        printf("error number: %d, %s\n", errno, strerror(errno));
        perror("close error");
        fd = open("close.c", O_RDONLY);
        close(fd);
    }

    exit(0);
}
