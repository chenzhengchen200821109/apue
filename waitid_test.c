#define _XOPEN_SOURCE 700

#include "apue.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int myWaitid(siginfo_t *infop);
void print_siginfo(siginfo_t *infop);

int main(void)
{
    pid_t pid;
    siginfo_t siginfo;
    
    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) /* child */
        _exit(2);
    if (myWaitid(&siginfo) == -1)
        err_sys("myWaitid error");
    print_siginfo(&siginfo);

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) /* child */
        abort();
    if (myWaitid(&siginfo) == -1)
        err_sys("myWaitid error");
    print_siginfo(&siginfo);

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) { /* child */
        int a = 7;
        int b = 0;
        a /= b;
    }
    if (myWaitid(&siginfo) == -1)
        err_sys("myWaitid error");
    print_siginfo(&siginfo);

    exit(0);
}

int myWaitid(siginfo_t *infop)
{
    return waitid(P_ALL, 9999, infop, WEXITED | WSTOPPED | WCONTINUED);
}

void print_siginfo(siginfo_t *infop)
{
    int code;
    int status;

    code = infop->si_code;
    status = infop->si_status;
    /* child process terminated normally */
    if (code == CLD_EXITED) 
        printf("normal termination, exit status = %d, code = %s\n", status, "CLD_EXITED");
    /* child process terminated abnormally */
    else if (code == CLD_KILLED || code == CLD_DUMPED)
        printf("abnormal termination, signal number = %d, code = %s\n", status, "CLD_KILLED or CLD_DUMPED");
    /* child process stopped */
    else if (code == CLD_STOPPED)
        printf("child stopped, signal number = %d, code = %s\n", status, "CLD_STOPPED");
    /* child process continued */
    else if (code == CLD_CONTINUED)
        printf("child continued, signal number = %d, code = %s\n", status, "CLD_CONTINUED");
}
