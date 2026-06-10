#pragma once
#include <fstream>
#include <cstdint>


class BitWriter {
private:
    std::ofstream& out;
    uint8_t buffer;
    int bitCount;

public:
    BitWriter(std::ofstream& output);
    void writeBit(int bit);
    void flush();
};


class BitReader {
private:
    std::ifstream& in;
    uint8_t buffer;
    int bitCount;

public:
    BitReader(std::ifstream& input);
    int readBit();
};