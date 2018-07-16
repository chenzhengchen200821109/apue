#include "apue.h"
#include <sys/wait.h>
#include <dirent.h>

void print_closeOnExec(int fd);

char *env_init[] = { "USER=unknown", "PATH=/tmp", NULL };

int main(void)
{
    pid_t pid;
    int fd, flag;
    DIR *dirp;

    if ((dirp = opendir("/root")) == NULL)
        err_sys("opendir error");
    fd = dirfd(dirp);
    print_closeOnExec(fd);
    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) { /* specify pathname, specify environment */
        if (execle("/home/chenzheng/Documents/github-apue/echoall", "echoall", "myarg1", "MY ARG2", (char *)0, env_init) < 0)
            err_sys("execle error");
    }
    if (waitpid(pid, NULL, 0) < 0)
        err_sys("wait error");
    print_closeOnExec(fd);
    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) { /* specify filename, inherit environment */
        if (execlp("echoall", "echoall", "only 1 arg", (char *)0) < 0)
            err_sys("execlp error");
    }

    exit(0);
}

void print_closeOnExec(int fd)
{
    int flag;

    if ((flag = fcntl(fd, F_GETFD)) == 1)
        printf("close_on_exec flag is on\n");
    else if (flag == 0)
        printf("close_on_exec flag is down\n");
    else
        printf("close_on_exec flag unknown\n");

}
