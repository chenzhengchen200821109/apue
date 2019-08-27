/*
 * 将向此程序注入一段代码。
 */ 
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <stdlib.h>

int main()
{
    /*
     * make any process can be tracer of this program.
     */
    if (prctl(PR_SET_PTRACER, PR_SET_PTRACER_ANY) < 0) {
        perror("prctl");
        exit(-1);
    }
    long a;
    while (1) {
        //printf("I am working\n");
        //sleep(5);
        a++;
    }
    return 0;
}
