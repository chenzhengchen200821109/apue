#include "apue.h"

void caller(void);

int main(void) 
{
	printf("we are in main routine\n");

	caller();

	printf("we should never be here\n");

	exit(0);
}

void caller(void)
{
	printf("we are in caller routine\n");

	exit(0);
}
