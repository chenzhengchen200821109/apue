#include "apue.h"

static void charatatime(char *);

int main(void)
{
    pid_t pid;

    //TELL_WAIT();

    if ((pid = fork()) < 0) 
        err_sys("fork error");
    else if (pid == 0) {
        //WAIT_PARENT(); /* parent goes first */
        charatatime("output from child: this is a long long long long long long long long long long long long long line\n");
        //TELL_PARENT(getppid());
    } else {
        //WAIT_CHILD(); /* child goes first */
        charatatime("output from parent: this is a long long long long long long long long long long long long long long line\n");
        //TELL_CHILD(pid);
    }
    exit(0);
}

static void charatatime(char *str)
{
    char *ptr;
    int c;

    setbuf(stdout, NULL); /* set unbuffered */
    for (ptr = str; (c = *ptr++) != 0; )
        putc(c, stdout);
}
