#

all: payload code_inject host asm

payload: payload.c
	gcc83 -fpic -pie -nostdlib -O0 payload.c -o payload

code_inject: code_inject.c
	gcc83 -g code_inject.c -o code_inject

host: host.c
	gcc83 -g host.c -o host 

asm: asm.c
	gcc83 -finline-functions asm.c -o asm

clean:
	rm -rf code_inject payload host asm *.txt core .gdb_history
