#include <iostream>
#include <glog/logging.h>
#include "BinaryFile.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    //Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);

    if (argc != 2) {
        fprintf(stderr, "Usage: ./prog file\n");
        return -1;
    }

    elfview::BinaryFile file(argv[1]);
    std::cout << file.Size() << std::endl;

    std::cout << file.ToStringHex() << std::endl;

    return 0;
}
