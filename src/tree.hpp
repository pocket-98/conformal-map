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
        std::string str;
    };

    /** tree with arbitrary number of children */
    struct Tree {
        Expression* expression;
        //Tree* parent;
        Tree** children;
        int n;
    };

    /**
     * extract the + - * / subintervals for dividing the string into terms
     * terms = n, { [a,d1),[c2,d2),[c3,b), 0, [a,d),[c,b), 0, [a,b) }
     * first element in array gives total number of terms
     * each group ints represents a subinterval for a term delimited with 0s
     * every term is broken into subsubinterval for a factor
     * the sign of the first factor for each term indicates + or -
     * the sign of every factor after the first indicates * or /
     */
    const std::vector<int>
    subdivide(const char* s, int a, int b, std::ostream* err=&std::cerr);

    /* print out the subintervals for + - * / to output stream */
    std::string subintervalString(const std::vector<int>& terms);

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
