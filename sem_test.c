#include "apue.h"
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <limits.h>
#include <sys/mman.h>

#define NLOOPS 50

struct semlock
{
    long value;
    sem_t semvalue;
    char name[NAME_MAX - 4];
};

#define SEMSIZE (sizeof(struct semlock)) /* size of semaphore */

static int update(long *ptr)
{
    return ((*ptr)++); /* return value before increment */
}

int main(int argc, char *argv[])
{
    int fd, i, counter;
    pid_t pid;
    void *area;
    struct semlock *plock;
    sem_t *semp;
    static int cnt;

    if ((fd = open("/dev/zero", O_RDWR)) < 0)
        err_sys("open error");
    if ((area = mmap(0, SEMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
        err_sys("mmap error");
    close(fd); /* can close /dev/zero now since it's mapped */

    if ((plock = malloc(sizeof(struct semlock))) == NULL)
        err_quit("malloc error");
    do {
        snprintf(plock->name, sizeof(plock->name), "/%ld.%d", (long)getpid(), cnt++);
        plock->value = 0;
        semp = sem_open(plock->name, O_CREAT | O_EXCL, S_IRWXU, 1);
    } while ((semp == SEM_FAILED) && (errno == EEXIST));
    if (semp == SEM_FAILED) {
        free(plock);
        err_quit("sem_open oo error");
    }
    plock->semvalue = *semp;
    sem_unlink(plock->name);
    memcpy((struct semlock *)area, plock, sizeof(struct semlock));

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) { /* parent process goes first */
        for (i = 0; i < NLOOPS; ) {
            sem_wait(&(((struct semlock *)area)->semvalue));
            printf("parent[%d] = %ld\n", i, (((struct semlock *)area)->value));
            (((struct semlock *)area)->value)++;
            sem_post(&(((struct semlock *)area)->semvalue));
            sem_wait(&(((struct semlock *)area)->semvalue));
            i += 2;
            sem_post(&(((struct semlock *)area)->semvalue));
            //while ((((struct semlock *)area)->flag) == 1)
            //    ;
        }
    } else { /* child process */
        //sem_t *semp;

        //if ((semp = sem_open(((struct semlock *)area)->name, O_CREAT)) == SEM_FAILED)
        //    err_sys("sem_open error");
        //sem_wait(&(((struct semlock *)area)->semvalue));
        for (i = 1; i < NLOOPS + 1; ) {
            //sem_wait(semp);
            sem_wait(&(((struct semlock *)area)->semvalue));
            //if ((((struct semlock *)area)->value) != i)
            //    sem_wait(&(((struct semlock *)area)->semvalue));
            //else {
            printf("child[%d] = %ld\n", i, ((struct semlock *)area)->value);
            (((struct semlock *)area)->value)++;
            sem_post(&(((struct semlock *)area)->semvalue));
            sem_wait(&(((struct semlock *)area)->semvalue));
            i += 2;
            //sem_post(semp);
            sem_post(&(((struct semlock *)area)->semvalue));
            //}
        }
    }
        //semlock_destroy((struct semlock *)shared);
    exit(0);
}
