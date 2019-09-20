#ifndef __ELF_PROGRAM_HEADER_H__
#define __ELF_PROGRAM_HEADER_H__

#include <elf.h>
#include <stdlib.h>
#include <stdint.h>
#include <glog/logging.h>
#include <iostream>
#include "BinaryFile.h"
#include "Elf_Program_Entry.h"

namespace elfview
{

//An executable or shared object file's program header table is an array
//of structures, each describing a segment or other information the system 
//needs to prepare the program for execution. An object file segment contains 
//one or more sections. Program headers are meaningful only for executable 
//and shared object files. A file specifies its own program header size with 
//the ELF header's e_phentsize and e_phnum members.
class Elf_Program_Header
{
    public:
        explicit Elf_Program_Header(BinaryFile* file, uint32_t offset, uint32_t size)
            : file_(file)
            , offset_(offset)
            , size_(size)
            , num_(size_ % sizeof(Elf32_Phdr))
        {
            //CHECK
            num_ = size_ / sizeof(Elf32_Phdr);
            phdr_ = (Elf32_Phdr *)file_->BasePoint(offset_);
        }
        ~Elf_Program_Header()
        {
        
        }
    public:
        std::string Elf_Program_Header_Table_Format()
        {
            return file_->HexFormat(offset_, size_);
        }
        //返回Program header table中某一项的格式化数据。
        void Elf_Program_Header_Format(int index)
        {
            //CHECK
            Elf_Program_Entry entry(phdr_[index]);
            std::cout << '\t' << '\t' << "type: " << entry.Program_Type_Format();
            std::cout << '\t' <<"offset: " << entry.Program_Offset_Format() << std::endl;

            std::cout << '\t' << '\t' << "vaddr: " << entry.Program_Vaddr_Format();
            std::cout << '\t' << "paddr: " << entry.Program_Paddr_Format() << std::endl;

            std::cout << '\t' << '\t' << "filesz: " << entry.Program_Filesz_Format();
            std::cout << '\t' << "memsz: " << entry.Program_Memsz_Format() << std::endl;

            std::cout << '\t' << '\t' << "flags: " << entry.Program_Flags_Format();
            std::cout << '\t' << "align: " << entry.Program_Align_Format() << std::endl;
        }
        //返回某Program中的格式化数据。
        std::string Elf_Program_Format(uint32_t index) const 
        {
            //CHECK
            Elf_Program_Entry entry(phdr_[index]);
            uint32_t offset = entry.Program_Offset();
            uint32_t size = entry.Program_Filesz();
            return file_->HexFormat(offset, size);
        }
    public:
        uint32_t Elf_Program_Start(uint32_t index) const 
        {
            //CHECK
            Elf_Program_Entry entry(phdr_[index]);
            return entry.Program_Offset();
        }
        uint32_t Elf_Program_End(uint32_t index) const
        {
            //CHECK
            Elf_Program_Entry entry(phdr_[index]);
            return (entry.Program_Offset() + entry.Program_Filesz());
        }
    private:
        BinaryFile* file_;
        uint32_t offset_;
        uint32_t size_;
        uint32_t num_; //number of entries in program header table.
        Elf32_Phdr* phdr_;
};

}//namespace elfview

#endif 
