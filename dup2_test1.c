#include "apue.h"

int main(int argc, char *argv[]) {
    
    int fd = STDIN_FILENO;
    int fd1 = 1;

    printf("STDIN_FILENO = %d\n", STDIN_FILENO);
    printf("STDOUT_FILENO = %d\n", STDOUT_FILENO);
    printf("STDERR_FILENO = %d\n", STDERR_FILENO);

    //close(STDOUT_FILENO);
    if (dup2(fd, fd1) < 0)
        printf("dup2 error\n");
    else
        printf("dup2 success\n");
    printf("fd = %d, fd1 = %d\n", fd, fd1);
    write(fd1, "****", 4);

    exit(0);
}
