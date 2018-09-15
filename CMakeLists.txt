cmake_minimum_required(VERSION 3.12)
project(huffmancito)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/util)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test)

add_library(huffman-coding STATIC
        lib/huffman.h
        lib/huffman.cpp lib/Tree.cpp lib/Tree.h)

add_executable(huffman
        util/util.cpp)

add_executable(huffman-testing
        test/gtest/gtest-all.cc
        test/gtest/gtest.h
        test/gtest/gtest_main.cc
        test/test.cpp
        )

target_link_libraries(huffman huffman-coding)
target_link_libraries(huffman-testing huffman-coding -lpthread)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE}")
endif()