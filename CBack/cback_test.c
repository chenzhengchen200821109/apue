#include <stdio.h>
#include "cback.h"

extern char* StackBottom;

void test()
{
    int i;
    int j;
    i = Choice(3);
    j = Choice(1);
    printf("i = %d, j = %d\n", i, j);
    //printf("i = %d\n", i);
    Backtrack();
}

int main()
{
    Backtracking(test());
}

