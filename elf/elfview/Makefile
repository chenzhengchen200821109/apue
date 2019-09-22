#
#

all: BinaryFile_Test Elf_Header_Test Elf_Section_Header_Test Elf_Program_Header_Test test Util_Test Elf_Section_Shstrtab_Test elfview  

BinaryFile_Test: BinaryFile_Test.cpp BinaryFile.cpp BinaryFile.h Util.cpp Util.h 
	g++ -std=c++11 -g BinaryFile_Test.cpp BinaryFile.cpp Util.cpp -o BinaryFile_Test -lglog

Elf_Header_Test: Elf_Header_Test.cpp BinaryFile.cpp Elf_Header.h Util.cpp 
	g++ -std=c++11 -g BinaryFile.cpp Elf_Header.h Util.cpp Elf_Header_Test.cpp -o Elf_Header_Test -lglog 

Elf_Section_Header_Test: Elf_Section_Header_Test.cpp BinaryFile.cpp Elf_Header.h Util.cpp Elf_Section_Header.h
	g++ -std=c++11 -g BinaryFile.cpp Util.cpp Elf_Section_Header_Test.cpp -o Elf_Section_Header_Test -lglog 

Elf_Program_Header_Test: Elf_Program_Header.h Elf_Program_Header_Test.cpp Util.cpp BinaryFile.cpp
	g++ -std=c++11 -g Elf_Program_Header_Test.cpp Elf_Program_Header.h BinaryFile.cpp Util.cpp -o Elf_Program_Header_Test -lglog

Elf_Section_Shstrtab_Test: Elf_Section_Shstrtab_Test.cpp Elf_Header.h Util.cpp BinaryFile.cpp
	g++ -std=c++11 -g Elf_Section_Shstrtab_Test.cpp BinaryFile.cpp Util.cpp -o Elf_Section_Shstrtab_Test -lglog

test: test.cpp
	g++ -std=c++11 -g test.cpp -o test 

Util_Test: Util_Test.cpp Util.cpp
	g++ -std=c++11 -g Util_Test.cpp Util.cpp -o Util_Test -lglog

elfview: elfview.cpp
	g++ -std=c++11 -g elfview.cpp Util.cpp BinaryFile.cpp Elf_Ident.cpp -o elfview -lglog

clean:
	rm -rf BinaryFile_Test Elf_Ident_Test Elf_Header_Test test a.out Elf_Program_Header_Test Util_Test Elf_Section_Header_Test Elf_Section_Shstrtab_Test elfview
	rm -rf .gdb* .*.swp .*.h.swp

