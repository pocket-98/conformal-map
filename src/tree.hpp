/**
 * @file tree.hpp
 * @author Pavan Dayal
 */
#ifndef TREE_H
#define TREE_H

#include <iostream>

namespace tree {
    /** holds string representation and function to evaluate */
    struct Expression {
        const char* str;
    };

    /** tree with arbitrary number of children */
    struct Tree {
        Expression* expression;
        //Tree* parent;
        Tree** children;
        int n;
    };

    /** create an expression tree from math string */
    Tree* parseTree(const std::string& s);
    Tree* parseTree(const char* s);

    /** malloc a new tree with n children */
    Tree* createTree(int n);

    /** free a tree in postorder: free the children first */
    void freeTree(Tree* t);
}

/** print each expression string in preorder: visit node then children */
std::ostream& operator<<(std::ostream& out, tree::Tree* t);

#endif /* TREE_H */
