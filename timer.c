/*
 * Implementation of a queued-timer, which should be used in a sigle process.
 * Basic timer model:
 *     start_timer(interval, timerId, expiryAction);
 *     stop_timer(timerId);
 *     per_tick_book_keeping();
 *     expiry_process();
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "timer.h"

#define TIMER_NUM_MAX 16

struct timer_list timer_list;

// Tick Bookkeeping.
static void sig_alrm(int signo)
{
    struct timer *node = timer_list.header;

    for ( ; node != NULL; node = node->next) {
        node->elapse++;
        if (node->elapse >= node->interval) {
            node->elapse = 0; /* repeating timer */
            node->cb(node->id, node->user_data, node->len);
        }
    }
}

// create a timer list.
int
timer_init(int count)
{
    int ret = 0;

    if (count <= 0 || count > TIMER_NUM_MAX) {
        printf("the timer max number MUST less than %d.\n", TIMER_NUM_MAX);
        return -1;
    }

    memset(&timer_list, 0, sizeof(struct timer_list));
    timer_list.header = NULL;
    timer_list.max_num = count;

    // Register our internal signal handler and store signal handler.
    if ((timer_list.sigfunc_old = signal(SIGALRM, sig_alrm)) == SIG_ERR)
        return -1;
    timer_list.sigfunc_new = sig_alrm;

    // Setting our interval timer for driver our multi-timer and store
    // old timer value.
    timer_list.nvalue.it_value.tv_sec = 1;
    timer_list.nvalue.it_value.tv_usec = 0;
    timer_list.nvalue.it_interval.tv_sec = 1;
    timer_list.nvalue.it_interval.tv_usec = 0;
    ret = setitimer(ITIMER_REAL, &timer_list.nvalue, &timer_list.ovalue);

    return ret;
}

// Delete all timers of the timer list no matter they are expired or not.
int
timer_destroy(void) {
    struct timer *node = NULL;
    
    // Restore previous SIGALRM handler
    if ((signal(SIGALRM, timer_list.sigfunc_old)) == SIG_ERR)
        return -1;
    // Restore previous timer
    if (setitimer(ITIMER_REAL, &timer_list.ovalue, NULL) < 0)
        return -1;
    // Delete all
    while (timer_list.header != NULL) {
        node = timer_list.header;
        timer_list.header = node->next;
        free(node->user_data);
        free(node);
    }
    memset(&timer_list, 0, sizeof(struct timer_list));

    return 0;
}

// Add a timer to the timer list
timer_id
timer_add(int interval, timer_expiry *cb, void* user_data, int len) {
    struct timer *node = NULL;

    if (cb == NULL || interval <= 0)
        return -1;
    if (timer_list.num < timer_list.max_num)
        timer_list.num++;
    else
        return -1;
    if ((node = malloc(sizeof(struct timer))) == NULL)
        return -1;
    if (user_data != NULL && len != 0) {
        node->user_data = malloc(len);
        memcpy(node->user_data, user_data, len);
        node->len = len;
    }
    node->cb = cb;
    node->interval = interval;
    node->elapse = 0;
    node->id = timer_list.num;

    // Insert a timer node in front of the timer list
    node->next = timer_list.header;
    timer_list.header = node;

    return node->id;
}
