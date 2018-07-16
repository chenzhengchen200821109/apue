#include "apue.h"

static void sig_quit(int);

int main(void)
{
	sigset_t newmask, oldmask, pendmask;
    unsigned int unslept;

	if (signal(SIGQUIT, sig_quit) == SIG_ERR)
			err_sys("can't catch SIGQUIT");

	/*
	 * Block SIGQUIT and save current signal mask.
	 */
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
			err_sys("SIG_BLOCK error");

	unslept = sleep(5); /* SIGQUIT here will remain pending */
    if (unslept == 0)
        printf("sleep system call was not interrupted\n");
    else if (unslept < 5)
        printf("sleep system call has been interrupted\n");
    else
        printf("unknown situations\n");

	if (sigpending(&pendmask) < 0)
			err_sys("sigpending error");
	if (sigismember(&pendmask, SIGQUIT))
			printf("\nSIGQUIT pending\n");

	/*
	 * Restore signal mask which unblocks SIQUIT.
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
			err_sys("SIG_SETMASK error");
	printf("SIGQUIT unblocked\n");

	sleep(5); /* SIGQUIT here will terminate with ccore file */
	exit(0);			
}

static void sig_quit(int signo)
{
	printf("caught SIGQUIT\n");
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
			err_sys("can't reset SIGQUIT");
}
