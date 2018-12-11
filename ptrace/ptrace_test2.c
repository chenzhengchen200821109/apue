#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <stdio.h>

int main()
{   
    pid_t child;
    long orig_eax, eax;
    int status;
    int insyscall = 0;
    struct user_regs_struct regs;
    child = fork();
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    }
    else {
       while(1) {
          wait(&status);
          if(WIFEXITED(status))
              break;
          orig_eax = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);
          if(orig_eax == SYS_write) {
              if(insyscall == 0) {
                 /* Syscall entry */
                 insyscall = 1;
                 ptrace(PTRACE_GETREGS, child, NULL, &regs);
                 printf("Write called with %ld, %ld, %ld\n",
                        regs.ebx, regs.ecx, regs.edx);
              } else { /* Syscall exit */
                  eax = ptrace(PTRACE_PEEKUSER, child, 4 * EAX, NULL);
                  printf("Write returned with %ld\n", eax);
                  insyscall = 0;
              }
          }
          /*
           * restart the stopped tracee, but arrange for the
           * tracee to be stopped at the next entry to or exit
           * from a system call.
           */
          ptrace(PTRACE_SYSCALL, child, NULL, NULL);
       }
   }
   return 0;
}
