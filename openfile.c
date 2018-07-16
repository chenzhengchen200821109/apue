/*
 * open.c : what if we open a file twice?
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {

    int fd1, fd2;

    fd1 = open("open.c", O_RDONLY);
    fd2 = open("open.c", O_RDONLY);

    printf("file descriptor is %d for the first open call\n", fd1);
    printf("file descriptor is %d for the second open call\n", fd2);

    exit(0);
}


