#include "timingwheel.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <syscall.h>

/**
 *
 */
struct Timer
{
    int rotation;
    int slot;
    void (*timer_callback)(void);
    struct Timer* prev;
    struct Timer* next;
};

struct TimingWheelSlot
{
    struct Timer** head;
    int interval;
    int nslots; /* number of slots in the timingwheel */
    int cur_slot; /* current slot where we are */
};

static struct TimingWheelSlot Head;

//void timer_thread(union sigval sig)
//{
//    pid_t tid = (pid_t)syscall(SYS_gettid);
//    printf("Timer in PID:[%d]TID:[%d]\n", getpid(), tid);
//}

int Timingwheel_init(int slot, int interval, pid_t* thread)
{
    void* p;
    size_t len;

    len = sizeof(struct Timer **) * slot;
    if ((p = malloc(len)) == NULL) {
        perror("malloc failed");
        return -1;
    }

    Head.head = (struct Timer **)p;
    Head.interval = interval;
    Head.nslots = slot;
    Head.cur_slot = 0;

    for (int i = 0; i < slot; i++) {
        Head.head[i] = NULL;
    }

    //timer_t timer_id;
    //struct sigevent se;
    //struct itimerspec ts;
    //int status;

    //se.sigev_notify = SIGEV_THREAD;
    //se.sigev_value.sival_ptr = &timer_id;
    //se.sigev_notify_function = &timer_thread;
    //se.sigev_notify_attributes = NULL;
    //if ((status = timer_create(CLOCK_REALTIME, &se, &timer_id)) == -1) {
    //    perror("timer_create error");
    //    return -1;
    //}

    //ts.it_value.tv_sec = 5;
    //ts.it_value.tv_nsec = 0;
    //ts.it_interval.tv_sec = 5;
    //ts.it_interval.tv_nsec = 0;
    //status = timer_settime(timer_id, 0, &ts, 0);

    return 0;
}

static void Timingwheel_handleTimer(struct Timer* timer)
{
    if (timer && timer->timer_callback)
        timer->timer_callback();
}

struct Timer* Timingwheel_addTimer(int timeout, pid_t* thread)
{
    int ticks = 0;
    int interval = Head.interval;
    int nslots = Head.nslots;
    int cur_slot = Head.cur_slot;
    struct Timer* timer;
    struct Timer* cur;

    if (timeout < 0)
        return NULL;
    if (timeout < interval) { /* si: slot interval */
        ticks = 1;
    } else {
        ticks = timeout / interval;
    }
    int rotation = ticks / nslots;
    int ts = (cur_slot + (ticks % nslots)) % nslots;
    if ((timer = (struct Timer *)malloc(sizeof(struct Timer))) == NULL)
        return NULL;
    timer->slot = ts;
    timer->rotation = rotation;
    timer->next = timer->prev = NULL;

    cur = Head.head[ts];
    if (!cur) {
        printf("add a timer that rotation is %d, ts is %d, cur_slot is %d\n",
            rotation, ts, cur_slot);
        Head.head[ts] = timer;
    } else {
        timer->next = cur;
        cur->prev = timer;
        Head.head[ts] = timer;
    }
    
    return timer;
}

/**
 * assume that timer will be deleted must be valid.
 */
void Timingwheel_deleteTimer(struct Timer* timer, pid_t* thread)
{
    if (!timer)
        return;

    int slot = timer->slot;
    struct Timer* head = Head.head[slot];

    if (timer == head) {
        head = head->next;
        if (head)
            head->prev = NULL;
        Head.head[slot] = head;
        free(timer);
    } else {
        timer->prev->next = timer->next;
        if (timer->next)
            timer->next->prev = timer->prev;
        free(timer);
    }
}

/**
 *
 */
void tick(pid_t* thread)
{
    int cur_slot = Head.cur_slot;
    struct Timer* cur = Head.head[cur_slot];

    while (cur) {
        if (cur->rotation > 0) {
            cur->rotation--;
            cur = cur->next;
        } else {
            struct Timer* next = cur->next;
            cur = next;
            Timingwheel_handleTimer(cur);
            Timingwheel_deleteTimer(cur, thread);
        }
    }
    cur_slot++;
    Head.cur_slot = cur_slot;
}

void Timingwheel_destroy(pid_t* thread)
{
    int nslots = Head.nslots;
    for (int i = 0; i < nslots; i++) {
        struct Timer* timer = Head.head[i];
        while (timer) {
            struct Timer* tmp = timer;
            timer = timer->next;
            free(tmp);
        }
    }
    free((void *)Head.head);
    Head.head = NULL;
}

