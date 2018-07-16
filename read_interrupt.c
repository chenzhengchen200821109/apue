#include "apue.h"
#include <setjmp.h>
#include <unistd.h>
#include <errno.h>

static void sig_alrm(int);
static jmp_buf env_alrm;
ssize_t read_interrupt2(int fd, void *buf, size_t count, unsigned int seconds);

int main(void)
{
    int n;
    char line[MAXLINE];

    //if (signal(SIGALRM, sig_alrm) == SIG_ERR)
    //    err_sys("signal(SIGALRM) error");
    //if (setjmp(env_alrm) != 0)
    //    err_quit("read timeout");

    //alarm(10);
    if ((n = read_interrupt2(STDIN_FILENO, line, MAXLINE, 5)) < 0)
        err_sys("read error");
    //alarm(0);

    write(STDOUT_FILENO, line, n);

    exit(0);
}

static void sig_alrm(int signo)
{
    longjmp(env_alrm, 1);
}

ssize_t read_interrupt2(int fd, void *buf, size_t count, unsigned int seconds)
{
    ssize_t n;
    unsigned int sec;
    void (*handler)(int);

    if ((handler = signal(SIGALRM, sig_alrm)) == SIG_ERR)
        return -1;
    if (setjmp(env_alrm) == 0) {
        sec = alarm(seconds);
        if ((n = read(fd, buf, count)) < 0)
            return -1;
        alarm(sec); /* restore previous alarm */
        signal(SIGALRM, handler); /* restore previous SIGALRM handler */
        return n;
    }
    signal(SIGALRM, handler);
    alarm(sec);
    errno = EINTR;
    return -1;
}
