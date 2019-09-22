#include "Elf_Ident.h"
#include "Util.h"
#include <stdio.h>

namespace elfview
{

std::string Elf_Ident::Elf_Ident_MAGO() const 
{
    return detail::ToHexByte(ident_[0]);
}

std::string Elf_Ident::Elf_Ident_MAG1() const
{
    return detail::ToASCIIByte(ident_[1]); 
}

std::string Elf_Ident::Elf_Ident_MAG2() const
{
    return detail::ToASCIIByte(ident_[2]);
}

std::string Elf_Ident::Elf_Ident_MAG3() const
{
    return detail::ToASCIIByte(ident_[3]);
}

std::string Elf_Ident::Elf_Ident_CLASS() const
{
    std::string s;
    switch (ident_[4]) {
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

std::string Elf_Ident::Elf_Ident_DATA() const
{
    std::string s;
    switch (ident_[5]) {
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

std::string Elf_Ident::Elf_Ident_VERSION() const
{
    std::string s;
    switch(ident_[6]) {
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

std::string Elf_Ident::Elf_Ident_OSABI() const
{
    std::string s;
    switch(ident_[7]) {
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

std::string Elf_Ident::Elf_Ident_ABIVERSION() const
{
    std::string s;
    switch(ident_[8]) {
        case 0:
            s = "0";
            break;
        defaut:
            s = "Unknown";
            break;
    }
    return s;
}

std::string Elf_Ident::Elf_Ident_PAD() const
{
    std::string s;
    for (uint32_t i = 9; i < size_; i++) {
        s += detail::ToASCIIByte(ident_[i]);
    } 
    return s;
}

}//namespace elfview 
