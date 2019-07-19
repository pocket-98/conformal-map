/**
 * @file main.cpp
 * @author Pavan Dayal
 */
#include <iostream>
#include "tree.hpp"

int main() {
    std::string s = "x/pi - 2*(x-y) + z/2";
    std::cout << s << std::endl;
    tree::Tree* pt = tree::parseTree(s);
    //tree::printTree(pt);
    //tree::freeTree(pt);

    return 0;
}
