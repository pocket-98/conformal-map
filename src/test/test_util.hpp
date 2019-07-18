/**
 * @file test/test_util.hpp
 * @author Pavan Dayal
 */
#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include "test.hpp"
#include "../util.hpp"

namespace test_util {
    const int NUM_TESTS = 3;
    /* return 0 for success and 1 for failure */
    int test_mean();
    int test_var();
    int test_std();
    /* returns number of failures */
    int run_all();
}

#endif /* TEST_UTIL_H */
