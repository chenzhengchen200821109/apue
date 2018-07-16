#include <stdio.h>
#include "timer.h"

int callback(timer_id id, void* user_data, int len)
{
    printf("id[%d] timer expires\n", id);
    return 0;
}

int main()
{
    timer_init(5);
    timer_add(5, &callback, NULL, 0);
    timer_add(8, &callback, NULL, 0);
    //timer_destroy();

    while (1)
        ;
    return 0;
}
