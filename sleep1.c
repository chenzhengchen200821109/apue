/*
 * Incorrect version of sleep implementation.
 */
#include <signal.h>
#include <unistd.h>

static void sig_alrm(int signo)
{
	/* nothing to do, just return to wake up the pause. */
}

/*
 * This implementation is buggy.
 */
unsigned int sleep1(unsigned int seconds)
{
    unsigned int sec;
    void (*handler)(int);

	if ((handler = signal(SIGALRM, sig_alrm)) == SIG_ERR)
			return(seconds);
	if ((sec = alarm(seconds)) == 0) { /* no previously scheduled alarm */
        pause(); /* caught signal wakes up us */
        signal(SIGALRM, handler);
        return (alarm(0)); /* turn off timer, return unslept time */
    } else {
        pause();
        sec = alarm(sec);
        signal(SIGALRM, handler);
        return sec;
    }
}
