/**
 * @file test/test.hpp
 * @author Pavan Dayal
 */
#ifndef _TEST_H
#define _TEST_H

#include <iostream>
#include <string>
#include <cmath>

#define _TEST_EPS 1.0e-10

/**
 * checks if t == true and prints useful error message
 */
#define test(str,t) { \
    if (!(t)) { \
        std::cerr << indent << "\033[0;31m" << "error:" << "\033[0m" \
                  << __LINE__ << ": " << (str) << std::endl; \
        return EXIT_FAILURE; \
    } \
}

/**
 * checks if |a-b| < eps and prints useful error message
 */
#define testequalish(str,a,b) { \
    if (fabs((a)-(b)) > _TEST_EPS) { \
        std::cerr << indent << "\033[0;31m" << "error:" << "\033[0m" \
                  << __LINE__ << ": " << (str) << ": expected: " << a \
                  << " actual: " << b << std::endl; \
        return EXIT_FAILURE; \
    } \
}

/**
 * checks if a == b and prints useful error message
 */
#define testequal(str,a,b) { \
    if ((a) != (b)) { \
        std::cerr << indent << "\033[0;31m" << "error:" << "\033[0m" \
                  << __LINE__ << ": " << (str) << ": expected: " << a \
                  << " actual: " << b << std::endl; \
        return EXIT_FAILURE; \
    } \
}

/**
 * useful for run_all() function
 * make sure parameter is named `fails`
 */
#define testfn(name,print) { \
    if (name()) { \
        fails += 1; \
    } else { \
        std::cerr << indent << "\033[0;32m" << "pass" << "\033[0m: "; \
        std::cerr << print << std::endl; \
    } \
}

/**
 * allows control of printing with layers of indentation
 */
extern std::string indent;

#endif /* _TEST_H */
