#
#
#

BINUTILS_TARGET = -DTARGET='"i686-pc-linux-gnu"'
BINUTILS_SOURCE = binutils-2.32
BINUTILS_CFLAGS = -I$(BINUTILS_SOURCE)/bfd -I$(BINUTILS_SOURCE)/include -I$(BINUTILS_SOURCE)/binutils -g -c
BINUTILS_LDFLAGS = -L$(BINUTILS_SOURCE)/bfd -L$(BINUTILS_SOURCE)/libiberty -L.

all: main main1 main2 
	
main: main.o trace.o 
	g++83 -g -rdynamic trace.o main.o $(BINUTILS_LDFLAGS) -o main -lbfd -liberty -ldl -lz

main.o: main.cpp 
	g++83 -std=c++11 -g -finstrument-functions -c main.cpp -o main.o 

trace.o: trace.c  
	gcc83 $(BINUTILS_CFLAGS) -fno-instrument-functions trace.c -o trace.o

#trace1.o: trace1.c
#	gcc83 $(BINUTILS_CFLAGS) -fno-instrument-functions trace1.c -o trace1.o

#生成静态库
libtrace.a: trace.o  
	ar crv libtrace.a trace.o
#生成共享库
libtrace.so: trace.c
	gcc83 -shared -fPIC $(BINUTILS_CFLAGS) trace.c -o libtrace.so 
#链接静态库	
main1: main.cpp libtrace.a 
	g++ -g -std=c++11 -finstrument-functions main.cpp -rdynamic  $(BINUTILS_LDFLAGS) libtrace.a -lbfd -liberty -ldl -lz -o main1
#链接共享库
main2: main.cpp libtrace.so
	g++ -g -std=c++11 -rdynamic -finstrument-functions main.cpp $(BINUTILS_LDFLAGS) -ltrace -lbfd -liberty -ldl -lz -o main2

clean:
	rm -rf a.out main main.o trace.o *.txt *.text main1 main2 libtrace.so libtrace.a .gdb_history .*.swp .*.c.swo .*.c.swp

