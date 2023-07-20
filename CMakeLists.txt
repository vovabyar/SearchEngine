cmake_minimum_required(VERSION 3.26)
project(trie)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(trie
        Trie/sample.cc
        Trie/trie.cpp
        Trie/trie.hpp
        VideosManager.cpp)
add_subdirectory(Google_tests)