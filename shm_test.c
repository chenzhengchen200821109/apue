#include "apue.h"
#include <fcntl.h>
#include <sys/shm.h>

extern void TELL_WAIT(void);
extern void TELL_PARENT(pid_t pid);
extern void WAIT_PARENT(void);
extern void TELL_CHILD(pid_t pid);
extern void WAIT_CHILD(void);

#define NLOOPS 1000
#define SHM_SIZE sizeof(long) /* size of shared memory area */
#define SHM_MODE 0600 /* user read/write */

static int update(long *ptr)
{
    return ((*ptr)++); /* return value before increment */
}

int main(int argc, char *argv[])
{
    int fd, i, counter;
    pid_t pid;
    int shmId;

    if ((shmId = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) == -1)
        err_sys("shmget error");
    
    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) { /* parent process goes first */
        for (i = 0; i < NLOOPS; i += 2) {
            long *addr1;
            if ((addr1 = shmat(shmId, 0, 0)) == (void *)-1)
                err_sys("shmat error");
            if ((counter = update(addr1)) != i)
                err_quit("parent: expected %d, got %d", i, counter);
            printf("LOOP[%d], i = %d, counter = %d\n", i, i, counter);
            TELL_CHILD(pid);
            WAIT_CHILD();
        }
    } else { /* child process */
        for (i = 1; i < NLOOPS + 1; i += 2) {
            long *addr2;
            WAIT_PARENT();
            if ((addr2 = shmat(shmId, 0, 0)) == (void *)-1)
                err_sys("shmat error");
            if ((counter = update(addr2)) != i)
                err_quit("child: expected %d, got %d", i, counter);
            printf("LOOP[%d], i = %d, counter = %d\n", i, i, counter);
            TELL_PARENT(getppid());
        }
    }
    exit(0);
}
