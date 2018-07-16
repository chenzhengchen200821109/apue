#include "apue.h"
#include "dup2.h"

int main(void)
{
    char *output = "dup2_u.txt";
    dup2_test(dup2_u, output);

    return 0;
}
