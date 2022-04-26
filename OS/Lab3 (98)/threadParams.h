//
// Created by amayn on 29.03.2022.
//

#include <cstdlib>

#ifndef LAB2__98__THREADPARAMS_H
#define LAB2__98__THREADPARAMS_H

#endif //LAB2__98__THREADPARAMS_H

struct threadParams{
    threadParams() : id(-1), arr(NULL), n(-1) {}
    threadParams(int threadId, int *arr, int n) : id(threadId), arr(arr), n(n) {}

    int id;

    int* arr;
    int n;
};