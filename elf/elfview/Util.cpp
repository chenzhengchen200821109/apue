#include "Util.h"

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

std::string ToHexWord(uint16_t value)
{
    std::string s;
    char hex[5] = { 0 };
    snprintf(hex, 5, "%04X", value);
    s.append(hex, 4);
    return s;
}

std::string ToHexDWORD(uint32_t value)
{
    std::string s;
    char hex[9] = { 0 };
    snprintf(hex, 9, "%08X", value);
    s.append(hex, 8);
    return s;
}

}//namespace detail 
}//namespace elfview
