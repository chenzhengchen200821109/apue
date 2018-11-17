CC = gcc
CFLAGS = -Wall -g

### using implicit rules
% : %.c
	$(CC) $(CFLAGS) -o $* $<

### wild card '?' matchs single character
clean:
	rm -rf ptrace_test?
