#include "apue.h"
#include <ctype.h>
#include <fcntl.h>
#include <aio.h>
#include <errno.h>

#define BSZ 2
#define NBUF 8

enum status {
    UNUSED = 0,
    READ_READY = 1
};

/* define a buf struct */
struct buf {
    enum status flag;
    struct aiocb aiocb;
    unsigned char data[BSZ];
};

struct buf bufs[NBUF];

static void aioSig_usr1(int singo)
{
    int i, err;
    printf("received SIGUSR1\n");
    for (i = 0; i < NBUF; i++) {
        err = aio_error(&bufs[i].aiocb);
        printf("bufs[%d].err = %d\n", i, err);
    }
}

//int main(int argc, char *argv[]) /* got a bug when redirect or pipe */
int main(void)
{
    int i, n, err;
    struct stat sbuf;
    struct aiocb *aiolist[NBUF];
    struct sigevent event;
    struct sigaction sa;
    off_t off = 0;

    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = aioSig_usr1;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        err_sys("sigaction error");

    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIGUSR1;
    /* initialize the buffers */
    for (i = 0; i < NBUF; i++) {
        bufs[i].flag = UNUSED;
        bufs[i].aiocb.aio_buf = bufs[i].data;
        bufs[i].aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
        bufs[i].aiocb.aio_fildes = STDIN_FILENO;
    }
    for ( ; ; ) {
        /* initialize the buffers */
        for (i = 0; i < NBUF; i++) {
            if (bufs[i].flag == UNUSED) {
                bufs[i].aiocb.aio_offset = off;
                bufs[i].aiocb.aio_lio_opcode = LIO_READ;
                off += BSZ;
                bufs[i].aiocb.aio_nbytes = BSZ;
                aiolist[i] = &bufs[i].aiocb;
            }
        }
        if ((err = lio_listio(LIO_NOWAIT, aiolist, NBUF, &event)) == -1)
            err_sys("lio_listio error");
        pause();

        for (i = 0; i < NBUF; i++) {
            if ((n = aio_return(&bufs[i].aiocb)) < 0)
                err_sys("aio_return failed");
            else
                write(STDOUT_FILENO, bufs[i].data, n);
            printf("bufs[%d]: %c%c\n", i, bufs[i].data[0], bufs[i].data[1]);
        }
    }
}
