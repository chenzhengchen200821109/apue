#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int globvar = 6;

int func(void)
{
    int var;
    pid_t pid;

    var = 88;
    printf("before vfork\n");
    if ((pid = vfork()) < 0) {
        printf("vfork error\n");
        exit(0);
    } else if (pid == 0) { /* child process */
        globvar++;
        var++;
        _exit(0);
    }
    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
    return (0);
}

int main()
{
    printf("before func()\n");
    func();
    printf("after func()\n");

    return 0;
}


