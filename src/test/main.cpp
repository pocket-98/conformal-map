/**
 * @file test/main.cpp
 * @author Pavan Dayal
 */
#include "test.hpp"
#include "test_util.hpp"

std::string indent = " * ";

int main() {
    int fails = 0;
    int total = 0;
    int f, t;

    std::cerr << "testing util.hpp:" << std::endl;
    f = test_util::run_all();
    t = test_util::NUM_TESTS;
    std::cerr << "passed (" << t - f << "/" << t << ")" << std::endl;
    fails += f;
    total += t;

    return f;
}
