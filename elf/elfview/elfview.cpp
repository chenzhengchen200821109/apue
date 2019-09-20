#include <glog/logging.h>
#include <iostream>
#include <elf.h>
#include "BinaryFile.h"
#include "Elf_Header.h"
#include "Elf_Section_Header.h"
#include "Elf_Program_Header.h"

int main(int argc, char* argv[])
{
    uint16_t type; //the object file type.

    uint32_t offset, size;
    uint16_t num, entsize, strtabndx;
    uint16_t phnum, phentsize;
    uint16_t shnum, shentsize;
    uint32_t start, end;

    //Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);

    if (argc != 2) {
        std::cerr << "Usage: elfview binaryfile" << std::endl;
        return -1;
    }

    elfview::BinaryFile file(argv[1]);
    elfview::Elf_Header header(&file);
    //
    shnum = header.Elf_Header_Shnum();
    shentsize = header.Elf_Header_Shentsize();
    size = shnum * shentsize;
    offset = header.Elf_Header_Shoff();
    elfview::Elf_Section_Header shdr(&file, offset, size);
    //
    phnum = header.Elf_Header_Phnum();
    phentsize = header.Elf_Header_Phentsize();
    size = phnum * phentsize;
    offset = header.Elf_Header_Phoff();
    elfview::Elf_Program_Header phdr(&file, offset, size);

    for (uint16_t i = 0; i < shnum; i++)
        shdr.Elf_Section_Header_Format(i);

    type = header.Elf_Header_Type();
    if (type == ET_EXEC || type == ET_DYN) {
        std::cout << "Elf segments: " << std::endl;
        for (uint16_t i = 0; i < phnum; i++) {
            start = phdr.Elf_Program_Start(i);
            end = phdr.Elf_Program_End(i);
            std::cout << '\t' << "Segment[" << i << "]:" << std::endl;
            phdr.Elf_Program_Header_Format(i);
            std::cout << '\t' << "Segment[" << i << "] contains the following sections:" << std::endl;
            for (uint16_t j = 0; j < shnum; j++) {
                uint32_t sstart = shdr.Elf_Section_Start(j);
                uint32_t send = shdr.Elf_Section_End(j);
                if (sstart >= start && send <= end) {
                    std::cout << '\t' << "Section[" << j << "]:" << std::endl;
                    shdr.Elf_Section_Header_Format(j);
                }
            }
            std::cout << '\t' << "------------------------------------------------------------------------------------------------" << std::endl;
        }
    }




    return 0;
}
