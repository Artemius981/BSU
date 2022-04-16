//
// Created by amayn on 29.03.2022.
//

#include <cstdlib>

#ifndef LAB2__98__THREADPARAMS_H
#define LAB2__98__THREADPARAMS_H

#endif //LAB2__98__THREADPARAMS_H

struct returnValues{
    returnValues() : min(INT_MIN), max(INT_MAX), average(0.0) {}

    double min;
    double max;
    double average;
};

struct threadParams{
    threadParams() : arr(NULL), n(-1), ret() {}
    threadParams(double *arr, int n) : arr(arr), n(n), ret() {}

    double* arr;
    int n;
    returnValues ret;
};