#pragma once

#include <cstdint>
#include <iostream>

/**
 * @brief Буферизованная побитовая запись в поток вывода.
 */
class BitWriter {
public:
    explicit BitWriter(std::ostream& os);
    ~BitWriter();
    BitWriter(const BitWriter&) = delete;
    BitWriter& operator=(const BitWriter&) = delete;

    /** @brief Записывает один бит (0 или 1). */
    void writeBit(int bit);

    /** @brief Сбрасывает буфер, дополняя последний байт нулями. */
    void flush();

private:
    std::ostream& out_;
    uint8_t buffer_{0};
    int bitCount_{0};
};

/**
 * @brief Буферизованное побитовое чтение из потока ввода.
 */
class BitReader {
public:
    explicit BitReader(std::istream& is);
    BitReader(const BitReader&) = delete;
    BitReader& operator=(const BitReader&) = delete;

    /** @brief Читает один бит. Возвращает 0, 1 или -1 при EOF. */
    int readBit();

private:
    std::istream& in_;
    uint8_t buffer_{0};
    int bitCount_{8};
};
