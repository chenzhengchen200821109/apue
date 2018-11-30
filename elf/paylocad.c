/*
 *
 */

int _write(int fd, char* buf, unsigned int len)
{
    
}

void Exit(int status)
{
    
}

int _start()
{
    _write(1, "I am the payload wha has hijacked your process\n", 48);
    Exit(0);
}

