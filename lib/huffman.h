//
// Created by xpech on 13.09.18.
//

#ifndef HUFFMANCITO_HUFFMAN_H
#define HUFFMANCITO_HUFFMAN_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include "Tree.h"

class huffman {

public:

    static void encode(std::istream &is, std::ostream &os);
    static bool decode(std::istream& is, std::ostream& os);

private:
    struct BufferedWriter;
    static const uint32_t BUFFER_SIZE = 1024*1024;

};


#endif //HUFFMANCITO_HUFFMAN_H
