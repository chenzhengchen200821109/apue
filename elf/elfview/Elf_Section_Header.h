#ifndef __ELF_SECTION_HEADER_H__
#define __ELF_SECTION_HEADER_H__

#include <elf.h>
#include <glog/logging.h>
#include "BinaryFile.h"
#include "Elf_Section_Entry.h"

namespace elfview
{

class Elf_Section_Header
{
    public:
        explicit Elf_Section_Header(BinaryFile* file, uint32_t offset, uint32_t size)
            : file_(file)
            , offset_(offset)
            , size_(size)
            , num_(size_ % sizeof(Elf32_Shdr))
        {
            //CHECK
            num_ = size_ / sizeof(Elf32_Shdr);
            shdr_ = (Elf32_Shdr *)file_->BasePoint(offset_); 
            //
            Elf32_Ehdr* header = (Elf32_Ehdr *)file_->BasePoint(0);
            uint16_t strtabndx = header->e_shstrndx;
            offset = (shdr_[strtabndx]).sh_offset;
            buf_ = (char *)file_->BasePoint(offset);
        }
        ~Elf_Section_Header()
        {
        
        }
    public:
        std::string Elf_Section_Header_Table_Format() const
        {
            return file_->HexFormat(offset_, size_);
        }
        //返回Section header table中某一项的格式化数据。
        void Elf_Section_Header_Format(uint32_t index)
        {
            //CHECK
            uint32_t location;
            Elf_Section_Entry entry(shdr_[index]);
            location = entry.Section_Name();
            std::cout << '\t' << '\t' << "name: " << Elf_Section_Header_Name(location);
            std::cout << '\t' << "type: " << entry.Section_Type_Format() << std::endl;
            
            std::cout << '\t' << '\t' << "flags: " << entry.Section_Flags_Format();
            std::cout << '\t' << "addr: " << entry.Section_Addr_Format() << std::endl;

            std::cout << '\t' << '\t' << "offset: " << entry.Section_Offset_Format();
            std::cout << '\t' << "size: " << entry.Section_Size_Format() << std::endl;

            std::cout << '\t' << '\t' << "link: " << entry.Section_Link_Format();
            std::cout << '\t' << "info: " << entry.Section_Info_Format() << std::endl;

            std::cout << '\t' << '\t' << "addralign: " << entry.Section_Addralign_Format();
            std::cout << '\t' << "entsize: " << entry.Section_Entsize_Format() << std::endl;
        }
        //返回某Section中的格式化数据。
        std::string Elf_Section_Format(uint32_t index) const 
        {
            //CHECK
            Elf_Section_Entry entry(shdr_[index]);
            uint32_t offset = entry.Section_Offset();
            uint32_t size = entry.Section_Size();
            return file_->HexFormat(offset, size);
        }
    public:
        uint32_t Elf_Section_Name(uint32_t index) const
        {
            //CHECK
            Elf_Section_Entry entry(shdr_[index]);
            return entry.Section_Name();
        }
        uint32_t Elf_Section_Offset(uint32_t index) const
        {
            //CHECK
            Elf_Section_Entry entry(shdr_[index]);
            return entry.Section_Offset();
        }
        uint32_t Elf_Section_Size(uint32_t index) const
        {
            //CHECK
            Elf_Section_Entry entry(shdr_[index]);
            return entry.Section_Size();
        }
    public:
        uint32_t Elf_Section_Start(uint32_t index) const
        {
            return Elf_Section_Offset(index);
        }
        uint32_t Elf_Section_End(uint32_t index) const
        {
            //CHECK
            Elf_Section_Entry entry(shdr_[index]);
            if (entry.Section_Type() == SHT_NOBITS)
                return entry.Section_Offset();
            return (entry.Section_Offset() + entry.Section_Size());
        }
    private:
        std::string Elf_Section_Header_Name(uint32_t location) const 
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
        uint32_t num_;
        Elf32_Shdr* shdr_;
        char* buf_; //shstrtab
};

}//namespace elfview



#endif 
