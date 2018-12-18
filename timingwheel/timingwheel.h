#ifndef TIMINGWHEEL_H
#define TIMINGWHEEL_H

#include <unistd.h>
#include <sys/types.h>

extern int Timingwheel_init(int slot, int interval, pid_t* thread);

extern struct Timer* Timingwheel_addTimer(int timeout, pid_t* thread);

extern void Timingwheel_deleteTimer(struct Timer* timer, pid_t* thread);

extern void Timingwheel_destroy(pid_t* thread);

extern void tick(pid_t* thread);

#endif
