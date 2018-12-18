#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define rdtsc(low, high) __asm__ \
    __volatile__("rdtsc" : "=a"(low), "=d"(high))

unsigned long long get_cycles()
{
    unsigned low, high;
    unsigned long long val;
    rdtsc(low, high);
    val = high;
    val = (val << 32) | low;
    return val;
}

double get_cpu_mhz(void)
{
    FILE* file;
    char buf[256];
    double mhz = 0.0;
    char* pMHz;

    if ((file = fopen("/proc/cpuinfo", "r")) == NULL)
        return 0.0;
    while (fgets(buf, sizeof(buf), file)) {
        if (strstr(buf, "cpu MHz"))
            break;
    }
    
    pMHz = strstr(buf, ": ");
    mhz = strtof(pMHz+2, NULL);
    fclose(file);

    return mhz;
}

int main()
{
    double mhz;
    mhz = get_cpu_mhz();
    unsigned long c1, c2;

    for ( ; ; ) {
        c1 = get_cycles();
        sleep(1);
        c2 = get_cycles();
        /* 1s = 1000000usec */
        printf("1 sec = %lf usec\n", (c2-c1)/mhz);
    }

    return 0;
}
