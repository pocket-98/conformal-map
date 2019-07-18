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
            out << ' ';
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
    return out;
}

int test_tree::test_subdivide() {
    const char* s;
    std::stringstream out;
    std::vector<int> terms;

    s = "-x + y*z";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", "", out.str());
    out.str("");
    print_subintervals(out, terms);
    testequal("wrong subintervals", "-[1,3) +[4,6)*[7,8)", out.str());
    out.str("");

    s = "x + -y*z";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", "", out.str());
    out.str("");
    print_subintervals(out, terms);
    testequal("wrong subintervals", "+[0,2) -[5,6)*[7,8)", out.str());
    out.str("");

    s = "x - -y*z";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", "", out.str());
    out.str("");
    print_subintervals(out, terms);
    testequal("wrong subintervals", "+[0,2) +[5,6)*[7,8)", out.str());
    out.str("");

    s = "  + x*y^2 + (y-z)/x";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", "", out.str());
    out.str("");
    print_subintervals(out, terms);
    testequal("wrong subintervals","+[3,5)*[6,10) +[11,17)/[18,19)",out.str());
    out.str("");

    s = "x*-y + (y-z)**x";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", \
        "error: extra - at char 2: 'x*-y + (y-z)**x'\n"
        "error: extra * at char 13: 'x*-y + (y-z)**x'\n",
        out.str()
    );
    out.str("");
    print_subintervals(out, terms);
    testequal("wrong subintervals", "", out.str());
    out.str("");

    s = "((x+i*y) + (x-i*y) / 2";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", \
        "error: missing ) at char 22: '((x+i*y) + (x-i*y) / 2'\n",
        out.str()
    );
    out.str("");
    print_subintervals(out, terms);
    testequal("wrong subintervals", "", out.str());
    out.str("");

    s = "(x+i*y) - (x-i*y)) / 2";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", \
        "error: missing ( at char 0: '(x+i*y) - (x-i*y)) / 2'\n",
        out.str()
    );
    out.str("");
    print_subintervals(out, terms);
    testequal("wrong subintervals", "", out.str());
    out.str("");

    return EXIT_SUCCESS;
}

int test_tree::run_all() {
    int fails = 0;
    testfn(test_tree::test_create, "tree::createTree(), tree::freeTree()");
    testfn(test_tree::test_print, "tree::printTree()");
    testfn(test_tree::test_subdivide, "tree::subdivide()");
    return fails;
}

