/**
 * @file tree.cpp
 * @author Pavan Dayal
 */
#include "tree.hpp"
#include "test/test_tree.hpp"
#include <cstring>
#include <sstream>

const std::vector<int>
tree::subdivide(const char* s, int a, int b, std::ostream* err) {
    int i;
    int j = 0;
    int k = 0;
    int prod_ctr;
    char c;
    int brackets = 0;
    std::vector<int> termerr; // indices where parsing ambiguous for terms
    std::vector<int> proderr; // indices where parsing ambiguous for products
    std::vector<int>::const_iterator t, tend;
    std::vector<int>::const_iterator p, pend;

    std::vector<int> terms; // holds all + and * subintervals with delimiter: 0
    bool notfirst = false; // whether looking at first character in subinterval

    // break up into +- substring intervals [+-a, b) and track parenthesis
    // for each term subinterval, break into subintervals of products [+-c, d)
    terms.push_back(1); // number of terms total
    terms.push_back(1); // number of products in term
    prod_ctr = 1;
    terms.push_back(a+1); // start first + * interval
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
                if (k == 1 && notfirst) { // error: + appeared after * or /
                    termerr.push_back(i+1);
                } else if (j == 1) { // increase |a| for new substring start
                    if (terms.back() < 0) {
                        terms.back() = -i - 2;
                    } else {
                        terms.back() = i + 2;
                    }
                } else { // end interval and start new interval with +a and +c
                    terms.push_back(i+1); // close last subinterval
                    terms[0] += 1; // increase total number of terms counter
                    prod_ctr = terms.size();
                    terms.push_back(1); // set number of products counter
                    terms.push_back(i+2); // start next subinterval
                }
                j = 0;
                k = 0;
            } else if (c == '-') {
                if (k == 1 && notfirst) { // warning: - appeared after * or /
                    termerr.push_back(-i-1);
                } else if (j == 1) { // increase |a| and flip sign for new start
                    if (terms.back() > 0) {
                        terms.back() = -i - 2;
                    } else {
                        terms.back() = i + 2;
                    }
                } else { // end interval and start new interval with -a and +c
                    terms.push_back(i+1); // close last subinterval
                    terms[0] += 1; // increase total number of terms counter
                    prod_ctr = terms.size();
                    terms.push_back(1); // set number of products counter
                    terms.push_back(-i-2); // start next subinterval
                }
                j = 0;
                k = 0;
            } else if (c == '*') {
                if (k == 1) { // error: * appeared after another operator
                    proderr.push_back(i+1);
                } else { // end interval and start new interval with +a
                    terms.push_back(i+1); // close last subsubinterval
                    terms.push_back(i+2); // start next subsubinterval
                    terms[prod_ctr] += 1; // increase number of products counter
                }
                k = 0;
                notfirst = true;
            } else if (c == '/') {
                if (k == 1) { // error: / appeared after another operator
                    proderr.push_back(-i-1);
                } else { // end interval and start new interval with +a
                    terms.push_back(i+1);
                    terms.push_back(-i-2);
                    terms[prod_ctr] += 1; // increase number of products counter
                }
                k = 0;
                notfirst = true;
            }
        }
    }
    terms.push_back(b+1); // close last +* interval

    // print errors for parsing ambiguity
    if (err != NULL) {
        if (brackets > 0) {
            *err << "error: missing ) at char " << b << ": '";
        } else if (brackets < 0) {
            *err << "error: missing ( at char 0" << ": '";
        }
        if (brackets != 0) {
            for (i=a; i<b; ++i) {
                *err << s[i];
            }
            *err << '\'' << std::endl;
        }
        t = termerr.cbegin();
        p = proderr.cbegin();
        tend = termerr.cend();
        pend = proderr.cend();
        while (p!=pend || t!=tend) {
            if (t == tend || (p != pend && (*p)*(*p) < (*t)*(*t))) {
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

    if (termerr.size() + proderr.size() > 0 || brackets != 0) {
        terms.clear();
        terms.push_back(0);
    }

    return terms;
}

/**
 * print subintervals for + and *
 */
std::string tree::subinterval_string(const std::vector<int>& terms) {
    std::stringstream out;
    std::vector<int>::const_iterator term, lastterm;
    char c1 = '+';
    char c2 = '-';
    int j, k;
    int prod_ctr;
    lastterm = terms.cend();
    term = terms.cbegin();
    if (*(term++) > 0) {
        prod_ctr = *(term++);
    }
    while (term != lastterm) {
        j = *(term++);
        if (prod_ctr-- == 0) { // start next term
            prod_ctr = j;
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
    return out.str();
}

tree::Tree* parseTreeHelper(const char* s, int a, int b) {
    const std::vector<int>& terms = tree::subdivide(s, a, b);
    std::cout << "num terms: " << terms[0] << std::endl;
    std::cout << tree::subinterval_string(terms) << std::endl;

    ////////////////////////////////////////
    // algorithm: recursive_parse
    ////////////////////////////////////////
    //  terms = subdivide()
    //  tree = add(x1,x2,x3,...)
    //  for term in terms:
    //      t = multiply(x1,x2,x3,...)
    //      for prod in term:
    //          if prod_has_form f(x+y)^g(z+w):
    //              p = pow(x1,x2)
    //               * child1 = f(x1)
    //                 * child11 = recursive_parse(x+y)
    //               * child2 = g(x1)
    //                 * child21 = recursive_parse(z+w)
    //          else if prod_has_form f(a*b)':
    //              p = conjugate(x1)
    //               * child1 = f(x1)
    //                 * child11 = recursive_parse(a*b)
    //          else if prod_has_form f(z^2):
    //              p = f(x1)
    //               * child11 = recursive_parse(z^2)
    //          else: // prod_has_form z
    //              p = constant_or_variable(z)
    //          t.children.append(p)
    //      tree.children.append(t)
    //  return tree
    ////////////////////////////////////////

//        if (j > 1) {
//            paren t = createTree(2);
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

// print tree using tree.expression.str like printf format
std::ostream& operator<<(std::ostream& out, tree::Tree* t) {
    const char* expr = t->expression->str;
    int i = 0;
    int j = 0;
    char c;
    while (true) {
        c = expr[j++];
        if (c == '$') {
            out << t->children[i++];
        } else if (c == '\0') {
            break;
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
    for (i=0; i<t->n; ++i) {
        freeTree(t->children[i]);
    }
    // 3 calls to delete
    delete t->expression;
    delete[] t->children;
    delete t;
}

