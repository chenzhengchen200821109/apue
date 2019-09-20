#ifndef __ELF_SECTION_SHSTRTAB_H__
#define __ELF_SECTION_SHSTRTAB_H__

#include <elf.h>
#include <stdint.h>
#include <glog/logging.h>
#include <string>
#include "BinaryFile.h"

namespace elfview
{

class Elf_Section_Shstrtab
{
    public:
        explicit Elf_Section_Shstrtab(BinaryFile* file, uint32_t offset, uint32_t size)
            : file_(file)
            , offset_(offset)
            , size_(size)
            , buf_(0)
        {
            //CHECK( bfile_ != NULL);
            buf_ = (char *)file_->BasePoint(offset_);
            
        }
        ~Elf_Section_Shstrtab()
        {

        }
        std::string ToHexStr() const
        {
            return file_->HexFormat(offset_, size_);
        }
        std::string ToShStr(uint32_t location) const
        {
            std::string s;
            uint32_t i = location;
            char c;
            while ((c = buf_[i]) != '\0') {
                s.push_back(c);
                i++;
            }
            return s;
        }
    private:
        BinaryFile* file_;
        uint32_t offset_;
        uint32_t size_;
        char* buf_;
};

} //namespace elfview

#endif 
