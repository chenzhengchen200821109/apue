#include "apue.h"

#define BSZ 48

void printChar(char *, int);

int main()
{
    FILE *fp;
    char buf[BSZ];

    memset(buf, 'a', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';

    //printf("after memset call...\n");
    //printChar(buf, BSZ);

    if ((fp = fmemopen(buf, BSZ, "w+")) == NULL)
        err_sys("fmemopen failed");
    printf("after fmemoopen call...\n");
    //printChar(buf, BSZ);

    printf("initial buffer contents: %s\n", buf);
    //printChar(buf, BSZ);

    fprintf(fp, "hello, world");
    printf("before flush: %s\n", buf);
    fflush(fp);
    printf("afer fflush call...\n");
    //printChar(buf, BSZ);
    printf("after fflush: %s\n", buf);
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    memset(buf, 'b', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';
    fprintf(fp, "hello, world");
    //fseek(fp, 0, SEEK_SET);
    //printf("after fseek call...\n");
    fflush(fp);
    printf("after fflush call...\n");
    //printChar(buf, BSZ);
    //printf("after fseek: %s\n", buf);
    printf("after fflush: %s\n", buf);
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    memset(buf, 'c', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';
    fprintf(fp, "hello, world");
    fclose(fp);
    printf("after fclose call...\n");
    //printChar(buf, BSZ);
    printf("after fclose: %s\n", buf);
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    return 0;
}

/* void printChar(char *buf, int size)
{
    int i;

    for (i = 0; i < size; i++) {
        if (buf[i] != '\0')
            printf("%c", buf[i]);
        else
            printf("*");
    }
    printf("\n");

} */
