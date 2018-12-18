#include <stdio.h>
#include <sys/time.h>
#include <math.h>

void function()
{
    unsigned int i, j;
    double y;
    
    for (i = 0; i < 1000; i++)
        for (j = 0; j < 1000; j++)
            y = sin((double)i);
}

int main()
{
    struct timeval start, end;
    double timeuse;

    gettimeofday(&start, NULL);
    function();
    gettimeofday(&end, NULL);
    timeuse = 1000000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec);
    timeuse /= 1000000;
    printf("Used time: %f\n", timeuse);

    return 0;
}

