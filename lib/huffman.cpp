//
// Created by sorokin on 13.03.18.
//

#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <set>
#include <climits>

#include "huffman.h"

typedef uint64_t ui64;
typedef uint32_t ui;


struct huffman::BufferedWriter {
    std::ostream &os;
    char buffer, cur_cnt;
    const static char BUFFER_SIZE = 8;

    explicit BufferedWriter(std::ostream &os) :
            os(os),
            buffer(0),
            cur_cnt(0) {}

    void feed(const std::vector<bool> &code) {
        for (auto const &i : code) {
            buffer += i << cur_cnt;
            cur_cnt++;
            if (cur_cnt == BUFFER_SIZE) {
                os.write(&buffer, sizeof(buffer));
                cur_cnt = 0;
                buffer = 0;
            }
        }
    }

    void end_chk() {
        if (cur_cnt) {
            cur_cnt = BUFFER_SIZE - cur_cnt;
            os.write(&buffer, sizeof(buffer));
        }
        os.seekp(0);
        os.write(&cur_cnt, sizeof(cur_cnt));
    }

};


void huffman::encode(std::istream &is, std::ostream &os) {


    /*
     * Structure of the output file:
     * ZERO (for zeroes count)
     * Symbols amount
     * Frequency dict
     * Encoded data
     * */

    //std::map<char, ui64> fr;
    ui64 fr[256];
    std::fill(fr, fr + 256, 0);
    char x;
    size_t symb_amount = 0;
    while (is.peek() != std::istream::traits_type::eof()) {
        is.read(&x, sizeof(char));
        if (!fr[x + 128]) {
            symb_amount++;
        }
        fr[x + 128]++;
    }
    char buffer[BUFFER_SIZE];
    is.seekg(is.beg);
    char zero = 0;
    os.write(&zero, sizeof(char)); // ZERO

    huffman::BufferedWriter bwriter(os);
    os.write(reinterpret_cast<char *>(&symb_amount), sizeof(symb_amount)); // Symbols amount

    char k;
    ui64 v;
    //for (const auto &i : fr) {
    for (auto i = CHAR_MIN; i <= CHAR_MAX; i++) {
        if (fr[128 + i] <= 0) {
            continue;
        }
        k = (char)i;
        v = fr[128 + i];
        os.write(&k, sizeof(k)); // FRD key
        os.write(reinterpret_cast<char *>(&v), sizeof(v)); // FRD val
    }

    Tree tree(fr, symb_amount);
    std::map<char, std::vector<bool>> codes;
    tree.get_codes(codes);


    size_t size;
    while (is) {
        is.read(buffer, BUFFER_SIZE);
        size = static_cast<size_t>(is.gcount());
        for (size_t i = 0; i < size; i++) {
            bwriter.feed(codes[buffer[i]]); // data
        }
     }

    bwriter.end_chk(); // write the tail and zeroes count
}


bool huffman::decode(std::istream &is, std::ostream &os) {
    char zeroes_cnt;
    is.read(&zeroes_cnt, sizeof zeroes_cnt);
    if (!is) {
        return false;
    }

    //std::map<char, ui64> fr;
    ui64 fr[256];
    std::fill(fr, fr + 256, 0);
    size_t symb_amount;
    is.read(reinterpret_cast<char *>(&symb_amount), sizeof(symb_amount));

    if (symb_amount == 0) {
        return true;
    }

    char k;
    ui64 v;
    for (size_t i = 0; i < symb_amount; i++) {
        is.read(&k, sizeof k);
        if (!is) {
            return false;
        }
        is.read(reinterpret_cast<char *>(&v), sizeof v);
        if (!is) {
            return false;
        }
        fr[k + 128] = v;
    }

    Tree tree(fr, symb_amount);

    char buffer[BUFFER_SIZE];
    const char out_block_size = 8;


    while (is) {
        is.read(buffer, BUFFER_SIZE);
        auto size = static_cast<size_t>(is.gcount());
        if (!size) break;

        char symb;

        for (size_t i = 0; i < size - !(bool) (is); i++) {
            symb = buffer[i];
            for (char j = 0; j < out_block_size; j++) {
                if (tree.chk_bit(symb, j)) {
                    tree.write_chk_symb(os);
                } else {
                    return false;
                }
            }
        }

        if (!is) {
            symb = buffer[size - 1];
            for (char j = 0; j < out_block_size - zeroes_cnt; j++) {
                if (tree.chk_bit(symb, j)) {
                    tree.write_chk_symb(os);
                } else {
                    return false;
                }
            }
        }
    }

    return true;
}

