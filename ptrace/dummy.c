#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <stdlib.h>

int main()
{
    int i = 0;
    /*
     * make any process can be tracer of this program.
     */
    if (prctl(PR_SET_PTRACER, PR_SET_PTRACER_ANY) < 0) {
        perror("prctl");
        exit(-1);
    }
    while (1) {
        printf("My counter: %d\n", i++);
        sleep(2);
    }

    return 0;
}
