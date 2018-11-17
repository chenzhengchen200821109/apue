/*
 * tracer.c: gcc -g -Wall tracer.c -o tracer
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/mman.h>
#include <sys/wait.h>

typedef struct handle
{
    Elf32_Ehdr* ehdr;
    Elf32_Phdr* phdr;
    Elf32_Shdr* shdr;
    char* mem;
    char* symname;
    Elf32_Addr symaddr;
    struct user_regs_struct pt_reg;
    char* exec;
} handle_t;

Elf32_Addr lookup_symbol(handle_t* , const char*);

int main(int argc, char* argv[], char* envp[])
{
    int fd;
    handle_t h;
    struct stat st;
    long trap, orig;
    int status, pid;
    char* args[2];
    
    if (argc < 3) {
        printf("Usage: %s <program> <function>\n", argv[0]);
        exit(0);
    }

    if ((h.exec = strdup(argv[1])) == NULL) {
        perror("strdup");
        exit(-1);
    }

    args[0] = h.exec;
    args[1] = NULL;
    if ((h.symname = strdup(argv[2])) == NULL) {
        perror("strdup");
        exit(-1);
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        perror("open");
        exit(-1);
    }

    if (fstat(fd, &st) < 0) {
        perror("fstat");
        exit(-1);
    }

    h.mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (h.mem == MAP_FAILED) {
        perror("mmap");
        exit(-1);
    }
    h.ehdr = (Elf32_Ehdr *)h.mem;
    h.phdr = (Elf32_Phdr *)(h.mem + h.ehdr->e_phoff);
    h.shdr = (Elf32_Shdr *)(h.mem + h.ehdr->e_shoff);

    if (h.mem[0] != 0x7f || strncmp((char *)&h.mem[1], "ELF", 3) != 0) {
        printf("%s is not an ELF file\n", h.exec);
        exit(-1);
    }

    if (h.ehdr->e_type != ET_EXEC) {
        printf("%s is not an ELF executable\n", h.exec);
        exit(-1);
    }

    if (h.ehdr->e_shstrndx == 0 || h.ehdr->e_shoff == 0 || h.ehdr->e_shnum ==0) {
        printf("Section header table not found\n");
        exit(-1);
    }

    if ((h.symaddr = lookup_symbol(&h, h.symname)) == 0) {
        printf("Unable to find symbol: %s not found in executable\n", h.symname);
        exit(-1);
    }
    close(fd);
    
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(-1);
    }

    if (pid == 0) {
        if (ptrace(PTRACE_TRACEME, pid, NULL, NULL) < 0) {
            perror("PTRACE_TRACEME");
            exit(-1);
        }
        execve(h.exec, args, envp);
        exit(0);
    }

    wait(&status);
    printf("Beginning analysis of pid: %d at %x\n", pid, (unsigned int)h.symaddr);
    errno = 0; /* see NOTES below */
    if ((orig = ptrace(PTRACE_PEEKTEXT, pid, h.symaddr, NULL)) < 0 && errno != 0) {
        /*
         * NOTES:
         *   On success, PTRACE_PEEK* requests return the requested data, while other
         *   requests return zero. (On linux, this is done in the libc wrapper around
         *   ptrace system call. On the system call level, PTRACE_PEEK* requests have
         *   a different API: they store the result at the address specified by data
         *   parameter, and return value is the error flag.)
         *   On error, all requests return -1, and errno is set appropriately. Since 
         *   the value returned by a successful PTRACE_PEEK* request may be -1, the 
         *   caller must clear errno before the call, and then check it afterward to
         *   determine whether or not an error occurred.
         */
        perror("PTRACE_PEEKTEXT");
        exit(-1);
    }
    trap = (orig & ~0xff) | 0xcc;
    if (ptrace(PTRACE_POKETEXT, pid, h.symaddr, trap) < 0) {
        perror("PTRACE_POKETEXT");
        exit(-1);
    }

trace:
    if (ptrace(PTRACE_CONT, pid, NULL, NULL) < 0) {
        perror("PTRACE_CONT");
        exit(-1);
    }
    wait(&status);
    if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
        if (ptrace(PTRACE_GETREGS, pid, NULL, &h.pt_reg) < 0) {
            perror("PTRACE_GETREGS");
            exit(-1);
        }
        printf("\nExecutable %s (pid: %d) has hit breakpoint 0x%x\n", h.exec, pid, (unsigned int)h.symaddr);
        printf("%%ebx: %x\n%%ecx: %x\n%%edx: %x\n%%esi: %x\n"
                "%%edi: %x\n%%ebp: %x\n%%eax: %x\n%%ds: %x\n"
                "%%es: %x\n%%fs: %x\n%%gs: %x\n%%orig_eax: %x\n"
                "%%eip: %x\n%%cs: %x\n%%eflags: %x\n%%esp: %x\n%%ss: %x\n", 
                (unsigned int)h.pt_reg.ebx, (unsigned int)h.pt_reg.ecx, (unsigned int)h.pt_reg.edx,
                (unsigned int)h.pt_reg.esi, (unsigned int)h.pt_reg.edi, (unsigned int)h.pt_reg.ebp, 
                (unsigned int)h.pt_reg.eax, (unsigned int)h.pt_reg.xds, (unsigned int)h.pt_reg.xes, 
                (unsigned int)h.pt_reg.xfs, (unsigned int)h.pt_reg.xgs, (unsigned int)h.pt_reg.orig_eax, 
                (unsigned int)h.pt_reg.eip, (unsigned int)h.pt_reg.xcs, (unsigned int)h.pt_reg.eflags,
                (unsigned int)h.pt_reg.esp, (unsigned int)h.pt_reg.xss);
        printf("\nPlease hit any key to continue: ");
        getchar();
        if (ptrace(PTRACE_POKETEXT, pid, h.symaddr, orig) < 0) {
            perror("PTRACE_POKETEXT");
            exit(-1);
        }

        h.pt_reg.eip = h.pt_reg.eip - 1;
        if (ptrace(PTRACE_SETREGS, pid, NULL, &h.pt_reg) < 0) {
            perror("PTRACE_SETREGS");
            exit(-1);
        }

        if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) < 0) {
            perror("PTRACE_SINGLESTEP");
            exit(-1);
        }
        wait(NULL);

        if (ptrace(PTRACE_POKETEXT, pid, h.symaddr, trap) < 0) {
            perror("PTRACE_POKETEXT");
            exit(-1);
        }
        goto trace;
    }

    if (WIFEXITED(status))
        printf("Completed tracing pid: %d\n", pid);

    exit(0);
}

Elf32_Addr lookup_symbol(handle_t* h, const char* symname)
{
    int i, j;
    char* strtab;
    Elf32_Sym* symtab;

    for (i = 0; i < h->ehdr->e_shnum; i++) {
        if (h->shdr[i].sh_type == SHT_SYMTAB) {
            strtab = (char *)&h->mem[h->shdr[h->shdr[i].sh_link].sh_offset];
            symtab = (Elf32_Sym *)&h->mem[h->shdr[i].sh_offset];
            for (j = 0; j < h->shdr[i].sh_size/sizeof(Elf32_Sym); j++) {
                if (strcmp(&strtab[symtab->st_name], symname) == 0)
                    return (symtab->st_value);
                symtab++;
            }
        }
    }

    return 0;
}
