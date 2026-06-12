#include "bit_writer.h"
#include "bit_reader.h"


BitWriter::BitWriter(std::ostream& os) : out(os), buffer(0), bit_count(0) {}

BitWriter::~BitWriter() {
    flush();
}

void BitWriter::writeBit(int bit) {
    if (bit) {
        buffer |= (1 << (7 - bit_count));
    }
    bit_count++;

    if (bit_count == 8) {
        out.put(static_cast<char>(buffer));
        buffer = 0;
        bit_count = 0;
    }
}

void BitWriter::flush() {
    if (bit_count > 0) {
        out.put(static_cast<char>(buffer));
        buffer = 0;
        bit_count = 0;
    }
    out.flush();
}


BitReader::BitReader(std::istream& is) : in(is), buffer(0), bit_count(8) {}

int BitReader::readBit() {
    if (bit_count == 8) {
        char ch;
        if (!in.get(ch)) {
            return -1;
        }
        buffer = static_cast<uint8_t>(ch);
        bit_count = 0;
    }

    int bit = (buffer >> (7 - bit_count)) & 1;
    bit_count++;
    return bit;
}