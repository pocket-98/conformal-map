/**
 * @file test/test_tree.hpp
 * @author Pavan Dayal
 */
#ifndef TEST_TREE_H
#define TEST_TREE_H

#include "test.hpp"
#include "../tree.hpp"
#include <iostream>

namespace test_tree {
    const int NUM_TESTS = 3;
    /* return 0 for success and 1 for failure */
    int test_create();
    int test_print();
    int test_subdivide();
    /* returns number of failures */
    int run_all();
}

#endif /* TEST_TREE_H */
