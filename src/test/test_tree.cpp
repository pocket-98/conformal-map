/**
 * @file test/test_tree.cpp
 * @author Pavan Dayal
 */
#include "test_tree.hpp"
#include <sstream>
#include <cstring>

int test_tree::test_create() {
    tree::Tree* t;
    t = tree::createTree(0);
    testequal("0 children", 0, t->n);
    testnull("children array not null", t->children);
    tree::freeTree(t);

    t = tree::createTree(2);
    t->expression->str = "$ + $";
    t->children[0] = tree::createTree(2);
    t->children[0]->expression->str = "$*$";
    t->children[0]->children[0] = tree::createTree(0);
    t->children[0]->children[0]->expression->str = "x";
    t->children[0]->children[1] = tree::createTree(0);
    t->children[0]->children[1]->expression->str = "y";
    t->children[1] = tree::createTree(0);
    t->children[1]->expression->str = "z";

    testequal("2 children", 2, t->n);
    testnotnull("children array null", t->children);
    testequal("2 children", 2, t->children[0]->n);
    testequal("0 children", 0, t->children[1]->n);
    testnotnull("children array null", t->children[0]->children);
    testnull("children array not null", t->children[1]->children);
    testequal("0 children", 0, t->children[0]->children[0]->n);
    testequal("0 children", 0, t->children[0]->children[1]->n);
    testnull("children array not null", t->children[0]->children[0]->children);
    testnull("children array not null", t->children[0]->children[1]->children);

    tree::freeTree(t);
    return EXIT_SUCCESS;
}

int test_tree::test_print() {
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

    std::stringstream out;
    out << t;
    testequal("bad print", "x*y + z", out.str());

    tree::freeTree(t);
    return EXIT_SUCCESS;
}

/**
 * print subintervals for + and *
 */
std::ostream& print_subintervals(std::ostream& out, const std::vector<int>& terms) {
    std::vector<int>::const_iterator term, lastterm;
    char c1 = '+';
    char c2 = '-';
    int j, k;
    lastterm = terms.cend();
    term = terms.cbegin();
    while (term != lastterm) {
        j = *(term++);
        if (j == 0) { // start next term
            out << std::endl;
            c1 = '+';
            c2 = '-';
            continue;
        } else {
            k = *(term++) - 1;
            if (j >= 0) {
                j = j - 1;
                out << c1 << '[' << j << ',' << k << ')';
            } else {
                j = -j - 1;
                out << c2 << '[' << j << ',' << k << ')';
            }
            c1 = '*';
            c2 = '/';
        }
    }
    return out << std::endl;
}

int test_tree::test_parse() {
    const char* s = "-x + y*z";
    int l = strlen(s);
    std::stringstream out;
    std::stringstream err;
    const std::vector<int>& terms = tree::subdivide(s, 0, l, &err);
    testequal("unexpected err message", "", err.str());
    print_subintervals(out, terms);
    std::cout << out.str();

    return EXIT_SUCCESS;
}

int test_tree::run_all() {
    int fails = 0;
    testfn(test_tree::test_create, "tree::createTree(), tree::freeTree()");
    testfn(test_tree::test_print, "tree::printTree()");
    testfn(test_tree::test_parse, "tree::subdivide()");
    return fails;
}

