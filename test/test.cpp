//
// Created by sorokin on 14.88.2012.
//

#include <iostream>
#include <climits>
#include "gtest/gtest.h"
#include "huffman.h"


namespace {
    std::stringstream cycle_coding(std::stringstream &in) {
        std::stringstream enc, dec;
        huffman::encode(in, enc);
        huffman::decode(enc, dec);
        return dec;
    }
}

TEST(correctness, empty_string) {
    std::stringstream in("");
    EXPECT_EQ(in.str(), cycle_coding(in).str());
}

TEST(correctness, one_char) {
    std::stringstream in("?");
    EXPECT_EQ(in.str(), cycle_coding(in).str());
}

TEST(correctness, sentence) {
    std::stringstream in("The quick brown fox jumps over lazy dog.");
    EXPECT_EQ(in.str(), cycle_coding(in).str());
}

TEST(correctness, all_chars) {
    std::stringstream in;
    for (auto i = CHAR_MIN; i <= CHAR_MAX; ++i){
        in >> i;
    }

    EXPECT_EQ(in.str(), cycle_coding(in).str());
}

TEST(correctness, russian) {
    std::stringstream in("Съешь еще этих мягких французских булок, да выпей чаю.");
    EXPECT_EQ(in.str(), cycle_coding(in).str());
}

TEST(correctness, big_file) {
    std::stringstream in;
    char regular = '%';
    for (auto i = 0; i < 1024 * 64; ++i) {
        in << regular;
        regular += i;
    }

    EXPECT_EQ(in.str(), cycle_coding(in).str());
}

TEST(correctness, corrupted_file) {
    std::stringstream enc;
    std::stringstream dec;
    enc << "#$36!#%!@&$*!@#4";
    EXPECT_EQ(false, huffman::decode(enc, dec));
}