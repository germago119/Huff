cmake_minimum_required(VERSION 3.1)
project(Huff)

set(CMAKE_CXX_STANDARD 11)

add_executable(Huff main.cpp HuffmanCode.cpp HuffmanCode.h Test.h)