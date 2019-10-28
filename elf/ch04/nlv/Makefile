#
#

all: nlv host

nlv: nlv.c
	gcc -g nlv.c -o nlv -nostdlib

host: host.c
	gcc host.c -o host

clean:
	rm -rf nlv host .v .*.*.swp .*.swp
