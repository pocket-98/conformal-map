/**
 * @file tree.cpp
 * @author Pavan Dayal
 */
#include "tree.hpp"
#include <cstring>

const std::vector<std::vector<int>>&
tree::subdivide(const char* s, int a, int b, std::ostream* err) {
    int i;
    int j = 0;
    int k = 0;
    char c;
    int brackets = 0;
    std::vector<int> termerr; // indices where parsing ambiguous for terms
    std::vector<int> proderr; // indices where parsing ambiguous for products
    std::vector<int>::const_iterator t, tend;
    std::vector<int>::const_iterator p, pend;

    // holds all addition and multiplication sub intervals
    std::vector<std::vector<int>>& terms = *(new std::vector<std::vector<int>>);
    std::vector<int> term; // subintervals of products for a single term

    // break up into +- substring intervals [+-a, b) and track parenthesis
    // for each term subinterval, break into subintervals of products [+-c, d)
    term.push_back(a+1); // start first + * interval
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
                if (k == 1) { // error: + appeared after * or /
                    termerr.push_back(i+1);
                } else if (j == 1) { // increase |a| for new substring start
                    if (term.back() < 0) {
                        term.back() = -i - 2;
                    } else {
                        term.back() = i + 2;
                    }
                } else { // end interval and start new interval with +a and +c
                    term.push_back(i+1);
                    terms.push_back(term);
                    term.clear();
                    term.push_back(i+2);
                }
                j = 0;
                k = 0;
            } else if (c == '-') {
                if (k == 1) { // error: - appeared after * or /
                    termerr.push_back(-i-1);
                } else if (j == 1) { // increase |a| and flip sign for new start
                    if (term.back() > 0) {
                        term.back() = -i - 2;
                    } else {
                        term.back() = i + 2;
                    }
                } else { // end interval and start new interval with -a and +c
                    term.push_back(i+1);
                    terms.push_back(term);
                    term.clear();
                    term.push_back(-i-2);
                }
                j = 0;
                k = 0;
            } else if (c == '*') {
                if (k == 1) { // error: * appeared after another operator
                    proderr.push_back(i+1);
                } else { // end interval and start new interval with +a
                    term.push_back(i+1);
                    term.push_back(i+2);
                }
                k = 0;
            } else if (c == '/') {
                if (k == 1) { // error: / appeared after another operator
                    proderr.push_back(-i-1);
                } else { // end interval and start new interval with +a
                    term.push_back(i+1);
                    term.push_back(-i-2);
                }
                k = 0;
            }
        }
    }
    term.push_back(b+1); // close last +* interval
    terms.push_back(term);

    // print errors for parsing ambiguity
    if (err != NULL) {
        t = termerr.cbegin();
        p = proderr.cbegin();
        tend = termerr.cend();
        pend = proderr.cend();
        while (p!=pend || t!=tend) {
            if ((*p)*(*p) < (*t)*(*t) || t == tend) {
                if (*p > 0) {
                    *err << "error: extra * at char " << *(p++) - 1 << ": '";
                } else {
                    *err << "error: extra / at char " << -*(p++) - 1 << ": '";
                }
            } else {
                if (*t > 0) {
                    *err << "error: extra + at char " << *(t++) - 1 << ": '";
                } else {
                    *err << "error: extra - at char " << -*(t++) - 1 << ": '";
                }
            }
            for (i=a; i<b; ++i) {
                *err << s[i];
            }
            *err << '\'' << std::endl;
        }
    }

    if (termerr.size() + proderr.size() > 0) {
        terms.clear();
    }

    return terms;
}

tree::Tree* parseTreeHelper(const char* s, int a, int b) {
    const std::vector<std::vector<int>>& terms = tree::subdivide(s, a, b);

    std::vector<std::vector<int>>::const_iterator term, lastterm = terms.cend();
    std::vector<int>::const_iterator prod, prodend;
    char c, c2;
    int j, k;
    for (term=terms.cbegin(); term!=lastterm; ++term) {
        prod = (*term).cbegin();
        prodend = (*term).cend();
        c = '+';
        c2 = '-';
        while (prod != prodend) {
            j = *(prod++);
            k = *(prod++) - 1;
            if (j >= 0) {
                j = j - 1;
                std::cout << c << '[' << j << ',' << k << ']';
            } else {
                j = -j - 1;
                std::cout << c2 << '[' << j << ',' << k << ']';
            }
            c = '*';
            c2 = '/';
        }
        std::cout << std::endl;
    }

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
    int l = terms.size();
    return tree::createTree(l);
}

tree::Tree* tree::parseTree(const std::string& s) {
    return parseTreeHelper(s.c_str(), 0, s.length());
}

tree::Tree* tree::parseTree(const char* s) {
    return parseTreeHelper(s, 0, strlen(s));
}

// print tree using tree.expression.str like printf format
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

