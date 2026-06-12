#pragma once
#include <iostream>
#include <cstdint>

class BitReader {
public:
    explicit BitReader(std::istream& is);

    int readBit();

private:
    std::istream& in;
    uint8_t buffer;
    int bit_count;
};