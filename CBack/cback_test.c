#include <stdio.h>
#include "cback.h"

extern char* StackBottom;

void test()
{
    int i;
    int j;
    i = Choice(3);
    j = Choice(2);
    printf("i = %d, j = %d\n", i, j);
    Backtrack();
}

int main()
{
    Backtracking(test());

    return 0;
}

