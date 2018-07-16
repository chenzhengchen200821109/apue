#include "apue.h"
#include <errno.h>

static void pr_ids(char *name)
{
    printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n", name, (long)getpid(), (long)getppid(), (long)getpgrp(), (long)tcgetpgrp(STDIN_FILENO));
    fflush(stdout);
}

int main(void)
{
    char c;
    pid_t pid;
    int fd;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child */
        setsid(); /* new session */
        pr_ids("child");
        if ((fd = open("/dev/tty", O_RDONLY)) == -1) {
            if (errno == ENXIO)
                printf("%ld has no controlling TTY, errno = %d\n", (long)getpid(), errno);
        }
    } else {
        pr_ids("parent");
    }

    exit(0);
}
