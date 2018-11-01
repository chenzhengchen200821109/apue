#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    int fd0, fd1;
    pid_t pid;
    int ret;

    if (argc != 2) {
        printf("Usage: a.out filename\n");
        return 0;
    }

    /* get current process pid */
    pid = getpid();

    /*
     * call open().
     */
    if ((fd0 = open(argv[1], O_RDONLY)) < 0) {
        perror("open() first call fails");
        return 0;
    } else
        printf("open() call succeed once, fd = %d\n", fd0);
    
    /*
     * call open() again.
     */
    if ((fd1 = open(argv[1], O_RDONLY)) < 0) {
        perror("open() second call fails");
        return 0;
    } else
        printf("open() call succeed twice, fd = %d\n", fd1);
    
    ret = syscall(SYS_kcmp, pid, pid, fd0, fd1);
    assert(ret == 0);
    printf("open() call repeatly succeed\n");

    return 0;
}
