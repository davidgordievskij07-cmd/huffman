include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(huffman_tests test_huffman.cpp)
target_link_libraries(huffman_tests PRIVATE huffman_lib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(huffman_tests)
