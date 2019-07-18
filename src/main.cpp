/**
 * @file main.cpp
 * @author Pavan Dayal
 */
#include <iostream>
#include "util.hpp"
#include "tree.hpp"

int main() {
    std::string s = "x*3 + (1)*-2/-3 + z/2";
    std::cout << s << std::endl;
    tree::Tree* pt = tree::parseTree(s);
    //tree::printTree(pt);
    //tree::freeTree(pt);

    return 0;
}
