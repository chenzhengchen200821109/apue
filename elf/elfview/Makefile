#
#

all: BinaryFile_Test Elf_Ident_Test test 

BinaryFile_Test: BinaryFile_Test.cpp BinaryFile.cpp BinaryFile.h
	g++ -std=c++11 -g BinaryFile_Test.cpp BinaryFile.cpp -o BinaryFile_Test -lglog

Elf_Ident_Test: Elf_Ident_Test.cpp Elf_Ident.cpp BinaryFile.cpp
	g++ -std=c++11 -g Elf_Ident_Test.cpp Elf_Ident.cpp BinaryFile.cpp -o Elf_Ident_Test -lglog

test: test.cpp
	g++ -std=c++11 -g test.cpp -o test 

clean:
	rm -rf BinaryFile_Test Elf_Ident_Test test a.out

