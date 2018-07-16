#include "apue.h"
#include <dirent.h>

void print_closeOnExec(int fd);

int main(int argc, char *argv[])
{
    int i;
    char **ptr;
    extern char **environ;
    int fd;

    fd = open("/root", O_RDONLY);
    print_closeOnExec(fd);

    for (i = 0; i < argc; i++) /* echo all command-line args */
        printf("argv[%d]: %s\n", i, argv[i]);
    for (ptr = environ; *ptr != 0; ptr++) /* and all env strings */
        printf("%s\n", *ptr);

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
