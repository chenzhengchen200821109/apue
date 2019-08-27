/*
 * 通过此程序向目标程序注入一段代码。
 */ 
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

//32-bit arch
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

static inline volatile void* evil_mmap(void *, size_t, int, int, int, off_t) __attribute__((aligned(8), always_inline));
void injection_code(void *) __attribute__((aligned(8)));
uint32_t get_text_base(pid_t);
int pid_write(int, void *, const void *, size_t);
char* create_fn_shellcode(void (*fn)(), size_t len);

void* f1 = (void *)injection_code;
void* f2 = (void *)get_text_base;

static inline volatile int evil_write(int fd, char* buf, size_t len)
{
    int ret;
    __asm__ volatile(
            "int $0x80" 
            : "=a"(ret) 
            : "0"(4), "b"(fd), "c"(buf), "d"(len)
            : "memory", "cc"
            );
    return ret;
}

static inline volatile int evil_fstat(int fd, struct stat* buf)
{
    int ret;
    __asm__ volatile(
            "int $0x80"
            : "=a"(ret)
            : "0"(108), "b"(fd), "c"(buf)
            : "memory", "cc"
            );
    return ret;
}

static inline volatile int evil_open(const char* path, int flags)
{
    int ret;
    __asm__ volatile(
            "int $0x80"
            : "=a"(ret)
            : "0"(5), "b"(path), "c"(flags)
            : "memory", "cc"
            );
    return ret;
}

static inline volatile void* evil_mmap(void* addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    int ret;

    /* ----------------- NOTE ---------------------------
     * 当输入参数小于或者等于5个时，linux用寄存器传递参数。
     *     Linux takes system call arguments in registers:
     *     syscall number   %eax
     *     arg 1            %ebx
     *     arg 2            %ecx
     *     arg 3            %edx
     *     arg 4            %esi
     *     arg 5            %edi
     * 当输入参数大于5个时，把参数按照顺序放入连续内存中，
     * 并把这块内存的首地址放入%ebx中。
     */
    __asm__ volatile(
            "subl $0x18, %%esp\n"
            "movl %1, (%%esp)\n"
            "movl %2, 0x4(%%esp)\n"
            "movl %3, 0x8(%%esp)\n"
            "movl %4, 0xc(%%esp)\n"
            "movl %5, 0x10(%%esp)\n"
            "movl %6, 0x14(%%esp)\n"
            "movl %%esp, %%ebx\n"
            "movl $90, %%eax\n"
            "int $0x80\n"
            "addl $0x18, %%esp"
            : "=a"(ret)    
            : "a"(addr), "b"(len), "c"(prot), "d"(flags), "S"(fd), "D"(offset)
            : "memory", "cc"
            );
    return (void *)ret;
}

/**
 * when compile, value of injection_code() must be smaller than value
 * of get_text_base() because injection_code() has been define first.
 */
void injection_code(void* vaddr)
{
    volatile void* mem;
    //mem = evil_mmap(vaddr, 8192, (PROT_READ | PROT_WRITE | PROT_EXEC), (MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS), -1, 0);
    mem = evil_mmap((void *)BASE_ADDRESS, 8192, (PROT_READ | PROT_WRITE | PROT_EXEC), (MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS), -1, 0);
    //产生中断异常，返回到调试进程
    __asm__ volatile ("int3");
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
        if (!strstr(line, "r-xp"))
            continue;
        /*
         * The alloca() function allocates size bytes in the stack fram
         * of the caller. This temporary space is automatically freed
         * when the function that called alloca() returns to its caller.
         */
        for (i = 0, start = (char *)alloca(32), p = line; *p != '-'; i++, p++)
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
    char* shellcode = (char *)malloc(len);
    char* p = (char *)fn;
    for (i = 0; i < len; i++)
        *(shellcode + i) = *p++;
    return shellcode;
}

/**
 * read memory space of a process whose PID equals pid.
 */
int pid_read(int pid, void* dst, const void* src, size_t len)
{
    //sz必须是4的倍数
    int sz = len / sizeof(long);
    unsigned char* s = (unsigned char *)src;
    unsigned char* d = (unsigned char *)dst;
    //long word;

    union u {
        long word;
        char chars[sizeof(long)];
    } data;

    while (sz > 0) {
        errno = 0;
        printf("s = %p\n", s);
        printf("d = %p\n", d);
        if ((data.word = ptrace(PTRACE_PEEKTEXT, pid, s, NULL)) < 0 && errno != 0) {
            fprintf(stderr, "pid_read failed, pid: %d: %s\n", pid, strerror(errno));
            goto fail;
        }
        memcpy(d, data.chars, sizeof(long));
        //*(long *)d = word;
        s += sizeof(long);
        d += sizeof(long);
        sz--;
    }
    return 0;
fail:
    perror("PTRACE_PEEKTEXT");
    return -1;
}

int pid_write(int pid, void* dest, const void* src, size_t len)
{
    long sz = len / sizeof(long);
    unsigned char* s = (unsigned char *)src;
    unsigned char* d = (unsigned char *)dest;

    union u {
        long val;
        char chars[sizeof(long)];
    } data;
    while (sz > 0) {
        memcpy(data.chars, s, sizeof(long));
        /* 
         * PTRACE_POKETEXT - Copy the word data to the address 
         * addr in the tracee's memory.
         * 注意data参数。
         */
        if (ptrace(PTRACE_POKETEXT, pid, d, data.val) == -1)
            goto failed;
        s += sizeof(long);
        d += sizeof(long);
        sz--;
    }
    return 0;
failed:
    perror("PTRACE_POKETEXT");
    return -1;
}

int main(int argc, char* argv[])
{
    handle_t h;
    unsigned int shellcode_size = (unsigned int)f2 - (unsigned int)f1; //
    printf("shellcode_size = %d\n", shellcode_size);
    int fd, status;
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
    shellcode_size += 4; //为了后边调用pid_read()时和pid_write()时规整为4的倍数。
    printf("shellcode_size = %d\n", shellcode_size);
    //h.shellcode指向一段shellcode，即函数injection_code().
    h.shellcode = create_fn_shellcode((void *)&injection_code, shellcode_size);
    origcode = (char *)alloca(shellcode_size);
    //save original code of target.
    if (pid_read(h.pid, (void *)origcode, (void *)h.base, shellcode_size) < 0)
        exit(1);
    //write shellcode in target's process space.
    if (pid_write(h.pid, (void *)(h.base), (void *)h.shellcode, shellcode_size) < 0)
        exit(1);

    char* checkcode = (char *)alloca(shellcode_size);
    if (pid_read(h.pid, (void *)checkcode, (void *)(h.base), shellcode_size) < 0)
        exit(1);

    if (ptrace(PTRACE_GETREGS, h.pid, NULL, &h.pt_reg) < 0) {
        perror("PTRACE_GETREGS");
        exit(1);
    }
    //target will re-execute here. Yes function injection_code() begin to start.
    //But there is ONE problem. What if function injection_code() has a few arguments?
    h.pt_reg.eip = h.base;
    //h.pt_reg.eax = BASE_ADDRESS; //%ebx MUST be set to accomplish function evil_mmap().
    if (ptrace(PTRACE_SETREGS, h.pid, NULL, &h.pt_reg) < 0) {
        perror("PTRACE_SETREGS");
        exit(1);
    }
    if (ptrace(PTRACE_GETREGS, h.pid, NULL, &h.pt_reg) < 0) {
        perror("PTRACE_GETREGS");
        exit(1);
    }
    if (ptrace(PTRACE_CONT, h.pid, NULL, NULL) < 0) {
        perror("PTRACE_CONT");
        exit(1);
    }

    waitpid(h.pid, &status, 0);
    if (!WIFSTOPPED(status)) {
        printf("something went wrong\n");
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
