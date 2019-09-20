#include <iostream>
#include "Util.h"
#include <glog/logging.h>

namespace elfview
{

namespace detail
{

std::string ToHexByte(uint8_t value) 
{
    std::string s;
    char hex[3] = { 0 };
    snprintf(hex, 3, "%02X", value);
    s.append(hex, 2);
    return s;
}

std::string ToHexByteFormat(uint8_t value)
{
    std::string s;
    char hex[5] = { 0 };
    snprintf(hex, 5, "0x%02X", value);
    s.append(hex, 4);
    return s;
}

std::string ToHexWord(uint16_t value)
{
    std::string s;
    char hex[5] = { 0 };
    snprintf(hex, 5, "%04X", value);
    s.append(hex, 4);
    return s;
}

std::string ToHexWordFormat(uint16_t value)
{
    std::string s;
    char hex[7] = { 0 };
    snprintf(hex, 7, "0x%04X", value);
    s.append(hex, 6);
    return s;
}

std::string ToHexDWord(uint32_t value)
{
    std::string s;
    char hex[9] = { 0 };
    snprintf(hex, 9, "%08X", value);
    s.append(hex, 8);
    return s;
}

std::string ToHexDWordFormat(uint32_t value)
{
    std::string s;
    char hex[11] = { 0 };
    snprintf(hex, 11, "0x%08X", value);
    s.append(hex, 10);
    return s;
}

// 00010203 04050607 08090a0b 0c0d0e0f
// ===================================
void PrintHeaderFormat()
{
    std::cout << "           00010203 04050607 08090a0b 0c0d0e0f" << std::endl;
    std::cout << "==============================================" << std::endl;
}

std::string ToEmptyHexByte()
{
    std::string s("  ");
    return s;
}

void PrintHexFormat(std::vector<uint8_t> vec, size_t offset, size_t len)
{
    size_t sz = vec.size();
    if (offset >= sz)
        LOG(FATAL) << "Invalid value for offset";
    if (offset + len > sz)
        LOG(FATAL) << "Invalid value for len";

    uint32_t newoffset = (offset >> 8) << 8;
    for (uint32_t i = newoffset; i < offset + len; i++) {
        if ( i < offset) {
            if (i % 16 == 0x00) {
                std::cout << ToHexDWordFormat(i) << " " << ToEmptyHexByte();
            } else if ( (i % 16 == 0x03) || (i % 16 == 0x07) || (i % 16 == 0x0b)) {
                std::cout << ToEmptyHexByte() << " ";
            } else if ( i % 16 == 0x0f) {
                std::cout << ToEmptyHexByte() << "\n";
            } else {
                std::cout << ToEmptyHexByte();
            }
        } else {
            if (i % 16 == 0x00) {
                std::cout << ToHexDWordFormat(i) << " " << ToHexByte(vec[i]);
            } else if ( (i % 16 == 0x03) || (i % 16 == 0x07) || (i % 16 == 0x0b)) {
                std::cout << ToHexByte(vec[i]) << " ";
            } else if ( i % 16 == 0x0f) {
                std::cout << ToHexByte(vec[i]) << "\n";
            } else {
                std::cout << ToHexByte(vec[i]);
            }
        }
    } 
    std::cout << std::endl;
}

// 00010203 04050607 08090a0b 0c0d0e0f
// ===================================
std::string ToHeaderFormat()
{
    std::string s("           00010203 04050607 08090a0b 0c0d0e0f");
    s += "\n";
    s += "==============================================";
    return s;
}

std::string ToHexFormat(uint8_t* buf, uint32_t offset, uint32_t size)
{
    std::string s = ToHeaderFormat();
    s += "\n";

    uint32_t newoffset = (offset >> 4) << 4;
    for (uint32_t i = newoffset; i < offset + size; i++) {
        if ( i < offset) {
            if (i % 16 == 0x00) {
                s += ToHexDWordFormat(i);
                s += " ";
                s += ToEmptyHexByte();
            } else if ( (i % 16 == 0x03) || (i % 16 == 0x07) || (i % 16 == 0x0b)) {
                s += ToEmptyHexByte();
                s += " ";
            } else if ( i % 16 == 0x0f) {
                s += ToEmptyHexByte();
                s += "\n";
            } else {
                s += ToEmptyHexByte();
            }
        } else {
            if (i % 16 == 0x00) {
                s += ToHexDWordFormat(i);
                s += " ";
                s += ToHexByte(buf[i]);
            } else if ( (i % 16 == 0x03) || (i % 16 == 0x07) || (i % 16 == 0x0b)) {
                s += ToHexByte(buf[i]);
                s += " ";
            } else if ( i % 16 == 0x0f) {
                s += ToHexByte(buf[i]);
                s += "\n";
            } else {
                s += ToHexByte(buf[i]);
            }
        }
    } 
    s += "\n";
    return s;
}

bool IsPowerTwo(uint32_t value)
{
    uint32_t i;

    if (value == 0)
        return false;
    else if (value == 1)
        return true;

    i = value;
    while (true) {
        if ((i % 2) != 0)
            break;
        else
            i = (i / 2);
    }

    if (i == 1)
        return true;
    else
        return false;

}

}//namespace detail 
}//namespace elfview
