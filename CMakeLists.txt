cmake_minimum_required(VERSION 3.23)
project(Feature_Selection)

set(CMAKE_CXX_STANDARD 14)

add_executable(Feature_Selection src/main.cpp src/node.h src/problem.h src/helperfunctions.h)
