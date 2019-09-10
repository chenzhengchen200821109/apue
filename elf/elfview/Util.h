#include <stdio.h>
#include <stdint.h>
#include <string>

namespace elfview
{

namespace detail
{

std::string ToHexByte(uint8_t value); 
std::string ToHexWord(uint16_t value);
std::string ToHexDWord(uint32_t value);

}//namespace detail 

}//namespace elfview
