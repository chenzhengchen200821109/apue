#include <stdio.h>
#include <sys/prctl.h>
#include <stdlib.h>

void print_string(const char* str)
{
    printf("%s\n", str);
}

int main()
{
    char str0[] = "Hello 1";
    char str1[] = "Hello 2";

    /*
     * make any process can be tracer of this program.
     */
    if (prctl(PR_SET_PTRACER, PR_SET_PTRACER_ANY) < 0) {
        perror("prctl");
        exit(-1);
    }

    while (1) {
        print_string(str0);
        print_string(str1);
    }

    return 0;
}
