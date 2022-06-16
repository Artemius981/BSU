cmake_minimum_required(VERSION 3.21)
project(Lab4__98_)

set(CMAKE_CXX_STANDARD 98)

add_executable(Receiver receiver.cpp processMessage.h errors.h constants.h)
add_executable(Sender sender.cpp processMessage.h errors.h constants.h)
