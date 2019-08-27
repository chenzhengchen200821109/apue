/*
 * Actually payload can execute from shell. Its entry point address is 0x1eb.
 * 将被注入的代码。
 */
int _write(int fd, char* buf, unsigned int len)
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

void Exit(int status)
{
    __asm__ volatile(
            "int $0x80"
            :
            : "a"(1), "b"(status)
            : "memory", "cc"
            );
}

/*
 * MUST be _start because ld only require it. ld do NOT care about main() except _start().
 */
int _start()
{
    _write(1, "I am the payload wha has hijacked your process\n", 48);
    Exit(0);
    return 0;
}

