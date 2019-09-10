#include <iostream>
#include "Elf_Ident.h"
#include <glog/logging.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    //Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);
    if (argc != 2) {
        fprintf(stderr, "Usage: ./prog file\n");
        return -1;
    }
    
    elfview::Elf_Ident ident(argv[1]);

    std::cout << ident.FileOffset() << std::endl;
    std::cout << ident.Data() << std::endl;
    std::cout << ident.Description() << std::endl;
    std::cout << ident.Value() << std::endl;

    std::cout << ident.Elf_Ident_MAGO() << std::endl;
    std::cout << ident.Elf_Ident_MAG1() << std::endl;
    std::cout << ident.Elf_Ident_OSABI_Data() << std::endl;
    std::cout << ident.Elf_Ident_OSABI_Value() << std::endl;

    return 0;
}
