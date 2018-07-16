#include "apue.h"
#include <sys/wait.h>

int main(void)
{
    int status;

    if ((status = system_v1("date")) < 0)
        err_sys("system_v1() error");
    pr_exit(status);

    if ((status = system_v1("nosuchcommand")) < 0)
        err_sys("system_v1() error");
    pr_exit(status);

    if ((status = system_v1("who; exit 44")) < 0)
        err_sys("system_v1() error");
    pr_exit(status);

    exit(0);
}
