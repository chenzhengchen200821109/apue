#include "apue.h"
#include <errno.h>
#include <signal.h>

#define NSIGS 31

void pr_mask(const char *str)
{
	sigset_t sigset;
	int errno_save;

	errno_save = errno; /* we can be called by signal handlers */
	if (sigprocmask(0, NULL, &sigset) < 0) {
		err_ret("sigprocmask error");
	} else {
		printf("%s", str);
        if (sigismember(&sigset, SIGHUP))
            printf(" SIGHUP");
		if (sigismember(&sigset, SIGINT))
			printf(" SIGINT");
		if (sigismember(&sigset, SIGQUIT))
			printf(" SIGQUIT");
        if (sigismember(&sigset, SIGILL))
            printf(" SIGILL");
        if (sigismember(&sigset, SIGABRT))
            printf(" SIGABRT");
        if (sigismember(&sigset, SIGFPE))
            printf(" SIGFPE");
        if (sigismember(&sigset, SIGKILL))
            printf(" SIGKILL");
        if (sigismember(&sigset, SIGSEGV))
            printf(" SIGSEGV");
        if (sigismember(&sigset, SIGPIPE))
            printf(" SIGPIPE");
		if (sigismember(&sigset, SIGALRM))
			printf(" SIGALRM");
        if (sigismember(&sigset, SIGTERM))
            printf(" SIGTERM");
        if (sigismember(&sigset, SIGUSR1))
            printf(" SIGUSR1");
        if (sigismember(&sigset, SIGUSR2))
            printf(" SIGUSR2");
        if (sigismember(&sigset, SIGCHLD))
            printf(" SIGCHLD");
        if (sigismember(&sigset, SIGCONT))
            printf(" SIGCONT");
        if (sigismember(&sigset, SIGSTOP))
            printf(" SIGSTOP");
        if (sigismember(&sigset, SIGTSTP))
            printf(" SIGTSTP");
        if (sigismember(&sigset, SIGTTIN))
            printf(" SIGTTIN");
        if (sigismember(&sigset, SIGTTOU))
            printf(" SIGTTOU");
        if (sigismember(&sigset, SIGBUS))
            printf(" SIGBUS");
        if (sigismember(&sigset, SIGPOLL))
            printf(" SIGPOLL");
        if (sigismember(&sigset, SIGPROF))
            printf(" SIGPROF");
        if (sigismember(&sigset, SIGSYS))
            printf(" SIGSYS");
        if (sigismember(&sigset, SIGTRAP))
            printf(" SIGTRAP");
        if (sigismember(&sigset, SIGURG))
            printf(" SIGURG");
        if (sigismember(&sigset, SIGVTALRM))
            printf(" SIGVTALRM");
        if (sigismember(&sigset, SIGXCPU))
            printf(" SIGXCPU");
        if (sigismember(&sigset, SIGXFSZ))
            printf(" SIGXFSZ");
        if (sigismember(&sigset, SIGIOT))
            printf(" SIGIOT");
        if (sigismember(&sigset, SIGSTKFLT))
            printf(" SIGSTKFLT");
        if (sigismember(&sigset, SIGIO))
            printf(" SIGIO");
        if (sigismember(&sigset, SIGCLD))
            printf(" SIGCLD");
        if (sigismember(&sigset, SIGPWR))
            printf(" SIGPWR");
        if (sigismember(&sigset, SIGWINCH))
            printf(" SIGWINCH");
        if (sigismember(&sigset, SIGUNUSED))
            printf(" SIGUNUSED");
		printf("\n");
	}
	errno = errno_save;
}

char *sigArray[] = { 
    /* 1,2,3,4,5,6,7,8 */ "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE",
    /* 9,10,11,12,13,14,15,16 */ "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFT",
    /* 17,18,19,20,21,22,23,24 */ "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU",
    /* 25,26,27,28,29,30,31,32 */ "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGIO", "SIGPWR", "SIGUNUSED", NULL
    };

void pr_mask_v1(const char *str)
{
    sigset_t sigset;
    int errno_save;
    int i;

    errno_save = errno;
    if (sigprocmask(0, NULL, &sigset) < 0)
        err_ret("sigprocmask error");
    else {
        printf("%s", str);
        for (i = 1; i < NSIGS + 1; i++) {
            if (sigismember(&sigset, i))
                printf(" %s", sigArray[i-1]);
        }
        printf("\n");
    }
    errno = errno_save;
}

