/**
 * @file test/test_util.cpp
 * @author Pavan Dayal
 */
#include "test_util.hpp"

int test_util::test_mean() {
    double x[] = {1.0};
    testequalish("single element array", 1.0, util::mean(x, 1));
    testequal("length 0", 0, util::mean(x, 0));
    testequal("length 0 null pointer", 0, util::mean(NULL, 0));

    double y[] = {-3.0, 0.0, 1.0, 4.0};
    testequalish("4 element array", 0.5, util::mean(y, 4));
    testequalish("subset of array", -2.0/3, util::mean(y, 3));

    double z[] = {2.0, 2.5, 3.0};
    std::vector<double> vz(z, z+3);
    testequalish("vector", 2.5, util::mean(vz));

    return EXIT_SUCCESS;
}

int test_util::test_var() {
    double x[] = {1.0};
    testequal("single element array", 0, util::var(x, 1));
    testequal("length 0", 0, util::var(x, 0));
    testequal("length 0 null pointer", 0, util::var(NULL, 0));

    double y[] = {-3.0, 0.0, 1.0, 4.0};
    testequalish("subset of array", 4.333333333333333, util::var(y, 3));
    testequalish("4 element array", 8.333333333333333, util::var(y, 4));

    double z[] = {2.0, 2.5, 3.0};
    std::vector<double> vz(z, z+3);
    testequalish("vector", 0.25, util::var(vz));

    return EXIT_SUCCESS;
}

int test_util::test_std() {
    double x[] = {1.0};
    testequal("single element array", 0, util::std(x, 1));
    testequal("length 0", 0, util::std(x, 0));
    testequal("length 0 null pointer", 0, util::std(NULL, 0));

    double y[] = {-3.0, 0.0, 1.0, 4.0};
    testequalish("4 element array", 2.886751345948129, util::std(y, 4));
    testequalish("subset of array", 2.081665999466133, util::std(y, 3));

    double z[] = {2.0, 2.5, 3.0};
    std::vector<double> vz(z, z+3);
    testequalish("vector", 0.5, util::std(vz));

    return EXIT_SUCCESS;
}

int test_util::run_all() {
    int fails = 0;
    testfn(test_util::test_mean, "util::mean()");
    testfn(test_util::test_var, "util::var()");
    testfn(test_util::test_std, "util::std()");
    return fails;
}

