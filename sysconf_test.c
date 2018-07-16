#include "apue.h"

int main(void)
{
    char *ptr;
    size_t size;

    if ((ptr = (char *)path_alloc(&size)) == NULL)
        err_sys("path_alloc error");
    else
        free(ptr);

    printf("size of path_alloc is %d\n", size);

    exit(0);
}
