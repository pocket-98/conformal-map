/**
 * @file util.hpp
 * @author Pavan Dayal
 */
#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <cmath>

namespace util {
    /** calculate the mean of an array */
    double mean(const double* arr, int n);
    double mean(const std::vector<double>& arr);
    double var(const double* arr, int n);
    double var(const std::vector<double>& arr);
    double std(const double* arr, int n);
    double std(const std::vector<double>& arr);
}

#endif /* UTIL_H */
