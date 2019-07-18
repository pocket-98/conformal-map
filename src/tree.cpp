/**
 * @file tree.cpp
 * @author Pavan Dayal
 */
#include "tree.hpp"
#include <cstring>
#include <vector>

tree::Tree* parseTreeHelper(const char* s, int a, int b) {
    tree::Tree* parent = NULL;
    tree::Tree* child = NULL;
    int i;
    int j = 0;
    int k = 0;
    char c;
    int brackets = 0;
    std::vector<int> terms; // contains substring intervals for terms
    std::vector<int> prod; // subintervals of products for a single term
    std::vector<int> proderr; // indices where parsing ambiguous for products
    std::vector<std::vector<int>> prods; // containter for all product subintervals
    std::vector<int>::const_iterator t, tend;
    std::vector<int>::const_iterator p, pend;

    // break up by + and - into substring intervals [+-a, b) and track parenthesis
    // for each term subinterval, break into sub-subintervals of products [+-c, d)
    terms.push_back(a+1); // start first + interval
    prod.push_back(a+1); // start first * interval
    for (i=a; i<b; ++i) {
        c = s[i];
        if (c == '\t' || c == '\n') {
            c = ' ';
        } else if (c == '(') {
            ++brackets;
        } else if (c == ')') {
            --brackets;
        }

        // track how many characters since last + or - and since last * or /
        if (c != ' ') {
            j++;
            k++;
        }

        if (brackets == 0) {
            if (c == '+') {
                if (j == 1) { // increase |a| for new substring start
                    if (terms.back() < 0) {
                        terms.back() = -i - 2;
                        prod.back() = -i - 2;
                    } else {
                        terms.back() = i + 2;
                        prod.back() = i + 2;
                    }
                } else { // end interval and start new interval with +a and +c
                    terms.push_back(i+1);
                    terms.push_back(i+2);
                    prod.push_back(i+1);
                    prods.push_back(prod);
                    prod.clear();
                    prod.push_back(i+2);
                }
                j = 0;
                k = 0;
            } else if (c == '-') {
                if (j == 1) { // increase |a| and flip sign for new substring start
                    if (terms.back() > 0) {
                        terms.back() = -i - 2;
                        prod.back() = -i - 2;
                    } else {
                        terms.back() = i + 2;
                        prod.back() = i + 2;
                    }
                } else { // end interval and start new interval with -a and +c
                    terms.push_back(i+1);
                    terms.push_back(-i-2);
                    prod.push_back(i+1);
                    prods.push_back(prod);
                    prod.clear();
                    prod.push_back(i+2);
                }
                j = 0;
                k = 0;
            } else if (c == '*') {
                if (k == 1) { // increase |k| for new subsubstring start
                    proderr.push_back(i+1);
                    if (prod.back() < 0) {
                        prod.back() = -i - 2;
                    } else {
                        prod.back() = i + 2;
                    }
                } else { // end interval and start new interval with +a
                    prod.push_back(i+1);
                    prod.push_back(i+2);
                }
                k = 0;
            } else if (c == '/') {
                if (k == 1) { // increase |k| for new subsubstring start
                    proderr.push_back(-i-1);
                    if (prod.back() < 0) {
                        prod.back() = -i - 2;
                    } else {
                        prod.back() = i + 2;
                    }
                } else { // end interval and start new interval with +a
                    prod.push_back(i+1);
                    prod.push_back(i+2);
                }
                k = 0;
            }
        }
    }
    terms.push_back(b+1); // close last + interval
    prod.push_back(b+1); // close last * interval
    prods.push_back(prod);

    // check for parsing ambiguity
    i = 0;
    pend = proderr.cend();
    for (p=proderr.cbegin(); p!=pend; ++p) {
        if (*p > 0) {
            std::cerr << "error: extra * at char " << *p - 1 << ": '";
        } else {
            std::cerr << "error: extra / at char " << -*p - 1 << ": '";
        }
        for (i=a; i<b; ++i) {
            std::cerr << s[i];
        }
        std::cerr << "'\n";
    }

    if (i) {
        return NULL;
    }

    // print each term and its products as subintervals +(a,b)*[c,d]*[c,d]
    i = 0;
    tend = terms.cend();
    for (t=terms.cbegin(); t!=tend;) {
        j = *(t++);
        k = *(t++) - 1;
        prod = prods[i++];
        pend = prod.cend();
        if (j >= 0) {
            j = j - 1;
            std::cout << "+(" << j << ',' << k << ')';
        } else {
            j = -j - 1;
            std::cout << "-(" << j << ',' << k << ')';
        }
        for (p=prod.cbegin(); p!=pend;) {
            if (*p >= 0) {
                std::cout << "*[" << *(p++) - 1 << ',';
                std::cout << *(p++) - 1 << ']';
            } else {
                std::cout << "/[" << -*(p++) - 1 << ',';
                std::cout << *(p++) - 1 << ']';
            }
        }
        std::cout << ' ';
    }
    std::cout << std::endl;

    // recursively parse each subinterval and combine with addition
    if (terms.size() > 2) {
    }

    return child;

//        if (j > 1) {
//            parent = createTree(2);
//            parent->expression->str = "$ $"; // add 2 children
//            parent->children[0] = parseTreeHelper(s, a, i);
//            parent->children[1] = parseTreeHelper(s, i, b);
//        } else {
//            parent = createTree(1);
//            parent->expression->str = "- $"; // negate child
//            parent->children[0] = parseTreeHelper(s, i, i);
//        }
//        break;

    return NULL;
}

tree::Tree* tree::parseTree(const std::string& s) {
    return parseTreeHelper(s.c_str(), 0, s.length());
}

tree::Tree* tree::parseTree(const char* s) {
    return parseTreeHelper(s, 0, strlen(s));
}

std::ostream& operator<<(std::ostream& out, tree::Tree* t) {
    const char* expr = t->expression->str;
    int i = 0;
    int j = 0;
    char c = ' ';
    while (c != '\0') {
        c = expr[j++];
        if (c == '$') {
            out << t->children[i++];
        } else {
            out << c;
        }
    }
    return out;
}

// 3 calls to new
tree::Tree* tree::createTree(int n) {
    tree::Tree* t = new tree::Tree;
    t->expression = new Expression;
    if (n == 0) {
        t->children = NULL;
    } else {
        t->children = new tree::Tree*[n];
    }
    t->n = n;
    return t;
}

void tree::freeTree(tree::Tree* t) {
    int i;
    tree::Tree* child;
    for (i=0; i<t->n; ++i) {
        child = t->children[i];
        if (child != NULL) {
            freeTree(child);
        }
    }
    // 3 calls to delete
    delete t->expression;
    delete t->children;
    delete t;
}

