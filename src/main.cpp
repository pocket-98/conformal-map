/**
 * @file main.cpp
 * @author Pavan Dayal
 */
#include <iostream>
#include "tree.hpp"

int main() {
    std::string s = "-x/2 + 3*z^2-y";
    std::cout << s << std::endl;

    tree::Tree* t = tree::parseTree(s);
    std::cout << tree::prefixString(t) << std::endl;
    std::cout << t << std::endl;
    tree::freeTree(t);

    return 0;
}
