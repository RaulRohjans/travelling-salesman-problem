cmake_minimum_required(VERSION 3.27)
project(travelling_salesman_problem C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(travelling_salesman_problem
        main.c)
