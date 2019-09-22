#include <elf.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <string.h>
#include "BinaryFile.h"

namespace elfview
{

class Elf_Ident
{
    public:
        explicit Elf_Ident(BinaryFile* file)
            : file_(file)
            , offset_(0)
            , size_(EI_NIDENT)
        {
            uint8_t* src = file_->BasePoint(0);
            memcpy(ident_, src, size_);
        }
        ~Elf_Ident()
        {
        
        }
    public:
        std::string Elf_Ident_MAGO() const;
        std::string Elf_Ident_MAG1() const;
        std::string Elf_Ident_MAG2() const;
        std::string Elf_Ident_MAG3() const;
        std::string Elf_Ident_CLASS() const;
        std::string Elf_Ident_DATA() const;
        std::string Elf_Ident_VERSION() const;
        std::string Elf_Ident_OSABI() const;
        std::string Elf_Ident_ABIVERSION() const;
        std::string Elf_Ident_PAD() const;
    private:
        BinaryFile* file_;
        uint32_t offset_;
        uint32_t size_;
        uint8_t ident_[EI_NIDENT];
};

}//namespace elfview 
