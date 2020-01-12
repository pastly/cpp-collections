#include "trie.hpp"
#include <iostream>
#include <iterator>
#include <sstream>
#include <cassert>
using std::map;
using std::vector;
using std::string;
using std::optional;

namespace Collections {
    template <typename T>
    Trie<T>::Trie(T self_) {
        this->self = self_;
        this->have_self = false;
        this->children = {};
    }

    template <typename T>
    Trie<T>::Trie() {
        this->self = {};
        this->have_self = false;
        this->children = {};
    }

    template <typename T>
    template <typename Iter>
    void Trie<T>::insert(Iter it, Iter end) {
        // No more items in the iter. So we are done and mark ourselves as existing.
        if (it == end) {
            this->have_self = true;
            return;
        }
        // More items in the iter. Need to go recursive. First insert a child if necessary
        if (!this->children.count(*it)) {
            auto t = Trie(*it);
            this->children.insert({*it, t});
        }
        Trie<T> &t = this->children.at(*it);
        t.insert(++it, end);
        return;
    }

    template <typename T>
    template <typename Join>
    vector<string> Trie<T>::values(Join join) {
        auto v = this->values(join, "");
        v.shrink_to_fit();
        return v;
    }

    template <typename T>
    template <typename Join>
    vector<string> Trie<T>::values(Join join, string prefix) {
        vector<string> vals;
        std::ostringstream ss;
        // this will be true for all but the root node
        if (this->self.has_value()) {
            ss << prefix << this->self.value();
        }
        if (this->have_self) {
            assert(this->self.has_value());
            vals.push_back(ss.str());
        }
        ss << join;
        for (auto kv: this->children) {
            Trie<T> child = kv.second;
            for (auto val: child.values(join, ss.str())) {
                vals.push_back(val);
            }
        }
        return vals;
    }

    template <typename T>
    template <typename Iter>
    bool Trie<T>::contains(Iter it, Iter end) {
        // if we're at the end of the iter, we're done and return whether or not
        // this node is contained in the Trie
        if (it == end) {
            return this->have_self;
        }
        auto next = *it;
        // if we don't have a child with the next part of the iter, we know we are done
        if (!this->children.count(next)) {
            return false;
        }
        // otherwise go recursive
        return this->children.at(next).contains(++it, end);
    }

    vector<string> tokenize(string s, char delim) {
        size_t start;
        size_t end = 0;
        auto out = vector<string>();
        while ((start = s.find_first_not_of(delim, end)) != string::npos) {
            end = s.find(delim, start);
            out.push_back(s.substr(start, end - start));
        }
        return out;
    }
}

/*
#include <unistd.h>
int main(int argc, char *argv[]) {
    // Read file names from stdin, one per line, store them in the Trie, and then print them
    auto trie = Trie<string>();
    for (string line; std::getline(std::cin, line); ) {
        auto parts = tokenize(line, '/');
        trie.insert(parts.begin(), parts.end());
    }
    //for (auto v: trie.values("/")) {
    ///    std::cout << v << std::endl;
    //}
    auto k1 = tokenize("./.git/refs/tags", '/');
    std::cout << trie.contains(k1.begin(), k1.end()) << std::endl;
    auto k2 = tokenize("./.git/reffs/tags", '/');
    std::cout << trie.contains(k2.begin(), k2.end()) << std::endl;
}
*/