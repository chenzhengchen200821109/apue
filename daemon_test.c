#include "apue.h"

extern void daemonize(const char *cmd);

int main(int argc, char *argv[])
{
    daemonize(argv[0]);
    while (1)
        ;

    return 0;
}
