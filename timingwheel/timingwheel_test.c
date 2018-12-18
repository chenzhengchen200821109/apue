#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include "timingwheel.h"

void print_info(int signo)
{
    printf("timer fired\n");
    tick(NULL);
}

void init_sigaction(void)
{
    struct sigaction act;
    act.sa_handler = print_info;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGPROF, &act, NULL);
}

void init_time()
{
    struct itimerval value;
    value.it_value.tv_sec = 2;
    value.it_value.tv_usec = 0;
    value.it_interval = value.it_value;
    setitimer(ITIMER_PROF, &value, NULL);
}

int main()
{
    init_sigaction();
    init_time();
    Timingwheel_init(6, 2, NULL);
    Timingwheel_addTimer(3, NULL);
    Timingwheel_addTimer(5, NULL);
    Timingwheel_addTimer(8, NULL);
    //Timingwheel_destroy(NULL);
    while (1)
        ;

    exit(0);
}
