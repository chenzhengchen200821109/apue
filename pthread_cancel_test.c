#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

static void *thread_func(void *ignored_argument)
{
    int s;

    /*
     * Disable cancellation for a while, so that we don't immediately react to
     * a cancellation request.
     */
    if ((s = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL)) != 0)
        handle_error_en(s, "pthread_setcancelstate");
    printf("thread_func(): started; cancellation disabled\n");
    sleep(5);
    printf("thread_func(): about to enable cancellation\n");
    if ((s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) != 0)
        handle_error_en(s, "pthread_setcancelstate");
    /* sleep() is a cancellation point */
    sleep(1000); /* should get canceled while we sleep */
    /* should never get here */
    printf("thread_func(): not canceled!\n");
    return (NULL);
}

int main(void)
{
    pthread_t thr;
    void *res;
    int s;

    /* start a thread and then send it a cancellation request */
    if ((s = pthread_create(&thr, NULL, &thread_func, NULL)) != 0)
        handle_error_en(s, "pthread_create");
    sleep(2); /* give thread a chance to get started */
    printf("main(): sending cancellation request\n");
    if ((s = pthread_cancel(thr)) != 0)
        handle_error_en(s, "pthread_cancel");
    /* join with thread to see what its exit status was */
    if ((s = pthread_join(thr, &res)) != 0)
        handle_error_en(s, "pthread_join");
    if (res == PTHREAD_CANCELED)
        printf("main(): thread was canceled\n");
    else
        printf("main(): thread wasn't canceled (shouldn't happen!)\n");
    exit(EXIT_SUCCESS);
}
