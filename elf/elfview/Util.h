#ifndef __UTIL_H__
#define __UTIL_H__ 

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>

namespace elfview
{

namespace detail
{

std::string ToHexByte(uint8_t value); 
std::string ToHexByteFormat(uint8_t value);
std::string ToASCIIByte(uint8_t value);
std::string ToHexWord(uint16_t value);
std::string ToHexWordFormat(uint16_t value);
std::string ToHexDWord(uint32_t value);
std::string ToHexDWordFormat(uint32_t value);

void PrintHexFormat(std::vector<uint8_t> vec, size_t offset, size_t len);
std::string ToEmptyHexByte();
void PrintHeaderFormat();

std::string ToHeaderFormat();
std::string ToHexFormat(uint8_t* buf, uint32_t offset, uint32_t size);

bool IsPowerTwo(uint32_t value);

}//namespace detail 
}//namespace elfview

#endif
