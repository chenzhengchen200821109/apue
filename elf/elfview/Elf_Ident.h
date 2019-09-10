#include <elf.h>
#include <string>
#include <vector>
#include <stdint.h>
#include "BinaryFile.h"

namespace elfview
{

class Elf_Ident
{
    public:
        explicit Elf_Ident(const char* name)
            : offset_(0)
            , size_(EI_NIDENT)
            , file_(name)
        {
            elf_ident_str_ = file_.HexBytes(offset_, size_);
            elf_ident_vec_ = file_.RawBytes(offset_, size_);
        }
        explicit Elf_Ident(const std::string& name)
            : Elf_Ident(name.c_str())
        {
        
        }
        ~Elf_Ident()
        {
        
        }
        std::string FileOffset() const;
        std::string Data() const;
        std::string Description() const;
        std::string Value() const;
    public:
        std::string Elf_Ident_MAGO() const;
        std::string Elf_Ident_MAG1() const;
        std::string Elf_Ident_MAG2() const;
        std::string Elf_Ident_MAG3() const;
        std::string Elf_Ident_CLASS_Data() const;
        std::string Elf_Ident_CLASS_Description() const;
        std::string Elf_Ident_CLASS_Value() const;
        std::string Elf_Ident_DATA_Data() const;
        std::string Elf_Ident_DATA_Description() const;
        std::string Elf_Ident_DATA_Value() const;
        std::string Elf_Ident_VERSION_Data() const;
        std::string Elf_Ident_VERSION_Description() const;
        std::string Elf_Ident_VERSION_Value() const;
        std::string Elf_Ident_OSABI_Data() const;
        std::string Elf_Ident_OSABI_Description() const;
        std::string Elf_Ident_OSABI_Value() const;
        std::string Elf_Ident_ABIVERSION_Data() const;
        std::string Elf_Ident_ABIVERSION_Description() const;
        std::string Elf_Ident_ABIVERSION_Value() const;
        std::string Elf_Ident_PAD_Data() const;
        std::string Elf_Ident_PAD_Description() const;
        std::string Elf_Ident_PAD_Value() const;
    private:
        std::string elf_ident_str_;
        std::vector<uint8_t> elf_ident_vec_;
        off_t offset_;
        off_t size_;
        BinaryFile file_;
        std::string desc_;
        std::string value_;
};

}//namespace elfview 
