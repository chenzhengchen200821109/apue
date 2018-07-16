#include "apue.h"

void
reboot_process(void) {
    char* argv[] = { NULL };
    char* environ[] = { NULL };
    if (execve("./atexit_test", argv, environ) == -1) {
        perror("execve");
        _exit(0);
    }
}

void
sig_term(int signo)
{
    printf("caught SIGTERM\n");
    exit(0);
}

int main(void)
{
    int i;
    int flag;

    if ((i = atexit(reboot_process)) != 0) {
        fprintf(stderr, "cannot set exit function\n");
        _exit(1);
    }

    signal(SIGTERM, sig_term);
    flag = 1;
    while (flag) {
        printf("I'm working\n");
        sleep(5);
    }
    exit(0);
}

