#include "util.hpp"

double util::mean(const double* arr, int n) {
    if (n < 1) {
        return 0;
    }
    double sum = 0.0;
    const double* end = arr + n;
    while (arr < end) {
        sum += *(arr++);
    }
    return sum / n;
}

double util::mean(const std::vector<double>& arr) {
    return util::mean(arr.data(), arr.size());
}

double util::var(const double* arr, int n) {
    if (n < 2) {
        return 0;
    }
    double sum = 0.0;
    double squares = 0.0;
    const double* end = arr + n;
    double d;
    while (arr < end) {
        d = *(arr++);
        sum += d;
        squares += d * d;
    }
    return (squares - sum * sum / n) / (n-1);
}

double util::var(const std::vector<double>& arr) {
    return util::var(arr.data(), arr.size());
}

double util::std(const double* arr, int n) {
    return sqrt(util::var(arr, n));
}

double util::std(const std::vector<double>& arr) {
    return sqrt(util::var(arr.data(), arr.size()));
}

