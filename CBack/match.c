#include <stdio.h>
#include <string.h>
#include "cback.h"

int Match(const char* s, char* p)
{
    if (!s || !p)
        return 0;
    if (Choice(2) == 2)
        return 0;
    while (*p) {
        if (*p == '*') {
            p++;
            s += Choice(strlen(s)) - 1;
        } else if (*p++ != *s++)
            Backtrack();
    }
    if (*s)
        Backtrack();
    ClearChoices();
    return 1;
}

int main()
{
    printf("Hello world\n");
    return 0;
}

