#include <sys/select.h>
#include <unistd.h>
#include <errno.h>

int usleep_v(useconds_t usec)
{
    struct timeval tv;
    int res;

    tv.tv_sec = 0;
    tv.tv_usec = (long)usec;

    if ((res = select(1, NULL, NULL, NULL, &tv)) == -1) {
        if (errno == EINTR)
            return -1;
        else if (errno == EINVAL)
            return -1;
        else {
            errno = EINVAL;
            return -1;
        }
    }
    return 0; /* timeout expires */
}

