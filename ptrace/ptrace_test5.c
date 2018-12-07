#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    pid_t traced_process;
    struct user_regs_struct regs;
    long ins;

    if (argc != 2) {
        printf("Usage: %s <pid to be traced>\n", argv[0]);
        exit(-1);
    }

    traced_process = atoi(argv[1]);
    /*
     * While being traced, the tracee will stop each time a signal is
     * delivered, even if the signal is being ignored.
     */
    if (ptrace(PTRACE_ATTACH, traced_process, NULL, NULL) < 0) {
        perror("ptrace attach failed");
        exit(-1);
    }
    wait(NULL); /* now target process is a child of current process */
    if (ptrace(PTRACE_GETREGS, traced_process, NULL, &regs) < 0) {
        perror("ptrace get regs failed");
        exit(-1);
    }

    //if (ptrace(PTRACE_CONT, traced_process, NULL, NULL) < 0) {
    //    perror("ptrace cont failed");
    //    exit(-1);
    //}
    //wait(NULL);
    //if (ptrace(PTRACE_GETREGS, traced_process, NULL, &regs) < 0) {
    //    perror("ptrace get regs failed");
    //    exit(-1);
    //}

    errno = 0;
    if ((ins = ptrace(PTRACE_PEEKTEXT, traced_process, regs.eip, NULL)) < 0 && errno != 0) {
        perror("ptrace peek text failed\n");
        exit(-1);
    }
    printf("EIP: %lx instruction executed: %lx\n", regs.eip, ins);
    ptrace(PTRACE_DETACH, traced_process, NULL, NULL);

    return 0;
}


