#ifndef __ELF_PROGRAM_ENTRY_H__
#define __ELF_PROGRAM_ENTRY_H__

#include <elf.h>
#include <string>
#include "Util.h"
#include "BinaryFile.h"

#define PAGE_SIZE 4096

namespace elfview
{

class Elf_Program_Entry
{
    public:
        explicit Elf_Program_Entry(Elf32_Phdr phdr)
            : phdr_(phdr)
        {}
        ~Elf_Program_Entry()
        {
        
        }
    public:
        std::string Program_Type_Format() const
        {
            std::string s;
            uint32_t type = phdr_.p_type;

            switch(type) {
                case PT_NULL:
                    s = "PT_NULL";
                    break;
                case PT_LOAD:
                    s = "PT_LOAD";
                    break;
                case PT_DYNAMIC:
                    s = "PT_DYNAMIC";
                    break;
                case PT_INTERP:
                    s = "PT_INTERP";
                    break;
                case PT_NOTE:
                    s = "PT_NOTE";
                    break;
                case PT_SHLIB:
                    s = "PT_SHLIB";
                    break;
                case PT_PHDR:
                    s = "PT_PHDR";
                    break;
                case PT_TLS:
                    s = "PT_TLS";
                    break;
                case PT_NUM:
                    s = "PT_NUM";
                    break;
                case PT_LOOS:
                    s = "PT_LOOS";
                    break;
                case PT_GNU_EH_FRAME:
                    s = "PT_GNU_EH_FRAME";
                    break;
                case PT_GNU_RELRO:
                    s = "PT_GNU_RELRO";
                    break;
                case PT_LOSUNW:
                    s = "PT_LOSUNW";
                    break;
                //case PT_SUNWBSS:
                //    s = "PT_SUNWBSS";
                //    break;
                case PT_SUNWSTACK:
                    s = "PT_SUNWSTACK";
                    break;
                case PT_HISUNW:
                    s = "PT_HISSUNW";
                    break;
                //case PT_HIOS:
                //    s = "PT_HIOS";
                //    break;
                case PT_LOPROC:
                    s = "PT_LOPROC";
                    break;
                case PT_HIPROC:
                    s = "PT_HIPROC";
                    break;
                case PT_GNU_STACK:
                    s = "PT_GNU_STACK";
                    break;
                default:
                    s = "Invalid Value";
                    break;
            }
            return s;
        }
        std::string Program_Offset_Format() const
        {
            std::string s;
            uint32_t type = phdr_.p_type;
            uint32_t vaddr = phdr_.p_vaddr;
            uint32_t offset = phdr_.p_offset;

            if (type == PT_LOAD) {
                if ((vaddr % PAGE_SIZE) != (offset % PAGE_SIZE)) {
                    s = "Error: ";
                    s += detail::ToHexDWordFormat(offset);
                    s += " (Loadable process segments must have congruent values for p_vaddr and p_offset, modulo the page size.)"; 
                    return s;
                }
            }
            return detail::ToHexDWordFormat(offset);
        
        }
        std::string Program_Vaddr_Format() const
        {
            uint32_t vaddr = phdr_.p_vaddr;
            return detail::ToHexDWordFormat(vaddr);
        }
        std::string Program_Paddr_Format() const
        {
            uint32_t paddr = phdr_.p_paddr;
            return detail::ToHexDWordFormat(paddr); 
        }
        std::string Program_Filesz_Format() const
        {
            uint32_t filesz = phdr_.p_filesz;
            return detail::ToHexDWordFormat(filesz);
        }
        std::string Program_Memsz_Format() const
        {
            uint32_t memsz = phdr_.p_memsz;
            return detail::ToHexDWordFormat(memsz);
        }
        std::string Program_Flags_Format() const
        {
            std::string s;
            uint32_t flags = phdr_.p_flags;
            if (flags | PF_X)
                s += "PF_X ";
            if (flags | PF_W)
                s += "PF_W ";
            if (flags | PF_R)
                s += "PF_R ";
            return s;
        }
        std::string Program_Align_Format() const
        {
            std::string s;
            uint32_t align = phdr_.p_align;
            uint32_t vaddr = phdr_.p_vaddr;
            uint32_t offset = phdr_.p_offset;

            s = detail::ToHexDWordFormat(align);

            if (align == 0 || align == 1) {
                s += " (not required)";
                return s;
            } else if (!detail::IsPowerTwo(align)) {
                s += " (must be power of 2)";
                return s;
            } else {
                if ((vaddr % align) != (offset % align)) {
                    s += " (p_vaddr should equal p_offset, modulo p_align)";
                    return s;
                } 
            }
            return s;
        }
    public:
        uint32_t Program_Offset() const
        {
            return phdr_.p_offset;
        }
        uint32_t Program_Filesz() const
        {
            return phdr_.p_filesz;
        }
    private:
        Elf32_Phdr phdr_;
};

}//namespace elfview 


#endif 
