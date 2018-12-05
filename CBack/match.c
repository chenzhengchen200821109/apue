#include <stdio.h>
#include <string.h>
#include "cback.h"
#include <assert.h>

extern char* StackBottom;

/**
 * Match() returns 1 if matched otherwise 0.
 */
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
    if (*s) /* still has some characters in s, back to line 9 */
        Backtrack();
    ClearChoices();
    return 1;
}

int main()
{
    char str[] = "abababababc";
    char pattern[] = "ab*c";
    int result;

    Backtracking(result = Match(str, pattern));
    assert(result == 1); 

    return 0;
}

