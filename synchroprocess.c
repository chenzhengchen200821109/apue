#include "apue.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

/* process synchronization */
extern void TELL_WAIT(void);
extern void TELL_PARENT(pid_t pid);
extern void WAIT_PARENT(void);
extern void TELL_CHILD(pid_t pid);
extern void WAIT_CHILD(void);

#define NUM 15 /* number of numbers*/
#define CSIZE sizeof(int) /* size of shared memory area for Counter */
#define FLEN (sizeof(int) * NUM) /* length of file which is mapping */
#define NDIGITS 11 /* max length of a integer representing as a string, includig '\0' */

/* convert a integer into a string for writing to a file */
static void intToStr(int value, char *buf)
{
    char str[NDIGITS];
    int rem; /* remainder */
    int quot; /* quotient */
    int i, j;

    i = 0;
    j = 0;
    quot = value;
    if (quot == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    while (quot > 0) {
        rem = quot % 10;
        quot = quot / 10;
        str[i] = rem + 48;
        i++;
    }
    while (i > 0) {
        i--;
        buf[j] = str[i];
        j++;
    }
    buf[j] = '\0';
}

int main(int argc, char *argv[])
{
    pid_t pid;
    int fd;
    char buf[NDIGITS];
    size_t n;
    int status;
    int *area;
    char *warea; /* area for writing a value */

    if ((fd = open("/dev/zero", O_RDWR)) < 0)
        err_sys("open error");
    if ((area = (int *)mmap(0, CSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
        err_sys("mmap error");
    close(fd); /* can close /dev/zero now that it's mapped */

    *area = 0;

    /* fd shared by both parent and child process */
    if ((fd = open(argv[1], O_RDWR | O_CREAT | O_APPEND)) < 0)
        err_sys("open error");
    if ((warea = (char *)mmap(0, FLEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
        err_sys("open error");

    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) { /* child process and wait for parent process to start first */
        while (1) {
            WAIT_PARENT();
            if (*area < NUM) {
                printf("child process id = %ld, value = %d\n", (long)getpid(), *area);
                intToStr(*area, buf);
                printf("child process buf = %s\n", buf);
                write(fd, buf, strlen(buf));
                write(fd, "\n", 1);
                (*area)++;
                TELL_PARENT(getppid());
            } else {
                TELL_PARENT(getppid());
                _exit(0);
            }
        }
    } else {
    /* parent process */
        while (1) {
                if (*area < NUM) {
                printf("parent process id = %ld, value = %d\n", (long)getpid(), *area);
                intToStr(*area, buf);
                printf("parent process buf = %s\n", buf);
                write(fd, buf, strlen(buf));
                write(fd, "\n", 1);
                (*area)++;
                TELL_CHILD(pid);
                WAIT_CHILD();
            } else
                break;
        }
        if (*area == NUM)
            kill(pid, SIGTERM);
        waitpid(pid, &status, WNOHANG);
        close(fd);
        return 0;
    }
}
