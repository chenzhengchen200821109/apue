#include "BinaryFile.h"
#include "Util.h"
#include <glog/logging.h>
#include <stdio.h>

namespace elfview
{

BinaryFile::BinaryFile(const char* name)
    : file_(fopen(name, "rb"))
    , size_(0)
    , buf_(0)
{
    struct stat st;

    if (!file_)
        LOG(FATAL) << "fopen() failed";
    if (stat(name, &st) == -1)
        LOG(FATAL) << "stat() failed";
    size_ = st.st_size;
    if ((buf_ = (uint8_t *)calloc(size_, 1)) == NULL)
        LOG(FATAL) << "calloc() failed";
    if (fseek(file_, 0, SEEK_SET) == -1)
        LOG(FATAL) << "fseek() failed";
    if (fread(buf_, 1, size_, file_) != size_)
        LOG(FATAL) << "fread() failed";
}

BinaryFile::BinaryFile(const std::string& name)
    : BinaryFile(name.c_str())
{

}

BinaryFile::~BinaryFile()
{
    fclose(file_);
    free(buf_);
}

uint32_t BinaryFile::Size() const
{
    return size_;
}

std::string BinaryFile::HexFormat(uint32_t offset, uint32_t size) const
{
    return detail::ToHexFormat(buf_, offset, size);
}

uint8_t* BinaryFile::BasePoint(uint32_t offset)
{
    return (uint8_t *)(buf_ + offset);
}

}//namespace elfview
