#
#

all: elfview  

elfview: elfview.cpp
	g++ -std=c++11 -g elfview.cpp Util.cpp BinaryFile.cpp Elf_Ident.cpp -o elfview -lglog

install: elfview
	sudo cp elfview /usr/local/bin

clean:
	rm -rf elfview
	rm -rf .gdb* .*.swp .*.h.swp

