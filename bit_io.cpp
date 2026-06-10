#include "bit_io.h"

BitWriter::BitWriter(std::ofstream& output) : out(output), buffer(0), bitCount(0) {}

void BitWriter::writeBit(int bit) {

    buffer = buffer << 1;


    if (bit == 1) {
        buffer = buffer | 1;
    }
    bitCount++;


    if (bitCount == 8) {
        out.put(static_cast<char>(buffer));
        buffer = 0;
        bitCount = 0;
    }
}

void BitWriter::flush() {

    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount);
        out.put(static_cast<char>(buffer));
        buffer = 0;
        bitCount = 0;
    }
}

BitReader::BitReader(std::ifstream& input) : in(input), buffer(0), bitCount(8) {}

int BitReader::readBit() {

    if (bitCount == 8) {
        char ch;
        if (!in.get(ch)) {
            return -1;
        }
        buffer = static_cast<uint8_t>(ch);
        bitCount = 0;
    }

    int bit = (buffer >> (7 - bitCount)) & 1;
    bitCount++;
    return bit;
}