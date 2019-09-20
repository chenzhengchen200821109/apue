#ifndef __ELF_SECTION_ENTRY_H__
#define __ELF_SECTION_ENTRY_H__

#include <elf.h>
#include <string>
#include "Util.h"
#include "BinaryFile.h"

namespace elfview
{

//entry in elf section header table 
class Elf_Section_Entry
{
    public:
        explicit Elf_Section_Entry(Elf32_Shdr shdr)
            : shdr_(shdr)
        {}
        ~Elf_Section_Entry()
        {
        
        }
    public:
        std::string Section_Name_Format() const
        {
            uint32_t name = shdr_.sh_name;
            return detail::ToHexDWordFormat(name);
        }
        std::string Section_Type_Format() const
        {
            std::string s;
            uint32_t type = shdr_.sh_type;

            switch (type) {
                case SHT_NULL:
                    s = "SHT_NULL";
                    break;
                case SHT_PROGBITS:
                    s = "SHT_PROGBITS";
                    break;
                case SHT_SYMTAB:
                    s = "SHT_SYMTAB";
                    break;
                case SHT_STRTAB:
                    s = "SHT_STRTAB";
                    break;
                case SHT_RELA:
                    s = "SHT_RELA";
                    break;
                case SHT_HASH:
                    s = "SHT_HASH";
                    break;
                case SHT_DYNAMIC:
                    s = "SHT_DYNAMIC";
                    break;
                case SHT_NOTE:
                    s = "SHT_NOTE";
                    break;
                case SHT_NOBITS:
                    s = "SHT_NOBITS";
                    break;
                case SHT_REL:
                    s = "SHT_REL";
                    break;
                case SHT_SHLIB:
                    s = "SHT_SHLIB";
                    break;
                case SHT_DYNSYM:
                    s = "SHT_DYNSYM";
                    break;
                case SHT_LOPROC:
                    s = "SHT_LOPROC";
                    break;
                case SHT_HIPROC:
                    s = "SHT_HIPROC";
                    break;
                case SHT_LOUSER:
                    s = "SHT_LOUSER";
                    break;
                case SHT_HIUSER:
                    s = "SHT_HIUSER";
                    break;
                default:
                    s = "Invaild Value";
                    break;
            }
            return s;
        }
        std::string Section_Flags_Format() const
        {
            std::string s;
            uint32_t flags = shdr_.sh_flags;

            if (flags | SHF_WRITE)
                s += "SHF_WRITE ";
            if (flags | SHF_ALLOC)
                s += "SHF_ALLOC ";
            if (flags | SHF_EXECINSTR)
                s += "SHF_EXECINSTR ";
            if (flags | SHF_MASKPROC)
                s += "SHF_MASKPROC ";
            return s;
        }

        std::string Section_Addr_Format() const
        {
            uint32_t addr = shdr_.sh_addr;
            return detail::ToHexDWordFormat(addr);
        }

        std::string Section_Offset_Format() const
        {
            uint32_t offset = shdr_.sh_offset;
            return detail::ToHexDWordFormat(offset);
        }

        std::string Section_Size_Format() const
        {
            uint32_t size = shdr_.sh_size;
            return detail::ToHexDWordFormat(size);
        }

        std::string Section_Link_Format() const
        {
            uint32_t link = shdr_.sh_link;
            return detail::ToHexDWordFormat(link);
        }

        std::string Section_Info_Format() const
        {
            uint32_t info = shdr_.sh_info;
            return detail::ToHexDWordFormat(info);
        }

        std::string Section_Addralign_Format() const
        {
            uint32_t addralign = shdr_.sh_addralign;
            return detail::ToHexDWordFormat(addralign);
        }

        std::string Section_Entsize_Format() const
        {
            uint32_t entsize = shdr_.sh_entsize;
            return detail::ToHexDWordFormat(entsize);
        }
    public:
        uint32_t Section_Type() const
        {
            return shdr_.sh_type;
        }
        uint32_t Section_Name() const
        {
            return shdr_.sh_name;
        }

        uint32_t Section_Offset() const
        {
            return shdr_.sh_offset;
        }

        uint32_t Section_Size() const
        {
            return shdr_.sh_size;
        }

        uint32_t Section_Addr() const
        {
            return shdr_.sh_addr;
        }
    private:
        Elf32_Shdr shdr_;
};

}//namespace elfview 

#endif 
