//
// Created by amayn on 02.04.2022.
//

#include "returnValues.h"
#include <iostream>
#include <chrono>
#include <thread>

#ifndef LAB2__14__AVERAGETHREAD_H
#define LAB2__14__AVERAGETHREAD_H


class averageThread {
public:
    void operator() (double* arr, int n, returnValues& ret){
        double average = 0.0;

        for (int i = 0; i < n; ++i)
        {
            average += arr[i];
            std::this_thread::sleep_for(std::chrono::milliseconds(12));
        }
        if (n != 0) average /= n;
        std::cout << "Average is : " << average << std::endl;
        ret.average = average;
    }

};


#endif //LAB2__14__AVERAGETHREAD_H
