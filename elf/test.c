#include <stdio.h>

void print_string(const char* str)
{
    printf("%s\n", str);
}

int main()
{
    char str0[] = "Hello 1";
    char str1[] = "Hello 2";

    print_string(str0);
    print_string(str1);

    //while (1)
    //    ;

    return 0;
}
