#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

const int long_size = sizeof(long);

void getdata(pid_t child, long addr, char *str, int len)
{   
    char *laddr;
    int i, j;
    union u {
        long val;
        char chars[long_size];
    } data;

    i = 0;
    j = len / long_size;
    laddr = str;

    while(i < j) {
        data.val = ptrace(PTRACE_PEEKTEXT, child, addr + i * 4, NULL);
        memcpy(laddr, data.chars, long_size);
        ++i;
        laddr += long_size;
    }

    j = len % long_size;
    if(j != 0) {
        data.val = ptrace(PTRACE_PEEKTEXT, child, addr + i * 4, NULL);
        memcpy(laddr, data.chars, j);
    }
    str[len] = '\0';
}

void putdata(pid_t child, long addr, char *str, int len)
{   
    char *laddr;
    int i, j;
    union u {
        long val;
        char chars[long_size];
    }data;

    i = 0;
    j = len / long_size;
    laddr = str;

    while(i < j) {
		memcpy(data.chars, laddr, long_size);
        ptrace(PTRACE_POKETEXT, child, addr + i * 4, data.val);
        ++i;
        laddr += long_size;
    }

    j = len % long_size;
    if(j != 0) {
        memcpy(data.chars, laddr, j);
        ptrace(PTRACE_POKETEXT, child, addr + i * 4, data.val);
    }
}

int main(int argc, char* argv[])
{
	int wait_status;
    pid_t traced_process;
	struct user_regs_struct oldregs, newregs;
	
	/* int 0x80, int3 */
	char code[] = { 0xcc, '\0' };
	char backup[5];

	if (argc != 2) {
		printf("Usage: %s <pid to be traced>\n", argv[0]);
		exit(-1);
	}

	traced_process = atoi(argv[1]);
	ptrace(PTRACE_ATTACH, traced_process, NULL, NULL);
	wait(&wait_status);

	ptrace(PTRACE_GETREGS, traced_process, NULL, &oldregs);
	printf("first breakpoint is at 0x%lx\n", oldregs.eip);
	/* Copy instructions into a backup variable */
	getdata(traced_process, oldregs.eip, backup, 4);
	/* Put the breakpoint */
	putdata(traced_process, oldregs.eip, code, 1);

	/* Let the process continue and execute the int3 instruction */
	ptrace(PTRACE_CONT, traced_process, NULL, NULL);
	wait(&wait_status);
	printf("The process stopped, putting back the original instructions\n");
	printf("Press <enter> to continue\n");
	getchar();

	ptrace(PTRACE_GETREGS, traced_process, 0, &newregs);
	printf("resumed breakpoint is at 0x%lx\n", newregs.eip);
	assert(oldregs.eip == (newregs.eip - 1));
	
	putdata(traced_process, oldregs.eip, backup, 4);
	/* Setting the eip back to the orginal instruction to let the process continue */
	ptrace(PTRACE_SETREGS, traced_process, NULL, &oldregs);
	ptrace(PTRACE_DETACH, traced_process, NULL, NULL);
	
    return 0;
}
