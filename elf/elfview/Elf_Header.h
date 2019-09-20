#ifndef __ELF_HEADER_H__
#define __ELF_HEADER_H__ 

#include <elf.h>
#include <stddef.h>
#include <string>
#include <iostream>
#include "BinaryFile.h"
#include "Util.h"

namespace elfview 
{

class Elf_Header
{
    public:
        explicit Elf_Header(BinaryFile* file)
            : offset_(0)
            , size_(sizeof(Elf32_Ehdr))
            , file_(file) 
        {
            header_ = (Elf32_Ehdr *)file_->BasePoint(offset_);
        }
        
        ~Elf_Header()
        {
            
        }
        std::string Elf_Header_Format()
        {
            return file_->HexFormat(offset_, size_);
        }
        void Elf_Header_Summary_Format()
        {
            std::cout << '\t' << "ident: " << Elf_Header_Ident_Format() << std::endl;
            std::cout << '\t' << "type: " << Elf_Header_Type_Format();
            std::cout << '\t' << '\t' << "machine: " << Elf_Header_Machine_Format() << std::endl;

            std::cout << '\t' << "version: " << Elf_Header_Version_Format();
            std::cout << '\t' << "entry: " << Elf_Header_Entry_Format() << std::endl;

            std::cout << '\t' << "phoff: " << Elf_Header_Phoff_Format();
            std::cout << '\t' << "shoff: " << Elf_Header_Shoff_Format() << std::endl;

            std::cout << '\t' << "flags: " << Elf_Header_Flags_Format();
            std::cout << '\t' << "ehsize: " << Elf_Header_Ehsize_Format() << std::endl;

            std::cout << '\t' << "phentsize: " << Elf_Header_Phentsize_Format();
            std::cout << '\t' << "phnum: " << Elf_Header_Phnum_Format() << std::endl;

            std::cout << '\t' << "shentsize: " << Elf_Header_Shentsize_Format();
            std::cout << '\t' << "shnum: " << Elf_Header_Shnum_Format() << std::endl;

            std::cout << '\t' << "shstrndx: " << Elf_Header_Shstrndx_Format() << std::endl;
        }
        std::string Elf_Header_Ident_Format()
        {
            std::string s;
            return s;
        }
        std::string Elf_Header_Type_Format()
        {
            std::string s;
            uint16_t type = header_->e_type;
            switch (type) {
                case ET_NONE:
                    s = "ET_NONE";
                    break;
                case ET_REL:
                    s = "ET_NONE";
                    break;
                case ET_EXEC:
                    s = "ET_EXEC";
                    break;
                case ET_DYN:
                    s = "ET_DYN";
                    break;
                case ET_CORE:
                    s = "ET_CORE";
                    break;
                default:
                    s = "Invaild Value";
                    break;
            }
            return s;
        }
        std::string Elf_Header_Machine_Format()
        {
            std::string s;
            uint16_t machine = header_->e_machine;
            switch (machine) {
                case EM_NONE:
                    s = "EM_NONE";
                    break;
                case EM_M32:
                    s = "EM_M32";
                    break;
                case EM_SPARC:
                    s = "EM_SPARC";
                    break;
                case EM_386:
                    s = "EM_386";
                    break;
                case EM_68K:
                    s = "EM_68K";
                    break;
                case EM_88K:
                    s = "EM_88K";
                    break;
                case EM_860:
                    s = "EM_860";
                    break;
                case EM_MIPS:
                    s = "EM_MIPS";
                    break;
                case EM_PARISC:
                    s = "EM_PARISC";
                    break;
                case EM_SPARC32PLUS:
                    s = "EM_SPARC32PLUS";
                    break;
                case EM_PPC:
                    s = "EM_PPC";
                    break;
                case EM_PPC64:
                    s = "EM_PPC64";
                    break;
                case EM_S390:
                    s = "EM_S390";
                    break;
                case EM_ARM:
                    s = "EM_ARM";
                    break;
                case EM_SH:
                    s = "EM_SH";
                    break;
                case EM_SPARCV9:
                    s = "EM_SPARCV9";
                    break;
                case EM_IA_64:
                    s = "EM_IA_64";
                    break;
                case EM_X86_64:
                    s = "EM_X86_64";
                    break;
                case EM_VAX:
                    s = "EM_VAX";
                    break;
                default:
                    s = "Invaild Value";
                    break;
            }
            return s;
        }
        std::string Elf_Header_Version_Format() const
        {
            std::string s;
            uint32_t version = header_->e_version;
            switch (version) {
                case EV_NONE:
                    s = "EV_NONE";
                    break;
                case EV_CURRENT:
                    s = "EV_CURRENT";
                    break;
                default:
                    s = "Invaild Value";
                    break;
            }
            return s;
        }
        std::string Elf_Header_Entry_Format() const
        {
            uint32_t entry = header_->e_entry;
            return detail::ToHexDWordFormat(entry);
        }
        std::string Elf_Header_Phoff_Format() const
        {
            uint32_t phoff = header_->e_phoff;
            return detail::ToHexDWordFormat(phoff);
        }
        std::string Elf_Header_Shoff_Format() const
        {
            uint32_t shoff = header_->e_shoff;
            return detail::ToHexDWordFormat(shoff);
        }
        std::string Elf_Header_Flags_Format() const
        {
            uint32_t flags = header_->e_flags;
            return detail::ToHexDWordFormat(flags);
        }
        std::string Elf_Header_Ehsize_Format() const
        {
            uint16_t ehsize = header_->e_ehsize;
            return detail::ToHexWordFormat(ehsize);
        }
        std::string Elf_Header_Phentsize_Format() const
        {
            uint16_t phentsize = header_->e_phentsize;
            return detail::ToHexWordFormat(phentsize);
        }
        std::string Elf_Header_Phnum_Format() const
        {
            uint16_t phnum = header_->e_phnum;
            return detail::ToHexWordFormat(phnum);
        }
        std::string Elf_Header_Shentsize_Format() const
        {
            uint16_t shentsize = header_->e_shentsize;
            return detail::ToHexWordFormat(shentsize);
        }
        std::string Elf_Header_Shnum_Format() const
        {
            uint16_t shnum = header_->e_shnum;
            return detail::ToHexWordFormat(shnum);
        }
        std::string Elf_Header_Shstrndx_Format() const
        {
            uint16_t shstrndx = header_->e_shstrndx;
            return detail::ToHexWordFormat(shstrndx);
        }
    public:
        uint16_t Elf_Header_Type() const
        {
            return header_->e_type;
        }
        uint32_t Elf_Header_Phoff() const
        {
            return header_->e_phoff;
        }
        uint32_t Elf_Header_Shoff() const
        {
            return header_->e_shoff;
        }
        uint16_t Elf_Header_Ehsize() const
        {
            return header_->e_ehsize;
        }
        uint16_t Elf_Header_Phentsize() const
        {
            return header_->e_phentsize;
        }
        uint16_t Elf_Header_Phnum() const
        {
            return header_->e_phnum;
        }
        uint16_t Elf_Header_Shentsize() const
        {
            return header_->e_shentsize;
        }
        uint16_t Elf_Header_Shnum() const
        {
            return header_->e_shnum;
        }
        uint16_t Elf_Header_Shstrndx() const
        {
            return header_->e_shstrndx;
        }
    private:
        BinaryFile* file_;
        off_t offset_;
        off_t size_;
        Elf32_Ehdr* header_;
};

}//namespace elfview 

#endif
