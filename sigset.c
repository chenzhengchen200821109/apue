#include <signal.h>
#include <errno.h>

/*
 * <signal.h> usually defines NSIG to include signal number 0.
 */

#define SIGBAD(signo) ((signo) <= 0 || (signo) >= NSIG)

#define sigemptyset(ptr) (*(ptr) = 0)
#define sigfillset(ptr) (*(ptr) = ~(sigset_t)0, 0)

int sigaddset(sigset_t *set, int signo)
{
	if (SIGBAD(signo)) {
		errno = EINVAL;
		return(-1);
	}
	*set |= 1 << (signo -1); /* turn bit on */
	return(0);
}

int sigdelet(sigset_t *set, int signo)
{
	if (SIGBAD(signo)) {
		errno = EINVAL;
		return(-1);
	}
	*set &= ~(1 << (signo - 1)); /* turn bit off */
	return(0);
}

int sigismember(const sgiset_t *set, int signo)
{
	if (SIGBAD(signo)) {
		errno = EINVAL;
		return(-1);
	}
	return ((*set & (1 << (signo - 1))) != 0);
}
