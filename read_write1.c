#include "apue.h"

#define BUFFSIZE 16

int main(void)
{
    int n1, n2;
    char buf[BUFFSIZE];

    while ((n1 = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
        if ((n2 = write(STDOUT_FILENO, buf, n1)) != n1)
            err_sys("write error");
        printf("read from stdin: %d bytes, wrote to stdout %d\n", n1, n2);
    }
    if (n1 < 0)
        err_sys("read error");
    exit(0);
}
