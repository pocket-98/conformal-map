/**
 * @file test/test.hpp
 * @author Pavan Dayal
 */
#ifndef TEST_H
#define TEST_H

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
 * checks if |a-b| > eps and prints useful error message
 */
#define testnotequalish(str,a,b) { \
    if (fabs((a)-(b)) < _TEST_EPS) { \
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
 * checks if a != b and prints useful error message
 */
#define testnotequal(str,a,b) { \
    if ((a) == (b)) { \
        std::cerr << indent << "\033[0;31m" << "error:" << "\033[0m" \
                  << __LINE__ << ": " << (str) << ": expected: " << a \
                  << " actual: " << b << std::endl; \
        return EXIT_FAILURE; \
    } \
}

/**
 * checks if b == NULL and prints useful error message
 */
#define testnull(str,b) { \
    if (NULL != (b)) { \
        std::cerr << indent << "\033[0;31m" << "error:" << "\033[0m" \
                  << __LINE__ << ": " << (str) << ": expected: null ptr" \
                  << " actual: " << b << std::endl; \
        return EXIT_FAILURE; \
    } \
}

/**
 * checks if b != NULL and prints useful error message
 */
#define testnotnull(str,b) { \
    if (NULL == (b)) { \
        std::cerr << indent << "\033[0;31m" << "error:" << "\033[0m" \
                  << __LINE__ << ": " << (str) << ": expected: valid ptr" \
                  << " actual: null ptr" << std::endl; \
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
        std::cerr << indent << "\033[0;31m" << "fail" << "\033[0m: "; \
        std::cerr << print << std::endl; \
    } else { \
        std::cerr << indent << "\033[0;32m" << "pass" << "\033[0m: "; \
        std::cerr << print << std::endl; \
    } \
}

/**
 * allows control of printing with layers of indentation
 */
extern std::string indent;

#endif /* TEST_H */
