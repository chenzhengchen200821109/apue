#include <string>
#include <vector>
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
        off_t Size() const;
        std::string HexBytes(off_t offset, size_t size) const;
        std::vector<uint8_t> RawBytes(off_t offset, size_t size) const;
        void RawBytes(off_t offset, size_t size, uint8_t* buf);
        ~BinaryFile();
        std::string ToStringHex() const;
    private:
        //std::string hexStr_;
        FILE* file_;
        off_t size_;
};


}//namespace elfview 
