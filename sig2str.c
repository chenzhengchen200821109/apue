#include <string.h>

int sig2str(int signo, char *str)
{
    size_t len;
    char *p;

    if (signo < 1 || signo > 31)
        return -1;
    p = strsignal(signo);
    len = strlen(p);
    strncpy(str, p, len);
    str[len] = '\0';
    return 0;
}
