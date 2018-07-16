#include "apue.h"
#include <pthread.h>

//int quitflag; /* set nonzero by thread */
sigset_t mask;

//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;


void *thr_fn2(void *arg)
{
    printf("thr_fn2 is invoking...\n");
}


void *thr_fn1(void *arg)
{
    int err, signo;

    for(;;) {
        err = sigwait(&mask, &signo);
        if (err != 0)
            err_exit(err, "sigwait failed");
        switch(signo) {
            case SIGINT:
                printf("\ninterrupt\n");
                break;
                //exit(0);
            case SIGALRM:
                printf("\nalarm\n");
                break;
            case SIGQUIT:
                //pthread_mutex_lock(&lock);
                //quitflag = 1;
                //pthread_mutex_unlock(&lock);
                //pthread_cond_signal(&waitloc);
                return(0);
            default:
                printf("unexpected signal %d\n", signo);
                exit(1);
        }
    }
}

int main(void)
{
    int err;
    sigset_t oldmask;
    pthread_t tid1, tid2;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGALRM);
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0)
        err_exit(err, "SIG_BLOCK error");

    alarm(10);
    err = pthread_create(&tid1, NULL, thr_fn1, 0);
    if (err != 0)
        err_exit(err, "Can't create thread");
    err = pthread_create(&tid2, NULL, thr_fn2, 0);
    if (err != 0)
        err_exit(err, "Can't create thread");
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    //pthread_mutex_lock(&lock);
    //while (quitflag == 0)
    //    pthread_cond_wait(&waitloc, &lock);
    //pthread_mutex_unlock(&lock);

    /* SIGQUIT has been caught and is now blocked; do whatever */
    //quitflag = 0;

    /* reset signal mask which unblocks SIGQUIT */
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");

    exit(0);
}

