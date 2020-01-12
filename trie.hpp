#ifndef TRIE_HPP
#define TRIE_HPP
#include <optional>
#include <vector>
#include <string>
#include <map>

namespace Collections {
    template <typename T>
    class Trie {
        // The contents of this node in the Trie. Only the root not will not have a value.
        std::optional<T> self;
        // Whether or not this node terminates an actual entry in the Trie.
        // Inserting /1/2/3 and /1/2/4 will insert 4 total nodes. /1 and /1/2 have_self=false
        // while /1/2/3 and /1/2/4 have_self=true
        bool have_self;
        // Child nodes, obviously. Can be empty.
        std::map<T, Trie<T>> children;
    private:
        // Helper for public values function.
        // Join argument is the separator between
        // parents and children when concatenating them together. E.g. for file
        // systems this would be "/".
        // string argument is what makes this the helper: it's the concatenation so
        // far in the iteration over the contained values.
        template <typename Join>
        std::vector<std::string> values(Join, std::string);
        // Private constructor of Trie with a self value. Note it starts with have_self=false
        Trie(T);
    public:
        // Insert into the Trie from an iterator over T values forming the logical key.
        // Upon exhasting the iterator, the final Trie is marked with have_self=true
        template <typename Iter>
        void insert(Iter, Iter);
        // Walk through the Trie collecting all stored values, concatenating with the
        // given separator.
        template <typename Join>
        std::vector<std::string> values(Join);
        // Check if the given value -- specified by an iterator over its parts -- is
        // contained in the Trie
        template <typename Iter>
        bool contains(Iter, Iter);
        // Remove all contents from the Trie
        void clear(void);
        // Get the number of items contained in the Trie
        size_t size(void);
        // Public constructor. Used to create root node with no self.
        Trie();
    };
}

#include "trie_impl.hpp"
#endif