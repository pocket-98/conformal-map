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
    t->expression->op = "add";
    t->children[0] = tree::createTree(2);
    t->children[0]->expression->str = "$*$";
    t->children[0]->expression->op = "multiply";
    t->children[0]->children[0] = tree::createTree(0);
    t->children[0]->children[0]->expression->str = "x";
    t->children[0]->children[0]->expression->op = "value";
    t->children[0]->children[1] = tree::createTree(0);
    t->children[0]->children[1]->expression->str = "y";
    t->children[0]->children[1]->expression->op = "value";
    t->children[1] = tree::createTree(0);
    t->children[1]->expression->str = "z";
    t->children[1]->expression->op = "value";

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
    t->expression->op = "add";
    t->children[0] = tree::createTree(2);
    t->children[0]->expression->str = "$*$";
    t->children[0]->expression->op = "multiply";
    t->children[0]->children[0] = tree::createTree(0);
    t->children[0]->children[0]->expression->str = "x";
    t->children[0]->children[0]->expression->op = "value";
    t->children[0]->children[1] = tree::createTree(0);
    t->children[0]->children[1]->expression->str = "y";
    t->children[0]->children[1]->expression->op = "value";
    t->children[1] = tree::createTree(0);
    t->children[1]->expression->str = "z";
    t->children[1]->expression->op = "value";

    std::stringstream out;
    out << t;
    testequal("bad print", "x*y + z", out.str());
    out.str("");

    out << tree::prefixString(t);
    testequal("bad prefix print", "add(multiply(x,y),z)", out.str());
    out.str("");

    tree::freeTree(t);
    return EXIT_SUCCESS;
}

int test_tree::test_subdivide() {
    const char* s;
    std::stringstream out;
    std::vector<int> terms;

    terms.push_back(0);
    out << tree::subintervalString(terms);
    testequal("string not blank", "", out.str());
    out.str("");

    terms[0] = 1;
    terms.push_back(1);
    terms.push_back(5+1);
    terms.push_back(10+1);
    out << tree::subintervalString(terms);
    testequal("subintervals displayed wrong", "+[5,10)", out.str());
    out.str("");

    terms[1] = 2;
    terms[2] = -5-1;
    terms.push_back(-15-1);
    terms.push_back(17+1);
    out << tree::subintervalString(terms);
    testequal("string wrong", "-[5,10)/[15,17)", out.str());
    out.str("");

    terms[0] = 2;
    terms.push_back(2);
    terms.push_back(20+1);
    terms.push_back(21+1);
    terms.push_back(24+1);
    terms.push_back(25+1);
    out << tree::subintervalString(terms);
    testequal("string wrong", "-[5,10)/[15,17) +[20,21)*[24,25)", out.str()
    );
    out.str("");

    s = "-x + y*z";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", "", out.str());
    out.str("");
    testequal("number of terms", 2, terms[0]);
    out << tree::subintervalString(terms);
    testequal("wrong subintervals", "-[1,3) +[4,6)*[7,8)", out.str());
    out.str("");

    s = "x + -y*z";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", "", out.str());
    out.str("");
    testequal("number of terms", 2, terms[0]);
    out << tree::subintervalString(terms);
    testequal("wrong subintervals", "+[0,2) -[5,6)*[7,8)", out.str());
    out.str("");

    s = "x - -y*z";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", "", out.str());
    out.str("");
    testequal("number of terms", 2, terms[0]);
    out << tree::subintervalString(terms);
    testequal("wrong subintervals", "+[0,2) +[5,6)*[7,8)", out.str());
    out.str("");

    s = "  + x*y^2 + (y-z)/x";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", "", out.str());
    out.str("");
    testequal("number of terms", 2, terms[0]);
    out << tree::subintervalString(terms);
    testequal("wrong subintervals","+[3,5)*[6,10) +[11,17)/[18,19)", out.str());
    out.str("");

    s = "  - x*y^2 + (y-z)/x";
    terms = tree::subdivide(s, 4, 17, &out);
    testequal("unexpected err message", "", out.str());
    out.str("");
    testequal("number of terms", 2, terms[0]);
    out << tree::subintervalString(terms);
    testequal("wrong subintervals","+[4,5)*[6,10) +[11,17)", out.str());
    out.str("");

    s = "(a)(x-x0)";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", "", out.str());
    out.str("");
    testequal("number of terms", 1, terms[0]);
    out << tree::subintervalString(terms);
    testequal("wrong subintervals","+[0,9)", out.str());
    out.str("");

    s = "a)(x-x0";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message",
        "error: missing ( at char 0: 'a)(x-x0'\n"
        "error: missing ) at char 7: 'a)(x-x0'\n",
        out.str()
    );
    out.str("");
    testequal("number of terms", 0, terms[0]);
    out << tree::subintervalString(terms);
    testequal("wrong subintervals","", out.str());
    out.str("");

    s = "a) )( (x";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message",
        "error: missing ( at char 0: 'a) )( (x'\n"
        "error: missing ( at char 0: 'a) )( (x'\n"
        "error: missing ) at char 8: 'a) )( (x'\n"
        "error: missing ) at char 8: 'a) )( (x'\n",
        out.str()
    );
    out.str("");
    testequal("number of terms", 0, terms[0]);
    out << tree::subintervalString(terms);
    testequal("wrong subintervals","", out.str());
    out.str("");

    s = "x*-y + (y-z)**x";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", \
        "error: extra - at char 2: 'x*-y + (y-z)**x'\n"
        "error: extra * at char 13: 'x*-y + (y-z)**x'\n",
        out.str()
    );
    out.str("");
    testequal("number of terms", 0, terms[0]);
    out << tree::subintervalString(terms);
    testequal("wrong subintervals", "", out.str());
    out.str("");

    s = "((x+i*y) + (x-i*y) / 2";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", \
        "error: missing ) at char 22: '((x+i*y) + (x-i*y) / 2'\n",
        out.str()
    );
    out.str("");
    testequal("number of terms", 0, terms[0]);
    out << tree::subintervalString(terms);
    testequal("wrong subintervals", "", out.str());
    out.str("");

    s = "(x+i*y) - (x-i*y)) / 2";
    terms = tree::subdivide(s, 0, strlen(s), &out);
    testequal("unexpected err message", \
        "error: missing ( at char 0: '(x+i*y) - (x-i*y)) / 2'\n",
        out.str()
    );
    out.str("");
    testequal("number of terms", 0, terms[0]);
    out << tree::subintervalString(terms);
    testequal("wrong subintervals", "", out.str());
    out.str("");

    return EXIT_SUCCESS;
}

int test_tree::run_all() {
    int fails = 0;
    testfn(test_tree::test_create, "tree::createTree(), tree::freeTree()");
    testfn(test_tree::test_print, "tree::operator<<(), tree::prefixString()");
    testfn(test_tree::test_subdivide, "tree::subdivide(), tree::subdivideString()");
    return fails;
}

