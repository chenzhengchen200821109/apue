#include "apue.h"

int main(int argc, char *argv[])
{
    int i;
    struct stat statbuf;

    for (i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);
        if (stat(argv[i], &statbuf) < 0) {
            err_ret("stat error");
            continue;
        }
        printf("dev = %d/%d", major(statbuf.st_dev), minor(statbuf.st_dev));
        /* check whether a special device */
        if (S_ISCHR(statbuf.st_mode) || S_ISBLK(statbuf.st_mode)) {
            printf(" (%s) rdev = %d/%d", (S_ISCHR(statbuf.st_mode)) ? "character" : "block", major(statbuf.st_rdev), minor(statbuf.st_rdev));
        }
        printf("\n");
    }

    exit(0);
}
