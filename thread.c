#include "apue.h"
#include <pthread.h>

/*
 * Create a detached thread.
 */

int make_detach_thread(void *(*fn)(void *), void *arg)
{
    int err;
    pthread_t tid;
    pthread_attr_t attr;

    /* initialize pthread attributes */
    err = pthread_attr_init(&attr);
    /* does not work right, return error value */
    if (err != 0)
        return err;
    /* set detach attribute */
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err == 0) 
        err = pthread_create(&tid, &attr, fn, arg);
    /* when a thread has been created, destroy the thread attributes */
    pthread_attr_destroy(&attr);
    return err;
}
