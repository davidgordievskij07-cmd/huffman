#pragma once
#include <iostream>
#include <cstdint>

class BitWriter {
public:
    explicit BitWriter(std::ostream& os);
    ~BitWriter();

    void writeBit(int bit);
    void flush();

private:
    std::ostream& out;
    uint8_t buffer;
    int bit_count;
};