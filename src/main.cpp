/**
 * @file main.cpp
 * @author Pavan Dayal
 */
#include <iostream>
#include "tree.hpp"

int main() {
    std::string s = "-x/pi-1/2";
    std::cout << s << std::endl;
    tree::Tree* pt = tree::parseTree(s);
    std::cout << pt << std::endl;
    tree::freeTree(pt);

    return 0;
}
