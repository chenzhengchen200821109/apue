#ifndef __BINARYFILE_H__
#define __BINARYFILE_H__ 

#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

namespace elfview
{
    
class BinaryFile
{
    public:
        explicit BinaryFile(const char* name);
        explicit BinaryFile(const std::string& name);
        uint32_t Size() const;
        ~BinaryFile();
        std::string HexFormat(uint32_t offset, uint32_t size) const;
        uint8_t* BasePoint(uint32_t offset);
    private:
        FILE* file_;
        uint32_t size_;
        uint8_t* buf_;
};

}//namespace elfview 

#endif
