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

    std::string s = "x*(y - + z)-3/2/(z)+sin(z)/2";
    std::cout << s << std::endl;
    t = tree::parseTree(s);
    //printTree(t);

    return 0;
}
