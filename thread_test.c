#include "apue.h"
#include <pthread.h>

extern char *getenv_unsafe(const char *name);

void printids(const char *s) {
    
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);

    return;
}
void *thr_fn(void *arg) {
    
    char *result;

    printids("new thread: ");
    result = getenv_unsafe("HOME");
    printf("The result is %s\n", result);

    return((void *)0);
}

int main(void) {
    
    int err;
    pthread_t tid1, tid2;

    err = pthread_create(&tid1, NULL, thr_fn, NULL);
    if (err != 0)
        err_exit(err, "can't create thread");
    err = pthread_create(&tid2, NULL, thr_fn, NULL);
    if (err != 0)
        err_exit(err, "can't create thread");
    printids("main thread: ");

    sleep(1);
    exit(0);
}
