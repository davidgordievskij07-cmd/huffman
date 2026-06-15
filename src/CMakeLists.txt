add_library(huffman_lib
    bit_io.cpp
    huffman_tree.cpp
    archiver.cpp
)
target_include_directories(huffman_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(huffman_archiver main.cpp)
target_link_libraries(huffman_archiver PRIVATE huffman_lib)
