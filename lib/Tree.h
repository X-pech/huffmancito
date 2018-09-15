//
// Created by xpech on 15.09.18.
//

#ifndef HUFFMANCITO_TREE_H
#define HUFFMANCITO_TREE_H

#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <climits>

class Tree {
public:
    Tree(uint64_t fr[], const size_t &s);
    void get_codes(std::map<char, std::vector<bool>> &codes);
    bool chk_bit(char symb, char pos);
    void write_chk_symb(std::ostream &os);
private:
    struct Node;
    struct cmp;
    void _get_codes(std::map<char, std::vector<bool>> &codes, std::shared_ptr<Tree::Node> &node, std::vector<bool> &code);
    std::shared_ptr<Tree::Node> root, chk_node;
    size_t _size;
};


#endif //HUFFMANCITO_TREE_H
