#include <stdio.h>

extern char* make_message(const char* fmt, ...);

int main() 
{
    char* str;

    str = make_message("This is a long long long long long line");
    printf("%s\n", str);

    return 0;
}
