#include <stdio.h>
#include <stdlib.h>
#include "tinytest.h"
#include "tinytest_macros.h"

#include "trie.hpp"
using namespace Collections;

static void
test_empty(void *arg) {
    auto t = Trie<int>();
    auto empty_key = std::vector<int>();
    std::vector<int> some_key = { 1 };
    // Trie starts empty
    tt_int_op(t.values('/').size(), ==, 0);
    // And also doesn't contain a "null" value in its root
    tt_assert(!t.contains(empty_key.begin(), empty_key.end()));
    // Nor does it contain some arbitrary key
    tt_assert(!t.contains(some_key.begin(), some_key.end()));
end:
    (void)0;
}

static void
test_insert(void *arg) {
    auto t = Trie<int>();
    std::vector<int> k1 = { 1 };
    std::vector<int> k11 = { 1, 1, 1 };
    std::vector<int> k2 = { 2 };
    std::vector<int> k3 = { 3, 3, 3 };
    std::vector<int> k33 = { 3, 3, 3, 3, 3 };
    auto all_keys = { k1, k2, k3, k33, k11 };
    // starts empty
    tt_int_op(t.values('/').size(), ==, 0);
    // insert first time and size is now 1
    t.insert(k1.begin(), k1.end());
    tt_int_op(t.values('/').size(), ==, 1);
    // insert same again and still 1
    t.insert(k1.begin(), k1.end());
    tt_int_op(t.values('/').size(), ==, 1);
    // insert another and size now 2
    t.insert(k2.begin(), k2.end());
    tt_int_op(t.values('/').size(), ==, 2);
    // insert same, still 2
    t.insert(k1.begin(), k1.end());
    t.insert(k2.begin(), k2.end());
    tt_int_op(t.values('/').size(), ==, 2);
    // insert another, more complex key and size now 3
    t.insert(k33.begin(), k33.end());
    tt_int_op(t.values('/').size(), ==, 3);
    // insert something that is decendent of an existing key
    t.insert(k11.begin(), k11.end());
    tt_int_op(t.values('/').size(), ==, 4);
    // insert something that follows but stops before the end of an existing key
    t.insert(k3.begin(), k3.end());
    tt_int_op(t.values('/').size(), ==, 5);
    // all keys exist in Trie
    for (auto k: all_keys) {
        tt_assert(t.contains(k.begin(), k.end()));
    }
end:
    (void)0;
}

static void
test_clear(void *arg) {
    auto t = Trie<int>();
    std::vector<int> k1 = { 1 };
    std::vector<int> k11 = { 1, 1, 1 };
    std::vector<int> k2 = { 2 };
    std::vector<int> k3 = { 3, 3, 3 };
    std::vector<int> k33 = { 3, 3, 3, 3, 3 };
    auto all_keys = { k1, k2, k3, k33, k11 };
    for (auto k: all_keys) {
        t.insert(k.begin(), k.end());
    }
    // starts with expected len
    tt_int_op(t.values('/').size(), ==, all_keys.size());
    t.clear();
    // ends empty
    tt_int_op(t.values('/').size(), ==, 0);
end:
    (void)0;
}

struct testcase_t trie_tests[] = {
    /* Name, function, flags, setup code, teardown code */
    { "empty", test_empty, 0, NULL, NULL },
    { "insert", test_insert, 0, NULL, NULL },
    { "clear", test_clear, 0, NULL, NULL },
    END_OF_TESTCASES
};

struct testgroup_t test_groups[] = {
    { "trie/", trie_tests },
    END_OF_GROUPS
};

int main(int argc, const char *argv[]) {
    return tinytest_main(argc, argv, test_groups);
}
