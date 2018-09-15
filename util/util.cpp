//
// Created by sorokin on 13.03.18.
//

#include <iostream>
#include <cstdlib>
#include <fstream>

#include "huffman.h"


void print_and_exit(const char* &s) {
    std::cout << s << std::endl;
    exit(0);
}

const char* usage = "usage ./huffman (-e | -d) [src] [destination]";
const char* opening = "Cannot open the files";
const char* decoding = "Cannot decode the src file";

int main(int argc, char* argv[]) {

    if (argc < 4) {
        print_and_exit(usage);
    }

    auto mode = std::string(argv[1]);
    auto src = argv[2];
    auto dest = argv[3];

    std::ifstream is(src, std::ifstream::binary);
    std::ofstream os(dest, std::ifstream::binary);

    if (!is.is_open() || !os.is_open()) {
        print_and_exit(opening);
    }

    if (mode == "-e") {
        huffman::encode(is, os);
    } else if (mode == "-d") {
        if (!huffman::decode(is, os)) {
            print_and_exit(decoding);
        }
    } else {
        print_and_exit(usage);
    }

    return 0;
}