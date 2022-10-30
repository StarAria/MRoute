cmake_minimum_required (VERSION 2.8.11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )
set (CMAKE_VERBOSE_MAKEFILE ON)

set(PROJECT_NAME "MRoute")
project(${PROJECT_NAME})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-std=c++14")

include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

file (GLOB SOURCES
    src/*
    src/parser/*
    src/db/*
    src/global/*
    src/maze_route/*
    src/pattern_match/*
    src/main/*
)

add_executable(${PROJECT_NAME} ${SOURCES})

