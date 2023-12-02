cmake_minimum_required(VERSION 3.6)

project(advent_of_code
    VERSION 1.0
    DESCRIPTION "Advent of code")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    inc
    InCommand/inc)

add_subdirectory(src)
add_subdirectory(InCommand)

if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT aocrunner)
endif()
