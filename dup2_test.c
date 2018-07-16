#include "apue.h"
/*
 * dup2_test.c - test dup2 output on different cases.
 */

void dup2_test(int (*func)(int, int), char *output)
{
    /* test cases */
    int oldfds[] = {0, 1, 2, 3, 4, 1, 6};
    int newfds[] = {1, 2, -1, 4, 1024, 6, 1023};

    int count =sizeof(oldfds)/sizeof(oldfds[0]);
    char mesg[] = "I'm a message for dup2 test";
    int ret;
    int i;
    int fd;
    int newfd = 34;

    printf("starting test 1......\n");
    for (i = 0; i < count; i++) {
        ret = func(oldfds[i], newfds[i]);
        if (ret == -1)
            printf("failed: return %d, error is %s\n", ret, strerror(errno));
        else if (ret == newfds[i]) {
            printf("success: return %d\n", newfds[i]);
        } else 
            printf("unknown return value: %d, shouldn't not happen\n", ret);
    } 
    printf("test 1 is ending......\n");
    printf("\n");
    
    printf("starting test 2......\n");
    fd = open(output, O_RDONLY | O_WRONLY | O_CREAT);
    if (fd != -1) {
        ret = func(fd, newfd);
        if (ret == -1)
            printf("failed: return %d, error is %s\n", ret, strerror(errno));
        else if (ret == newfd) {
            printf("success\n");
            write(fd, mesg, sizeof(mesg)+1);
            close(ret);
        } else
            printf("unknown return value: %d, shouldn't not happen\n", ret);

        close(fd);
    }
    printf("test 2 ending......\n");


    return;
}
