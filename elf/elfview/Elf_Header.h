#include <elf.h>
#include <string>
#include "Elf_Ident.h"
#include "BinaryFile.h"
#include "Util.h"

namespace elfview 
{

class Elf_Header
{
    public:
        explicit Elf_Header(const char* name)
            : offset_(0)
            , size_(sizeof(Elf32_Ehdr))
            , file_(name) 
        {
            file_.RawBytes(0, size_, (uint8_t *)&elf_header_);
            elf_header_str_ = file.RawBytes(0, size_);

        }
        explicit Elf_Header(const std::string& name)
            : Elf_Header(name.c_str())
        {
        
        }
        ~Elf_Header()
        {
            
        }
        std::string FileOffset() const
        {
            return detail::ToHexWord(offset_);
        }
        std::string Data() const
        {
            return elf_header_str_;
        }
        std::string Description() const;
        std::string Value() const;
    public:
        std::string Elf_Header_Type_FileOffset() const;
        std::string Elf_Header_Type_Data() const
        {
            uint16_t type = elf_header_.e_type;
            return detail::ToHexByte(type);
        }
        std::string Elf_Header_Type_Description() const;
        std::string Elf_Header_Type_Value() const
        {
            std::string s;
            uint16_t type = elf_header_.e_type;

            switch (type) {
                case ET_NONE:
                    s = "ET_NONE";
                    break;
                case ET_REL:
                    s = "ET_REL";
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
                    s = "Invalid Value";
                    break;
            }
            return s;
        }
    public:
        std::string Elf_Header_Machine_FileOffset() const;
        std::string Elf_Header_Machine_Data() const
        {
            uint16_t machine = elf_header_.e_machine;
            return detail::ToHexWord(machine);
        }
        std::string Elf_Header_Machine_Description() const;
        std::string Elf_Header_Machine_Value() const
        {
            std::string s;
            uint16_t machine = elf_header_.e_machine;

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
                deault:
                    s = "Invaild Value";
                    break;
            }
            return s;
        }
    public:
        std::string Elf_Header_Version_FileOffset() const;
        std::string Elf_Header_Version_Data() const;
        std::string Elf_Header_Version_Description() const;
        std::string Elf_Header_Version_Value() const;
    public:
        std::string Elf_Header_Entry_FileOffset() const;
        std::string Elf_Header_Entry_Data() const;
        std::string Elf_Header_Entry_Description() const;
        std::string Elf_Header_Entry_Value() const;
    public:
        std::string Elf_Header_PHOFF_FileOffset() const;
        std::string Elf_Header_PHOFF_Data() const;
        std::string Elf_Header_PHOFF_Description() const;
        std::string Elf_Header_PHOFF_Value() const;
    public:
        std::string Elf_Header_SHOFF_FileOffset() const;
        std::string Elf_Header_SHOFF_Data() const;
        std::string Elf_Header_SHOFF_Description() const;
        std::string Elf_Header_SHOFF_Value() const;
    public:
        std::string Elf_Header_FLAGS_FileOffset() const;
        std::string Elf_Header_FLAGS_Data() const;
        std::string Elf_Header_FLAGS_Description() const;
        std::string Elf_Header_FLAGS_Value() const;
    public:
        std::string Elf_Header_EHSIZE_FileOffset() const;
        std::string Elf_Header_EHSIZE_Data() const;
        std::string Elf_Header_EHSIZE_Description() const;
        std::string Elf_Header_EHSIZE_Value() const;
    public:
        std::string Elf_Header_PHENTSIZE_FileOffset() const;
        std::string Elf_Header_PHENTSIZE_Data() const;
        std::string Elf_Header_PHENTSIZE_Description() const;
        std::string Elf_Header_PHENTSIZE_Value() const;
    public:
        std::string Elf_Header_PHNUM_FileOffset() const;
        std::string Elf_Header_PHNUM_Data() const;
        std::string Elf_Header_PHNUM_Description() const;
        std::string Elf_Header_PHNUM_Value() const;
    public:
        std::string Elf_Header_SHENTSIZE_FileOffset() const;
        std::string Elf_Header_SHENTSIZE_Data() const;
        std::string Elf_Header_SHENTSIZE_Description() const;
        std::string Elf_Header_SHENTSIZE_Value() const;
    public:
        std::string Elf_Header_SHNUM_FileOffset() const;
        std::string Elf_Header_SHNUM_Data() const;
        std::string Elf_Header_SHNUM_Description() const;
        std::string Elf_Header_SHNUM_Value() const;
    public:
        std::string Elf_Header_SHSTRNDX_FileOffset() const;
        std::string Elf_Header_SHSTRNDX_Data() const;
        std::string Elf_Header_SHSTRNDX_Description() const;
        std::string Elf_Header_SHSTRNDX_Value() const;
    private:
        Elf32_Ehdr elf_header_;
        std::string elf_header_str_;
        std::vector<uint8_t> elf_header_vec_;
        off_t offset_;
        off_t size_;
        BinaryFile file_;
};

}//namespace elfview 
