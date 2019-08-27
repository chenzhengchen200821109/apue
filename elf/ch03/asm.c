/*
 * asm.c - How to Use Inline Assembly Language in C Code.
 */

#include <stdio.h>
#include <sys/mman.h>

static inline volatile void* user_mmap(void *, size_t, int, int, int, off_t) __attribute__((always_inline));

static inline volatile int user_write(int fd, char* buf, unsigned int len)
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

static inline volatile void user_exit(int status)
{
    __asm__ volatile(
            "int $0x80"
            :
            : "a"(1), "b"(status)
            : "memory", "cc"
            );
}

static inline volatile void* user_mmap(void* addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    int ret;

    int mmap_flags = flags;
    int mmap_fd = fd;
    off_t mmap_offset = offset;

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
            : "a"(addr), "b"(len), "c"(prot), "d"(mmap_flags), "S"(mmap_fd), "D"(mmap_offset)
            : "memory", "cc"
            );
    return (void *)ret;
}

/*
 * MUST be _start because ld only require it. ld do NOT care about main() except _start().
 */
int main()
{
    char* p;
    char* p0;
    user_write(1, "I am the payload wha has hijacked your process\n", 48);
    p = (char *)user_mmap((void *)0x100000, 8192, (PROT_READ | PROT_WRITE | PROT_EXEC), (MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED), -1, 0);
    p[0] = 'H';
    p[1] = 'e';
    p[2] = 'l';
    p[3] = 'l';
    p[4] = 'o';
    p[5] = '\n';
    p[6] = '\0';
    printf("address of p = %p, contents of p = %s", p, p);
    user_exit(0);    
}

