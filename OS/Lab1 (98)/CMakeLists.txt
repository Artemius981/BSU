cmake_minimum_required(VERSION 3.21)
project(Lab1__98_)

set(CMAKE_CXX_STANDARD 98)

add_executable(creator creator.cpp employee.h errorsAPI.h)
add_executable(reporter reporter.cpp employee.h errorsAPI.h)
add_executable(main main.cpp employee.h errorsAPI.h)
