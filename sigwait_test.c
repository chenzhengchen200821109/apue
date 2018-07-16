#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sig_handler(int signo)
{
    printf("Received signal. %d\n", signo);
}

void *sig_thread()
{
    sigset_t waitset, oset;
    int sig;
    int rc;
    pthread_t ppid = pthread_self();

    pthread_detach(ppid);
    sigemptyset(&waitset);
    sigaddset(&waitset, SIGALRM);
    sigaddset(&waitset, SIGTERM);
    sigaddset(&waitset, SIGINT);
    sigaddset(&waitset, SIGUSR1);
    sigaddset(&waitset, SIGUSR2);
    while (1) {
        if ((rc = sigwait(&waitset, &sig)) != -1)
            sig_handler(sig);
        else
            printf("sigwait() returned err: %d, %s\n", errno, strerror(errno));
    }
}

int main()
{
    sigset_t bset, oset;
    int i;
    pid_t pid = getpid();
    pthread_t ppid;

    sigemptyset(&bset);
    sigaddset(&bset, SIGALRM);
    sigaddset(&bset, SIGTERM);
    sigaddset(&bset, SIGINT);
    sigaddset(&bset, SIGUSR1);
    sigaddset(&bset, SIGUSR2);
    if (pthread_sigmask(SIG_BLOCK, &bset, &oset) != 0)
        printf("set pthread mask failed\n");
    //kill(pid, SIGALRM);
    //kill(pid, SIGTERM);
    //kill(pid, SIGINT);
    //kill(pid, SIGUSR1);
    //kill(pid, SIGUSR2);

    pthread_create(&ppid, NULL, sig_thread, NULL);
    sleep(10);

    exit(0);
}
