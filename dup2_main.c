#include "apue.h"
#include "dup2.h"

int main(void)
{
    char *output = "dup2.txt";
    dup2_test(dup2, output);

    return 0;
}
