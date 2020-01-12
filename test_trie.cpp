#include <stdio.h>
#include <stdlib.h>
#include "tinytest.h"
#include "tinytest_macros.h"

static void test_foo(void *dummy_arg) {
    tt_assert(1);
    tt_assert(2);
end:
    (void)0;
}

struct testcase_t trie_tests[] = {
    /* Name, function, flags, setup code, teardown code */
    { "foo", test_foo, 0, NULL, NULL },
    END_OF_TESTCASES
};

struct testgroup_t test_groups[] = {
    { "trie/", trie_tests },
    END_OF_GROUPS
};

int main(int argc, const char *argv[]) {
    return tinytest_main(argc, argv, test_groups);
}
