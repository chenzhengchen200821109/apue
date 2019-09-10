#include "Elf_Ident.h"
#include <stdio.h>

namespace elfview
{

std::string Elf_Ident::FileOffset() const
{
    std::string s;
    char hex[9] = { 0 };
    snprintf(hex, 9, "%08X", (uint32_t)offset_);
    s.append(hex, 8);
    return s;
}

std::string Elf_Ident::Data() const
{
    return elf_ident_str_;
}

std::string Elf_Ident::Description() const
{
    std::string desc_("Elf header ident");
    return desc_;
}

std::string Elf_Ident::Value() const
{
    return value_;
}

std::string Elf_Ident::Elf_Ident_MAGO() const 
{
    return elf_ident_str_.substr(0, 2);
}

std::string Elf_Ident::Elf_Ident_MAG1() const
{
    return elf_ident_str_.substr(1*2, 2);
}

std::string Elf_Ident::Elf_Ident_MAG2() const
{
    return elf_ident_str_.substr(2*2, 2);
}

std::string Elf_Ident::Elf_Ident_MAG3() const
{
    return elf_ident_str_.substr(3*2, 2);
}

std::string Elf_Ident::Elf_Ident_CLASS_Data() const
{
    return elf_ident_str_.substr(4*2, 2);
}

std::string Elf_Ident::Elf_Ident_CLASS_Description() const
{
    std::string s("the architecture of binary");
    return s;
}

std::string Elf_Ident::Elf_Ident_CLASS_Value() const
{
    std::string s;
    switch (elf_ident_vec_[4]) {
        case ELFCLASSNONE:
            s = "ELFCLASSNONE";
            break;
        case ELFCLASS32:
            s = "ELFCLASS32";
            break;
        case ELFCLASS64:
            s = "ELFCLASS64";
            break;
        defaut:
            s = "Unknown";
            break;
    }
    return s;
}

std::string Elf_Ident::Elf_Ident_DATA_Data() const
{
    return elf_ident_str_.substr(5*2, 2);
}

std::string Elf_Ident::Elf_Ident_DATA_Description() const
{
    std::string s("the data encoding of the processor-specific data");
    return s;
}

std::string Elf_Ident::Elf_Ident_DATA_Value() const
{
    std::string s;
    switch (elf_ident_vec_[5]) {
        case ELFDATANONE:
            s = "ELFDATANONE";
            break;
        case ELFDATA2LSB:
            s = "ELFDATA2LSB";
            break;
        case ELFDATA2MSB:
            s = "ELFDATA2MSB";
            break;
        default:
            s = "Unknown";
            break;
    }
    return s;
}

std::string Elf_Ident::Elf_Ident_VERSION_Data() const
{
    return elf_ident_str_.substr(6*2, 2);
}

std::string Elf_Ident::Elf_Ident_VERSION_Description() const
{
    std::string s("the version number of the ELF specification");
    return s;
}

std::string Elf_Ident::Elf_Ident_VERSION_Value() const
{
    std::string s;
    switch(elf_ident_vec_[6]) {
        case EV_NONE:
            s = "EV_NONE";
            break;
        case EV_CURRENT:
            s = "EV_CURRENT";
            break;
        defaut:
            s = "Unknown";
            break;
    }
    return s;
}

std::string Elf_Ident::Elf_Ident_OSABI_Data() const
{
    return elf_ident_str_.substr(7*2, 2);
}

std::string Elf_Ident::Elf_Ident_OSABI_Description() const
{
    std::string s("the operating system and ABI targeted");
    return s;
}

std::string Elf_Ident::Elf_Ident_OSABI_Value() const
{
    std::string s;
    switch(elf_ident_vec_[7]) {
        //case ELFOSABI_NONE:
        //    s = "ELFOSABI_NONE";
        //    return s;
        case ELFOSABI_SYSV:
            s = "ELFOSABI_SYSV";
            break;
        case ELFOSABI_HPUX:
            s = "ELFOSABI_HPUX";
            break;
        case ELFOSABI_NETBSD:
            s = "ELFOSABI_NETBSD";
            break;
        case ELFOSABI_LINUX:
            s = "ELFOSABI_LINUX";
            break;
        case ELFOSABI_SOLARIS:
            s = "ELFOSABI_SOLARIS";
            break;
        case ELFOSABI_IRIX:
            s = "ELFOSABI_IRIX";
            break;
        case ELFOSABI_FREEBSD:
            s = "ELFOSABI_FREEBSD";
            break;
        case ELFOSABI_TRU64:
            s = "ELFOSABI_TRU64";
            break;
        case ELFOSABI_ARM:
            s = "ELFOSABI_ARM";
            break;
        case ELFOSABI_STANDALONE:
            s = "ELFOSABI_STANDALONE";
            break;
        defaut:
            s = "Unknown";
            break;
    }
    return s;
}

std::string Elf_Ident::Elf_Ident_ABIVERSION_Data() const
{
    return elf_ident_str_.substr(8*2, 2);
}

std::string Elf_Ident::Elf_Ident_ABIVERSION_Description() const
{
    std::string s("the version of the ABI");
    return s;
}

std::string Elf_Ident::Elf_Ident_ABIVERSION_Value() const
{
    std::string s;
    switch(elf_ident_vec_[8]) {
        case 0:
            s = "0";
            break;
        defaut:
            s = "Unknown";
            break;
    }
    return s;
}

std::string Elf_Ident::Elf_Ident_PAD_Data() const
{
    return elf_ident_str_.substr(9*2, 2);
}

std::string Elf_Ident::Elf_Ident_PAD_Description() const
{
    std::string s("Start of padding");
    return s;
}

std::string Elf_Ident::Elf_Ident_PAD_Value() const
{
    return Elf_Ident_PAD_Data();
}

}//namespace elfview 
