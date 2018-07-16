#include "apue.h"
#include <sys/utsname.h>

int main(void)
{
    struct utsname name;

    if (uname(&name) == -1)
        err_sys("uname error");

    printf("name of operating system is %s\n", name.sysname);
    printf("name of this node is %s\n", name.nodename);
    printf("current release of operating system is %s\n", name.release);
    printf("current version of this release is %s\n", name.version);
    printf("name of hardware type is %s\n", name.machine);

    exit(0);
}
