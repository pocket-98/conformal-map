/**
 * @file test/main.cpp
 * @author Pavan Dayal
 */
#include "test_tree.hpp"

std::string indent = " * ";

int main() {
    int fails = 0;
    int total = 0;
    int f, t;

    std::cerr << "testing tree.hpp:" << std::endl;
    f = test_tree::run_all();
    t = test_tree::NUM_TESTS;
    std::cerr << "passed (" << t - f << "/" << t << ")" << std::endl;
    fails += f;
    total += t;

    return f;
}

