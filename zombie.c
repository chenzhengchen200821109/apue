#include "apue.h"

int main()
{
    pid_t pid;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) { /* child */
        printf("child process id = %ld\n", (long)getpid());
        _exit(0);
    }
    else {
        sleep(10);
    }

    _exit(0);
}
