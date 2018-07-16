#include "apue.h"
#include <pthread.h>

pthread_t ntid;

extern char *getenv_v(const char *name);
extern int putenv_v(char *string);

void printids(const char *s)
{
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);
}

void *thr_fn1(void *arg) {
    putenv_v("XXX=YYY");
    printids("new thread1: ");
    return ((void *)0);
}

void *thr_fn2(void *arg) {
    putenv_v("XXX=OOO");
    printids("new thread2: ");
    return ((void *)0);
}

int main(void) {
    int err;
    char *value;

    err = pthread_create(&ntid, NULL, thr_fn1, NULL);
    if (err != 0)
        err_exit(err, "can't create thread");
    err = pthread_create(&ntid, NULL, thr_fn2, NULL);
    if (err != 0)
        err_exit(err, "can't create thread");
    sleep(10);
    //putenv_v("XXX=ZZZ");
    printids("main thread: ");
    value = getenv_v("XXX");
    printf("XXX = %s\n", value);
    //sleep(1);
    exit(0);
}
