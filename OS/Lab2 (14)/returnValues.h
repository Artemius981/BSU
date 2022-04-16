//
// Created by amayn on 02.04.2022.
//

#include <climits>

#ifndef LAB2__14__RETURNVALUES_H
#define LAB2__14__RETURNVALUES_H

struct returnValues{
    returnValues() : min(INT_MIN), max(INT_MAX) {}

    double min;
    double max;
    double average;
};

#endif //LAB2__14__RETURNVALUES_H