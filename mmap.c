#include "apue.h"
#include <fcntl.h>
#include <sys/mman.h>

extern void TELL_WAIT(void);
extern void TELL_PARENT(pid_t pid);
extern void WAIT_PARENT(void);
extern void TELL_CHILD(pid_t pid);
extern void WAIT_CHILD(void);

#define NLOOPS 4
#define SIZE sizeof(long) /* size of shared memory area */

static int update(long *ptr)
{
    return ((*ptr)++); /* return value before increment */
}

int main(int argc, char *argv[])
{
    int fd, i, counter;
    pid_t pid;
    void *area;
    struct stat sb;

    //if ((fd = open("/dev/zero", O_RDWR)) < 0)
    //    err_sys("open error");
    if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
        err_sys("open error");
    ftruncate(fd, 8);
    if (fstat(fd, &sb) == -1)
        err_sys("fstat error");
    printf("length of %s is %d bytes\n", argv[1], (int)sb.st_size);
    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
        err_sys("mmap error");
    close(fd); /* can close /dev/zero now that it's mapped */

    *(long *)area = 0;
    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) { /* parent process goes first */
        for (i = 0; i < NLOOPS; i += 2) {
            if ((counter = update((long *)area)) != i)
                //err_quit("parent: expected %d, got %d", i, counter);
                printf("parent: expected %d, got %d\n", i, counter);
            printf("parent process: counter = %d\n", counter);
            TELL_CHILD(pid);
            WAIT_CHILD();
        }
    } else { /* child process */
        for (i = 1; i < NLOOPS + 1; i += 2) {
            WAIT_PARENT();
            if ((counter = update((long *)area)) != i)
                //err_quit("child: expected %d, got %d", i, counter);
                printf("child: expected %d, got %d\n", i, counter);
            printf("child process: counter = %d\n", counter);
            TELL_PARENT(getppid());
        }
    }
}
