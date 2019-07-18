/**
 * @file tree.hpp
 * @author Pavan Dayal
 */
#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <vector>

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

    /**
     * extract the subintervals for dividing the string into a sum of products
     * prods = { <[a,d1) [c2,d2) [c3,b)>, <[a,d) [c,b)>, <[a,b)> }
     * each element represents a subinterval for a term
     * each element has an element representing a subsubinterval for a factor
     * the sign of the first factor for each term indicates + or -
     * the sign of every factor after the first indicates * or /
     */
    const std::vector<std::vector<int>>&
    subdivide(const char* s, int a, int b, std::ostream* err=&std::cerr);

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
