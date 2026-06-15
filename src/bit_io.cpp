#include "bit_io.h"

BitWriter::BitWriter(std::ostream& os) : out_(os), buffer_(0), bitCount_(0) {}

BitWriter::~BitWriter() {
    flush();
}

void BitWriter::writeBit(int bit) {
    if (bit) {
        buffer_ |= static_cast<uint8_t>(1 << (7 - bitCount_));
    }
    ++bitCount_;
    if (bitCount_ == 8) {
        out_.put(static_cast<char>(buffer_));
        buffer_ = 0;
        bitCount_ = 0;
    }
}

void BitWriter::flush() {
    if (bitCount_ > 0) {
        out_.put(static_cast<char>(buffer_));
        buffer_ = 0;
        bitCount_ = 0;
    }
}

BitReader::BitReader(std::istream& is) : in_(is), buffer_(0), bitCount_(8) {}

int BitReader::readBit() {
    if (bitCount_ == 8) {
        char ch;
        if (!in_.get(ch)) {
            return -1;
        }
        buffer_ = static_cast<uint8_t>(ch);
        bitCount_ = 0;
    }
    int bit = (buffer_ >> (7 - bitCount_)) & 1;
    ++bitCount_;
    return bit;
}
