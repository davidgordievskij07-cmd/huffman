cmake_minimum_required(VERSION 3.16)
project(huffman_archiver CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

option(BUILD_TESTS "Собрать тесты" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
