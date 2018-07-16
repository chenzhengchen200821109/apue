#include "apue.h"
#include <pthread.h>

void *thr_fn1(void *arg)
{
    int a = 0;

    a++;
    printf("thread 1 returning\n");
    printf("thread 1: %d\n", a);
    /* return value */
    return((void *)1);
}

void *thr_fn2(void *arg)
{
    int a = 0;

    a++;
    a++;
    printf("thread 2 returning\n");
    printf("thread 2: %d\n", a);
    /* return value */
    return((void *)2);
}

int main(void)
{
    int err;
    pthread_t tid1, tid2;
    void *tret;
    int a;

    a = 1;
    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0)
        err_exit(err, "can't create thread 1");
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0)
        err_exit(err, "can't create thread 2");
    err = pthread_join(tid1, &tret);
    if (err != 0)
        err_exit(err, "can't join with thread 1");
    printf("thread 1 exit code %ld\n", (long)tret);
    err = pthread_join(tid2, &tret);
    if (err != 0)
        err_exit(err, "can't join with thread 2");
    printf("thread 2 exit code %ld\n", (long)tret);
    printf("main thread: %d\n", a);
    
    exit(0);
}
