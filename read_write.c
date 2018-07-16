#include "apue.h"
#include <sys/resource.h>

#define BUFFSIZE 100

static void sig_intr(int signo); /* one handler for SIGXFSZ signal */

int main(int argc, char *argv[])
{
    int n;
    ssize_t rev;
    char buf[BUFFSIZE];
    struct rlimit limit;
    struct stat sb;
    int fdin, fdout;

    if (getrlimit(RLIMIT_FSIZE, &limit) == -1)
        err_sys("getrlimit error");
    limit.rlim_cur = 1024;
    if (setrlimit(RLIMIT_FSIZE, &limit) == -1)
        err_sys("setrlimit error");
    if (getrlimit(RLIMIT_FSIZE, &limit) == -1)
        err_sys("getrlimit error");
    printf("rlim_cur = %10lld\n", (unsigned long long)limit.rlim_cur);
    printf("rlim_max = %10lld\n", (unsigned long long)limit.rlim_max);

    if ((fdin = open(argv[1], O_RDONLY, FILE_MODE)) == -1)
        err_sys("open error");
    if ((fdout = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, FILE_MODE)) == -1)
        err_sys("open error");
    fstat(fdin, &sb);
    printf("input file has %ld bytes\n", (long)sb.st_size);

    if (signal(SIGXFSZ, sig_intr) == SIG_ERR)
        err_sys("can't catch SIGXFSZ");
    while ((n = read(fdin, buf, BUFFSIZE)) > 0) {
        if (( rev = write(fdout, buf, n)) != n) 
            printf("expected %d bytes, got %d bytes\n", n, rev);
        else
            printf("expected %d bytes, got %d bytes\n", n, rev);
    }
    if (n < 0) {
        err_sys("read error");
    }
    fstat(fdout, &sb);
    printf("output file has %ld bytes\n", (long)sb.st_size);

    close(fdin);
    close(fdout);
    exit(0);
}

void sig_intr(int signo)
{
    printf("received SIGXFSZ\n");
}
