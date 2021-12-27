cmake_minimum_required(VERSION 3.21)

project(advent_of_code_2021
    VERSION 1.0
    DESCRIPTION "Advent of code 2021")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    src
    inc)

add_subdirectory(src)