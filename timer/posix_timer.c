#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <syscall.h>
#include <unistd.h>
#include <stdlib.h>

static int counter = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;


void timer_thread(union sigval sig)
{
    int status;

    status = pthread_mutex_lock(&mutex);
    if (++counter >= 5) 
        status = pthread_cond_signal(&cond);
    status = pthread_mutex_unlock(&mutex);
    pid_t tid = (pid_t)syscall(SYS_gettid);
    printf("Timer %d in PID:[%d]TID:[%d]\n", counter, getpid(), tid);
}

int main()
{
    timer_t timer_id;
    struct sigevent se;
    struct itimerspec ts;
    int status;

    se.sigev_notify = SIGEV_THREAD;
    se.sigev_value.sival_ptr = &timer_id;
    se.sigev_notify_function = &timer_thread;
    se.sigev_notify_attributes = NULL;
    status = timer_create(CLOCK_REALTIME, &se, &timer_id);

    ts.it_value.tv_sec = 5;
    ts.it_value.tv_nsec = 0;
    ts.it_interval.tv_sec = 5;
    ts.it_interval.tv_nsec = 0;
    status = timer_settime(timer_id, 0, &ts, 0);
    
    while (counter < 5) {
        status = pthread_cond_wait(&cond, &mutex);
    }

    exit(0);
}
