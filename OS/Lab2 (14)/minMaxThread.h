//
// Created by amayn on 02.04.2022.
//

#include "returnValues.h"
#include <iostream>
#include <chrono>
#include <thread>

#ifndef LAB2__14__MINMAX_H
#define LAB2__14__MINMAX_H

class minMaxThread{
public:
    void operator() (double* arr, int n, returnValues& ret){
        double min = INT_MAX;
        double max = INT_MIN;

        for (int i = 0; i < n; ++i)
        {
            if (arr[i] < min) min = arr[i];
            if (arr[i] > max) max = arr[i];
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        std::cout << "min is : " << min << std::endl;
        std::cout << "max is : " << max << std::endl;

        ret.min = min;
        ret.max = max;

    }
};

#endif //LAB2__14__MINMAX_H