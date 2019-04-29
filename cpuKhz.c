/*
 * The following codes come from Tecent "libco" coroutine library.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static unsigned long long counter(void)
{
    register uint32_t lo, hi;
    register unsigned long long o;
    __asm__ __volatile__("rdtscp" : "=a"(lo), "=d"(hi)::"%rcx");
    o = hi;
    o <<= 32;
    printf("o = %llu\n", o);
    return (o | lo);
}

static unsigned long long getCpuKhz()
{
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) return 1;
    char buf[4096] = { 0 };
    fread(buf, 1, sizeof(buf), fp);
    fclose(fp);

    char *lp = strstr(buf, "cpu MHz");
    if (!lp) return 1;
    lp += strlen("cpu MHz");
    while (*lp == ' ' || *lp == '\t' || *lp == ':')
    {
        ++lp;
    }

    double mhz = atof(lp);
    unsigned long long u = (unsigned long long)(mhz * 1000);
    printf("u = %llu\n", u);
    return u;
}

static unsigned long long GetTickMS()
{
    uint32_t khz = getCpuKhz();
    return counter() / khz;
}

int main()
{
    unsigned long long res;

    res = GetTickMS();
    printf("res = %llu\n", res);

    return 0;
}
