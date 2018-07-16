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

int main(void)
{
    int i, err;
    ssize_t n;
    off_t off = 0;
    int openReqs, numReqs = NBUF;

    for ( ; ; ) {
        for (i = 0; i < NBUF; i++) {
            /* be ready for read */
            bufs[i].flag = UNUSED;
            bufs[i].aiocb.aio_buf = bufs[i].data;
            bufs[i].aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
            bufs[i].aiocb.aio_fildes = STDIN_FILENO;
            bufs[i].aiocb.aio_offset = off;
            off += BSZ;
            bufs[i].aiocb.aio_nbytes = BSZ;
            if (aio_read(&bufs[i].aiocb) < 0)
                err_sys("aio_read failed");
        }
        openReqs = NBUF;
        /* loop until all buffer is ready for write */
        while (openReqs > 0) {
            for (i = 0; i < NBUF; i++) {
                err = aio_error(&bufs[i].aiocb);
                switch (err) {
                    case 0:
                        bufs[i].flag = READ_READY;
                        break;
                    case EINPROGRESS:
                        break;
                    case -1:
                        err_sys("aio_error failed");
                        break;
                    default:
                        err_exit(err, "read failed");

                }
                if (bufs[i].flag != UNUSED)
                    openReqs--;
            }
        }
        /* be ready for write */
        for (i = 0; i < NBUF; i++) {   
            if ((n = aio_return(&bufs[i].aiocb)) < 0) {
                //err_sys("aio_return failed");
                //printf("bufs[%d]: n = %d\n", i, n);
            }
            else {
                //printf("bufs[%d]: n = %d\n", i, n);
                write(STDOUT_FILENO, bufs[i].data, n);
                bufs[i].flag = UNUSED;
            }
        }
    }
}
