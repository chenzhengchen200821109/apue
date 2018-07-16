#include "apue.h"

int dup2_u(int oldfd, int newfd)
{
    long max_open_file = sysconf(_SC_OPEN_MAX);
    int dupfd;
    int openfd[newfd];
    int count, i = 0;

    /* case 1: oldfd is not a valid file descriptor */
    if ((long)oldfd >= max_open_file || oldfd < 0) {
        errno = EBADF;
        return -1;
    }

    /* check if oldfd is a open fd */
    dupfd = dup(oldfd);
    if (dupfd == -1) { /* dup failed */
        /* oldfd is not a open valid fd */
        if (errno == EBADF) {
            errno = EBADF;
            return -1;
        } 
        else if (errno == EMFILE) { /* oldfd is a open valid fd but got too many fds */
            if ((long)newfd >= max_open_file || newfd < 0) {
                errno = EMFILE;
                return -1;
            } else { /* newfd is a open valid fd */
                close(newfd);
                dupfd = dup(oldfd);
                return dupfd;
            }
        } 
        else if (errno == EINTR) { /* interrupted by signal handler */
            errno = EINTR;
            return -1;
        }
    }  
    /* dup success */
    close(dupfd); /* close just dupped fd */

    if ((long)newfd >= max_open_file || newfd < 0) {
        errno = EBADF;
        return -1;
    } 
    /* newfd is a valid fd */
    else {
        /* case 2: newfd is equal to oldfd */
        if (newfd == oldfd) {
            return newfd;
        }
        /* case 3: newfd is a valid but open fd */
        if ((dupfd = dup(oldfd)) > newfd) {
            close(newfd);
            close(dupfd);
            dupfd = dup(oldfd);
            return dupfd;
        }
        /* case 4: newfd is a valid but not open fd */
        else {
            close(dupfd);
            while ((dupfd = dup(oldfd)) <= newfd) {
                if (dupfd == -1)
                    break;
                openfd[i] = dupfd;
                i++;
            }
            count = i - 1;
            /* Debug purpose */
            //printf("count = %d\n", count);
            //printf("openfd[%d] = %d\n", count, openfd[count]);
            if (dupfd != -1) {
                close(dupfd);
            } 
            for (i = 0; i < count; i++)
                close(openfd[i]); /* close temporary open fd */
            return openfd[count];
        }
    }
}

