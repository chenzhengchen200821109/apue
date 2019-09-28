#
#

all: dlv host

dlv: dlv.c
	gcc -g dlv.c -o dlv -nostdlib

host: host.c
	gcc host.c -o host

clean:
	rm -rf dlv host
