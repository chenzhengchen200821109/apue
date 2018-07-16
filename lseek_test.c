#include "apue.h"

int main(int argc, char *argv[])
{
    int fd;
    off_t currpos;

    char buf1[] = "abcdefghij";
    char buf2[] = "ABCDEFGHIJ";
    char buf[10];

    if ((fd = open(argv[1], O_RDWR | O_TRUNC | O_APPEND)) < 0)
        err_sys("open error");
    /* offset = 0 */

    if (write(fd, buf1, 10) != 10)
        err_sys("first write error");
    /* offset = 10 */

    if (lseek(fd, 5, SEEK_SET) != 5)
        err_sys("lseek error");
    /* offset = 5 */
    currpos = lseek(fd, 0, SEEK_CUR);
    printf("before write, currpos = %d\n", (int)currpos);
    
    if (read(fd, buf, 5) == 5)
        printf("read %s to buffer\n", buf);
    else
        err_sys("read error");

    if (write(fd, buf2, 10) != 10)
        err_sys("second write error");
    /* offset should be 15 if lseek() can affect flag O_APPEND  otherwise it can not */
    currpos = lseek(fd, 0, SEEK_CUR);
    printf("after write, currpos = %d\n", (int)currpos);

    close(fd);

    exit(0);
}
