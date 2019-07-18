/**
 * @file main.cpp
 * @author Pavan Dayal
 */
#include <iostream>
#include "util.hpp"
#include "tree.hpp"

int main() {
    tree::Tree* t = tree::createTree(2);
    t->expression->str = "$ + $";
    t->children[0] = tree::createTree(2);
    t->children[0]->expression->str = "$*$";
    t->children[0]->children[0] = tree::createTree(0);
    t->children[0]->children[0]->expression->str = "x";
    t->children[0]->children[1] = tree::createTree(0);
    t->children[0]->children[1]->expression->str = "y";
    t->children[1] = tree::createTree(0);
    t->children[1]->expression->str = "z";
    std::cout << t << std::endl;
    tree::freeTree(t);

    std::string s = "x*3 - (y-2) - z/2";
    std::cout << s << std::endl;
    tree::Tree* pt = tree::parseTree(s);
    //tree::printTree(pt);
    //tree::freeTree(pt);

    return 0;
}
