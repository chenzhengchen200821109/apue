#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

static jmp_buf env_alrm;

static void sig_alrm(int signo)
{
	longjmp(env_alrm, 1);
}

/*
 * This implementation should be correct.
 */
unsigned int sleep2(unsigned int seconds)
{
    unsigned int sec;
    void (*handler)(int);

	if ((handler = signal(SIGALRM, sig_alrm)) == SIG_ERR)
			return(seconds);
	if (setjmp(env_alrm) == 0) {
		if ((sec = alarm(seconds)) == 0) { /* no previously scheduled alarm */
            pause(); /* caught signal wakes up us */
            signal(SIGALRM, handler);
            return (alarm(0)); /* turn off timer, return unslept time */
        } else {
		pause(); /* next caught signal wakes us up */
        sec = alarm(sec);
        signal(SIGALRM, handler);
        return sec;
	    }
    }
}
