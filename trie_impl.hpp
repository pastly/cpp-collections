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
            assert((
                "End of value iter, but no self value. Was it empty, thus we're at the root?",
                this->self.has_value()
            ));
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
            assert((
                "Supposed to be impossible to have_self without a value in self.",
                this->self.has_value()
            ));
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

    template <typename T>
    void Trie<T>::clear(void) {
        for (auto kv: this->children) {
            kv.second.clear();
        }
        this->children.clear();
        // setting have_self to false should not be necessary:
        // - if we aren't the root, we're getting dropped so it doesn't matter
        // - if we are the root, we aren't allowed to have a value in self
        //this->have_self = false;
    }

    template <typename T>
    size_t Trie<T>::size(void) {
        auto size = this->have_self ? 1 : 0;
        for (auto kv: this->children) {
            size += kv.second.size();
        }
        return size;
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