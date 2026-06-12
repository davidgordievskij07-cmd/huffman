cmake_minimum_required(VERSION 3.16)
project(huffman_archiver CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(huffman_archiver
    main.cpp
    bit_writer.h
    bit_reader.h
    bit_io.cpp
    huffman_tree.h
    huffman_tree.cpp
    archiver.h
    archiver.cpp
)
