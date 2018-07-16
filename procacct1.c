#include "apue.h"

int main()
{
    pid_t pid;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) {
        /* we are in parent process now */
        sleep(2);
        exit(2); /* terminate with exit status 2 */
    }

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) {
        /* we are in first child process now */
        sleep(4);
        abort(); /* terminate with core dump */
    }

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) {
        /* we are in second child process now */
        execl("/bin/dd", "dd", "if=/ect/passwd", "of=/dev/null", NULL);
        exit(7); /* shouldn't get here */
    }

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid != 0) {
        /* we are in third child process now */
        sleep(8);
        exit(0); /* normal exit */
    }

    sleep(6); /* fourth child process */
    kill(getpid(), SIGKILL); /* terminate w/signal, no core dump */
    exit(6); /* shouldn't get here */
}
