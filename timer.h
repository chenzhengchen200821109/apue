#include <sys/time.h>

typedef int timer_id;
typedef int timer_expiry(timer_id, void* user_data, int len);

struct timer {
    struct timer* next;
    timer_id id;
    int count; /* count on each timer reset */
    int interval; /* timer interval(second) */
    int elapse;
    timer_expiry* cb; /* call back */
    void* user_data;
    int len;
};

struct timer_list {
    struct timer* header;
    int num;
    int max_num;
    void (*sigfunc_old)(int); /* save previous signal handler */
    void (*sigfunc_new)(int); /* our new signal handler */
    struct itimerval ovalue;
    struct itimerval nvalue;
};

int timer_init(int count);
int timer_destroy(void);
timer_id timer_add(int interval, timer_expiry *cb, void* user_data, int len);

