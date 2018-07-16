#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include "apue.h"
#include <string.h>

#define FIFO_SERVER "/tmp/fifoserver"

static int handle_client(char *pathname);
static int write_nonblock_openfifo(char *pathname);

int main(void)
{
    int readfd, writefd;
    pid_t pid;

    unlink(FIFO_SERVER);
    if (mkfifo(FIFO_SERVER, O_CREAT) < 0)
        err_sys("mkfifo_error");
    handle_client(FIFO_SERVER);

    return 0;
}

int handle_client(char *pathname)
{
    int ret;
    ret = write_nonblock_openfifo(pathname);
    switch(ret) {
        case 0:
            printf("no process has the fifo open for reading\n");
            break;
        case -1:
            printf("open error except for ENXIO\n");
            break;
        default:
            printf("open server ok\n");
    }
    return 0;
}

int read_nonblock_openfifo(char *pathname)
{
    return open(pathname, O_RDONLY | O_NONBLOCK, FILE_MODE);
}

int read_block_openfifo(char *pathname)
{
    return open(pathname, O_RDONLY, FILE_MODE);
}

int write_nonblock_openfifo(char *pathname)
{
    int ret;
    if ((ret = open(pathname, O_WRONLY | O_NONBLOCK, FILE_MODE)) == -1) {
        if (errno == ENXIO)
            return 0; /* indicates no process has the file open for reading */
        else {
            printf("error occurres: %s\n", strerror(errno));
            return -1;
        }
    }
    return ret;
}

int write_block_open(char *pathname)
{
    return open(pathname, O_WRONLY, FILE_MODE);
}
