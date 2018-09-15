//
// Created by xpech on 15.09.18.
//

#include "Tree.h"
#include <set>

typedef uint64_t ui64;

struct Tree::Node {
    char symb;
    ui64 fr;
    bool term;

    std::shared_ptr<Tree::Node> l;
    std::shared_ptr<Tree::Node> r;

    Node(char symbol, ui64 frequency, bool is_term = true, std::shared_ptr<Tree::Node> l_child = nullptr,
         std::shared_ptr<Tree::Node> r_child = nullptr) :
            symb(symbol),
            fr(frequency),
            term(is_term),
            l(std::move(l_child)),
            r(std::move(r_child)) {}

};

struct Tree::cmp {
    bool operator()(const std::shared_ptr<Tree::Node> &l, const std::shared_ptr<Tree::Node> &r) {
        return l->fr < r->fr;
    }
};


Tree::Tree(std::map<char, uint64_t> &fr) {

    _size = fr.size();

    if (_size == 0) {
        root = chk_node = nullptr;
        return;
    }

    std::multiset<std::shared_ptr<Tree::Node>, cmp> nodes;

    for (const auto &i : fr) {
        nodes.insert(std::make_shared<Tree::Node>(i.first, i.second));
    }

    std::shared_ptr<Tree::Node> first, second;
    while (nodes.size() > 1) {
        first = *nodes.begin();
        nodes.erase(nodes.begin());
        second = *nodes.begin();
        nodes.erase(nodes.begin());
        nodes.insert(std::make_shared<Tree::Node>(0, first->fr + second->fr, false, first, second));
    }
    root = chk_node = *nodes.begin();
}

void Tree::get_codes(std::map<char, std::vector<bool>> &codes) {
    std::vector<bool> code;

    if (_size == 0) {
        return;
    } else if (_size == 1) {
        codes[root->symb] = std::vector<bool>(1, 0);
        return;
    }
    _get_codes(codes, root, code);
}

void Tree::_get_codes(std::map<char, std::vector<bool>> &codes, std::shared_ptr<Tree::Node> &node,
        std::vector<bool> &code) {
    if (node->term) {
        codes[node->symb] = std::vector<bool>(code);
        return;
    }

    code.push_back(false);
    _get_codes(codes, node->l, code);
    code.pop_back();

    code.push_back(true);
    _get_codes(codes, node->r, code);
    code.pop_back();
}

bool Tree::chk_bit(char symb, char pos) {
    if (_size < 2) {
        return (symb == 0);
    }
    if (1 & (symb >> pos)) {
        chk_node = chk_node->r;
    } else {
        chk_node = chk_node->l;
    }
    return (bool)(chk_node);
}

void Tree::write_chk_symb(std::ostream &os) {
    if (chk_node->term || _size == 1) {
        os.write(&chk_node->symb, sizeof(chk_node->symb));
        chk_node = root;
    }
}