#include <stdio.h>
#include <string.h>

#define NSIGS 31
#define SIG2STR_MAX 32

extern int sig2str(int, char *);

int main()
{
    int i;
    char str[SIG2STR_MAX];

    for (i = 1; i <= NSIGS; i++) {
        sig2str(i, str);
        printf("signal number %d: signal string is %s\n", i, str);
    }
    return 0;
}
