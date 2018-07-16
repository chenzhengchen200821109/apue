#include "apue.h"

int main(void)
{
    int n, num1, num2;
    char line[MAXLINE];

    while ((n =read(STDIN_FILENO, line, MAXLINE)) > 0) {
        line[n] = 0; /* null terminate */
        if (sscanf(line, "%d%d", &num1, &num2) == 2) {
            sprintf(line, "%d\n", num1 + num2);
            n = strlen(line);
            if (write(STDOUT_FILENO, line, n) != n)
                err_sys("write error");
        } else {
            if (write(STDOUT_FILENO, "invalid args\n", 13) != 13)
                err_sys("write error");
        }
    }
    exit(0);
}
