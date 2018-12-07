/*
 *
 */

int _write(int fd, char* buf, unsigned int len)
{
    int ret;
    __asm__ volatile (
            "movl $4, %%eax\n"
            "movl %0, %%ebx\n"
            "movl %1, %%ecx\n"
            "movl %2, %%edx\n"
            "int $0x80" : : "g"(fd), "g"(buf), "g"(len));
    __asm__ volatile (
            "movl %%eax, %0\n" : "=r"(ret));
    return ret;
}

void Exit(int status)
{
    __asm__ volatile (
            "movl %0, %%ebx\n"
            "movl $1, %%eax\n"
            "int $0x80" : : "r"(status));
}

int _start()
{
    _write(1, "I am the payload wha has hijacked your process\n", 48);
    Exit(0);
    return 0;
}

