#include "apue.h"

int main()
{
    char *login_name;

    if ((login_name = getlogin()) != NULL)
        printf("login name is %s\n", login_name);

    exit(0);
}
