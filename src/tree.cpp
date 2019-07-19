/**
 * @file tree.cpp
 * @author Pavan Dayal
 */
#include "tree.hpp"
#include <cstring>
#include <sstream>

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

const std::vector<int>
tree::subdivide(const char* s, int a, int b, std::ostream* err) {
    int i, l;
    int j = 0;
    int k = 0;
    int prod_ctr;
    char c;
    int brackets = 0;
    bool error = false;
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
            if (brackets < 0) {
                error = true;
                if (err != NULL) {
                    *err << "error: missing ( at char 0: '";
                    for (l=a; l<b; ++l) { *err << s[l]; }
                    *err << '\'' << std::endl;
                    ++brackets;
                } else {
                    break;
                }
            }
        }

        // track how many characters since last + or - and since last * or /
        if (c != ' ') {
            j++;
            k++;
        }

        if (brackets == 0) {
            if (c == '+') {
                if (k == 1 && notfirst) { // error: + appeared after * or /
                    error = true;
                    if (err != NULL) {
                        *err << "error: extra + at char " << i << ": '";
                        for (l=a; l<b; ++l) { *err << s[l]; }
                        *err << '\'' << std::endl;
                    } else {
                        break;
                    }
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
                if (k == 1 && notfirst) { // error: - appeared after * or /
                    error = true;
                    if (err != NULL) {
                        *err << "error: extra - at char " << i << ": '";
                        for (l=a; l<b; ++l) { *err << s[l]; }
                        *err << '\'' << std::endl;
                    } else {
                        break;
                    }
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
                    error = true;
                    if (err != NULL) {
                        *err << "error: extra * at char " << i << ": '";
                        for (l=a; l<b; ++l) { *err << s[l]; }
                        *err << '\'' << std::endl;
                    } else {
                        break;
                    }
                } else { // end interval and start new interval with +a
                    terms.push_back(i+1); // close last subsubinterval
                    terms.push_back(i+2); // start next subsubinterval
                    terms[prod_ctr] += 1; // increase number of products counter
                }
                k = 0;
                notfirst = true;
            } else if (c == '/') {
                if (k == 1) { // error: / appeared after another operator
                    error = true;
                    if (err != NULL) {
                        *err << "error: extra / at char " << i << ": '";
                        for (l=a; l<b; ++l) { *err << s[l]; }
                        *err << '\'' << std::endl;
                    } else {
                        break;
                    }
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
    if (brackets > 0) {
        error = true;
        if (err != NULL) {
            while (brackets-- > 0) {
                *err << "error: missing ) at char " << b << ": '";
                for (l=a; l<b; ++l) { *err << s[l]; }
                *err << '\'' << std::endl;
            }
        }
    }

    if (error) {
        terms.clear();
        terms.push_back(0);
    }

    return terms;
}

/////////////////////////////////////////
// algorithm: parse_tree               //
/////////////////////////////////////////
//  terms = subdivide()                //
//  tree = add(x1,x2,x3,...)           //
//  for term in terms:                 //
//      t = multiply(x1,x2,x3,...)     //
//      for prod in term:              //
//          p = handle_factor(prod)    //
//          t.children.append(p)       //
//      tree.children.append(t)        //
//  return tree                        //
/////////////////////////////////////////
// algorithm: handle_factor            //
/////////////////////////////////////////
//  if prod_has_form f(x+y)^g(z+w):    //
//      p = pow(x1,x2)                 //
//       * child1 = f(x1)              //
//         * child11 = parse_tree(x+y) //
//       * child2 = g(x1)              //
//         * child21 = parse_tree(z+w) //
//  else if prod_has_form f(a*b)':     //
//      p = conjugate(x1)              //
//       * child1 = f(x1)              //
//         * child11 = parse_tree(a*b) //
//  else if prod_has_form f(z^2):      //
//      p = f(x1)                      //
//       * child11 = parse_tree(z^2)   //
//  else: // prod_has_form z           //
//      p = constant_or_variable(z)    //
//  return p                           //
/////////////////////////////////////////

// break down factor into tree of function calls and values
tree::Tree* handleFactor(const char* s, int a, int b) {
    //TODO implement recursion
    std::stringstream expr;
    const char* end = s + b;
    s = s + a;
    while (s != end) {
        expr << *(s++);
    }
    tree::Tree* t = tree::createTree(0);
    t->expression->str = expr.str();
    t->expression->op = "value";
    return t;
}

// create tree of sums of products
tree::Tree* parseTreeHelper(const char* s, int a, int b) {
    int i, j, newa, newb;
    int num_terms, num_prods;
    bool negative;
    tree::Tree* t;
    tree::Tree* term;
    tree::Tree* prod;
    std::stringstream term_str, prod_str;
    const std::vector<int>& terms = tree::subdivide(s, a, b);
    std::vector<int>::const_iterator terms_ptr = terms.cbegin();

    num_terms = *(terms_ptr++);
    if (num_terms == 0) {
        return NULL;
    }

    num_prods = *(terms_ptr++);
    newa = *(terms_ptr++);
    newb = *(terms_ptr++) - 1;
    if (newa >= 0) { // the term is positive
        newa -= 1;
        negative = false;
    } else { // the term is negative
        newa = -newa - 1;
        negative = true;
    }
    t = handleFactor(s, newa, newb);
    if (num_prods > 1) { // create parent to hold all factors
        prod = t;
        term = tree::createTree(num_prods);
        prod_str << "$";
        term->children[0] = prod;
        for (j=1; j<num_prods; ++j) {
            newa = *(terms_ptr++);
            newb = *(terms_ptr++) - 1;
            if (newa >= 0) { // this factor gets multiplied
                newa -= 1;
                prod_str << " * $";
                term->children[j] = handleFactor(s, newa, newb);
            } else { // this factor geds divided
                newa = -newa - 1; // ones complement
                prod_str << " $";
                prod = tree::createTree(1);
                prod->expression->str = "/ $";
                prod->expression->op = "inverse";
                prod->children[0] = handleFactor(s, newa, newb);
                term->children[j] = prod;
            }
        }
        term->expression->str = prod_str.str();
        term->expression->op = "multiply";
        t = term;
    }
    if (negative) {
        prod = t;
        t = tree::createTree(1);
        t->expression->str = "-$"; // no space since only 1 term
        t->expression->op = "negate";
        t->children[0] = prod;
    }

    if (num_terms > 1) {
        term = t;
        t = tree::createTree(num_terms);
        term_str << "$";
        t->children[0] = term;
        for (i=1; i<num_terms; ++i) {
            prod_str.str("");
            num_prods = *(terms_ptr++);
            newa = *(terms_ptr++);
            newb = *(terms_ptr++) - 1;
            if (newa >= 0) { // the term is positive
                newa -= 1;
                negative = false;
            } else { // the term is negative
                newa = -newa - 1;
                negative = true;
            }
            term = handleFactor(s, newa, newb);
            if (num_prods > 1) { // create parent to hold all factors
                prod = term;
                term = tree::createTree(num_prods);
                prod_str << "$";
                term->children[0] = prod;
                for (j=1; j<num_prods; ++j) {
                    newa = *(terms_ptr++);
                    newb = *(terms_ptr++) - 1;
                    if (newa >= 0) { // this factor gets multiplied
                        newa -= 1;
                        prod_str << " * $";
                        term->children[j] = handleFactor(s, newa, newb);
                    } else { // this factor geds divided
                        newa = -newa - 1; // ones complement
                        prod_str << " $";
                        prod = tree::createTree(1);
                        prod->expression->str = "/ $";
                        prod->expression->op = "inverse";
                        prod->children[0] = handleFactor(s, newa, newb);
                        term->children[j] = prod;
                    }
                }
                term->expression->str = prod_str.str();
                term->expression->op = "multiply";
            }

            if (negative) {
                prod = term;
                term = tree::createTree(1);
                term->expression->str = "- $"; // space since second term
                term->expression->op = "negate"; // space since second term
                term->children[0] = prod;
                term_str << " $"; // prepare to append this - term to t
            } else {
                term_str << " + $"; // prepare to append this + term to t
            }
            t->children[i] = term;
        }
        t->expression->str = term_str.str();
        t->expression->op = "add";
    }

    return t;
}

tree::Tree* tree::parseTree(const std::string& s) {
    return parseTreeHelper(s.c_str(), 0, s.length());
}

tree::Tree* tree::parseTree(const char* s) {
    return parseTreeHelper(s, 0, strlen(s));
}

// print + - * / subintervals from list of indices
std::string tree::subintervalString(const std::vector<int>& terms) {
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

// print tree using prefix notation
void prefixStringHelper(std::ostream& out, tree::Tree* t) {
    if (t->n == 0) {
        out << t->expression->str;
    } else {
        out << t->expression->op << '(';
        prefixStringHelper(out, t->children[0]);
        for (int i=1; i<t->n; ++i) {
            out << ',';
            prefixStringHelper(out, t->children[i]);
        }
        out << ')';
    }
}

std::string tree::prefixString(tree::Tree* t) {
    std::stringstream out;
    prefixStringHelper(out, t);
    return out.str();
}

// print tree using tree.expression.str like printf format
std::ostream& operator<<(std::ostream& out, tree::Tree* t) {
    const char* expr = t->expression->str.c_str();
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

