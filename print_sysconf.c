#include "apue.h"
#include <errno.h>
#include <limits.h>

static void pr_sysconf(char *, int);
static void pr_pathconf(char *, char *, int);

int main(int argc, char *argv[])
{
    int notrunc = 0;

    if (argc != 2)
        err_quit("usage: a.out <dirname>");

#ifdef _POSIX_NO_TRUNC
    notrunc = _POSIX_NO_TRUNC;
#else
    printf("Option _POSIX_NO_TRUNC is not supported\n");
#endif

    if (notrunc == -1)
        printf("Option _POSIX_NO_TRUNC is not supported\n");
    else if (notrunc == 0)
        pr_pathconf("At running time", argv[1], _PC_NO_TRUNC);
    else
        printf("Option _POSIX_NO_TRUNC is supported, _POSIX_NO_TRUNC = %d\n", _POSIX_NO_TRUNC);

#ifdef _POSIX_ARG_MAX
    printf("_POSIX_ARG_MAX defined to be %ld in <unistd.h>\n", (long)_POSIX_ARG_MAX + 0);
#else
    printf("no symbol defined in <unistd.h> for _POSIX_ARG_MAX\n");
#endif
#ifdef ARG_MAX
    printf("ARG_MAX defined to be %ld in <limits.h>\n", (long)ARG_MAX + 0);
#else
    printf("no symbol defined in <limits.h> for ARG_MAX\n");
#endif
#ifdef _SC_ARG_MAX
    pr_sysconf("At running time, ARG_MAX = ", _SC_ARG_MAX);
#else
    printf("no symbol for _SC_ARG_MAX\n");
#endif

#ifdef _POSIX_PATH_MAX
    printf("_POSIX_PATH_MAX defined to be %ld in <unistd.h>\n", (long)_POSIX_PATH_MAX + 0);
#else
    printf("no symbol defined in <unistd.h> for _POSIX_PATH_MAX\n");
#endif
#ifdef PATH_MAX
    printf("PATH_MAX defined to be %ld in <limits.h>\n", (long)PATH_MAX + 0);
#else
    printf("no symbol defined in <limits.h> for PATH_MAX\n");
#endif
#ifdef _PC_PATH_MAX
    pr_pathconf("At running time, PATH_MAX = ", argv[1], _PC_PATH_MAX);
#else
    printf("no symbol for _PC_PATH_MAX\n");
#endif

    /* similar processing for all the rest of the sysconf symbols... */
    
    exit(0);
}

static void pr_sysconf(char *mesg, int name)
{
    long val;

    fputs(mesg, stdout);
    errno = 0;
    if ((val = sysconf(name)) < 0) {
        if (errno != 0) {
            if (errno == EINVAL)
                fputs(" (not supported)\n", stdout);
            else
                err_sys("sysconf error");
        }
    } else {
        fputs(" (no limit)\n", stdout);
    }
}

static void pr_pathconf(char *mesg, char *path, int name)
{
    long val;

    fputs(mesg, stdout);
    errno = 0;
    if ((val = pathconf(path, name)) < 0) {
        if (errno != 0) {
            if (errno == EINVAL)
                fputs(" (not supported)\n", stdout);
            else
                err_sys("pathconf error, path = %s", path);
        } else {
            fputs(" (no limit)\n", stdout);
        }
    } else {
        printf(" %ld\n", val);
    }
}


