#include "cback.h"
#include <stdio.h>

int main()
{
    int r, c; /* row, column */
    int R[9] = {0}, S[15] = {0}, D[15] = {0}; /* clear board */

    for (r = 1; r <= 8; r++) {
        c = Choice(8);
        if (R[c] || S[r+c-2] || D[r-c+7])
            Backtrack();
        R[c] = S[r+c-2] = D[r-c+7] = r;
    }

    for (c = 1; c <= 8; c++)
        printf("(%d, %d) ", R[c], c);
    return 0;
}
