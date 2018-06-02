cmake_minimum_required(VERSION 3.10)
project(Huff)

set(CMAKE_CXX_STANDARD 98)

add_executable(Huff main.cpp heap.cpp heap.h HuffmanCoding.cpp HuffmanCoding.h  mergingTree.cpp  mergingTree.h mergingTreeNode.h mergingType.h)