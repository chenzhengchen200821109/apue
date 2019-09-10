#include "BinaryFile.h"
#include <glog/logging.h>
#include <stdio.h>

namespace elfview
{

BinaryFile::BinaryFile(const char* name)
    : file_(fopen(name, "rb"))
{
    struct stat st;
    uint8_t *p = NULL;

    if (!file_)
        LOG(FATAL) << "fopen() failed";
    if (stat(name, &st) == -1)
        LOG(FATAL) << "stat() failed";
    size_ = st.st_size; 
}

BinaryFile::BinaryFile(const std::string& name)
    : BinaryFile(name.c_str())
{

}

BinaryFile::~BinaryFile()
{
    fclose(file_);
}

off_t BinaryFile::Size() const
{
    return size_;
}

std::string BinaryFile::HexBytes(off_t offset, size_t size) const
{
    CHECK(offset <= size_);
    std::string s;
    uint8_t* p;

    if (fseek(file_, offset, SEEK_SET) == -1)
        LOG(FATAL) << "fseek() failed";
    if ((p = (uint8_t *)calloc(size, 1)) == NULL)
        LOG(FATAL) << "calloc() failed";

    if (fread(p, 1, size, file_) != size)
        LOG(FATAL) << "fread() failed";

    for (off_t i = 0; i < size; i++) {
        char hex[3] = { 0 };
        snprintf(hex, 3, "%02X", p[i]);
        s.append(hex, 2);
    }
    return s;
}

std::vector<uint8_t> BinaryFile::RawBytes(off_t offset, size_t size) const
{
    
    CHECK(offset <= size_);
    std::vector<uint8_t> v;
    uint8_t* p;

    if (fseek(file_, offset, SEEK_SET) == -1)
        LOG(FATAL) << "fseek() failed";
    if ((p = (uint8_t *)calloc(size, 1)) == NULL)
        LOG(FATAL) << "calloc() failed";

    if (fread(p, 1, size, file_) != size)
        LOG(FATAL) << "fread() failed";

    for (off_t i = 0; i < size; i++) {
        v.push_back(p[i]);
    }
    return v;
}

void BinaryFile::RawBytes(off_t offset, size_t size, uint8_t* buf)
{

    CHECK(offset <= size_);

    if (fseek(file_, offset, SEEK_SET) == -1)
        LOG(FATAL) << "fseek() failed";

    if (fread(buf, 1, size, file_) != size)
        LOG(FATAL) << "fread() failed";
}

std::string BinaryFile::ToStringHex() const
{
    return HexBytes(0, size_);
}

}//namespace elfview
