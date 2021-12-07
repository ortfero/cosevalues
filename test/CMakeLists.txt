cmake_minimum_required(VERSION 3.20)
project(cosevalues-test)

set(CMAKE_CXX_STANDARD 17)

add_executable(cosevalues-test
        test.cpp
        ../include/cosevalues/cosevalues.hpp)

target_include_directories(cosevalues-test PRIVATE ../include)
