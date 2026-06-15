#include "archiver.h"
#include "huffman_tree.h"
#include "bit_io.h"
#include <fstream>
#include <iostream>
#include <vector>

bool compressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл: " << inputFile << '\n';
        return false;
    }

    std::vector<uint64_t> freqs(256, 0);
    uint64_t totalSymbols = 0;
    {
        char ch;
        while (in.get(ch)) {
            freqs[static_cast<uint8_t>(ch)]++;
            ++totalSymbols;
        }
    }

    std::ofstream out(outputFile, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Ошибка: не удалось создать архив: " << outputFile << '\n';
        return false;
    }

    out.write(reinterpret_cast<const char*>(&totalSymbols), sizeof(totalSymbols));
    if (totalSymbols == 0) return true;

    uint16_t uniqueSymbols = 0;
    for (const auto& f : freqs) {
        if (f > 0) ++uniqueSymbols;
    }

    out.write(reinterpret_cast<const char*>(&uniqueSymbols), sizeof(uniqueSymbols));
    for (int i = 0; i < 256; ++i) {
        if (freqs[i] > 0) {
            auto symbol = static_cast<uint8_t>(i);
            out.write(reinterpret_cast<const char*>(&symbol), sizeof(symbol));
            out.write(reinterpret_cast<const char*>(&freqs[i]), sizeof(freqs[i]));
        }
    }

    HuffmanTree tree(freqs);
    std::vector<std::string> codes(256);
    tree.getCodes(codes);

    in.clear();
    in.seekg(0, std::ios::beg);

    {
        BitWriter writer(out);
        char ch;
        while (in.get(ch)) {
            const std::string& code = codes[static_cast<uint8_t>(ch)];
            for (char bit : code) {
                writer.writeBit(bit == '1' ? 1 : 0);
            }
        }
    }
    return true;
}

bool decompressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Ошибка: не удалось открыть архив: " << inputFile << '\n';
        return false;
    }

    uint64_t totalSymbols = 0;
    if (!in.read(reinterpret_cast<char*>(&totalSymbols), sizeof(totalSymbols))) {
        std::cerr << "Ошибка: повреждённый заголовок архива.\n";
        return false;
    }

    std::ofstream out(outputFile, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл: " << outputFile << '\n';
        return false;
    }

    if (totalSymbols == 0) return true;

    uint16_t uniqueSymbols = 0;
    if (!in.read(reinterpret_cast<char*>(&uniqueSymbols), sizeof(uniqueSymbols))) return false;

    std::vector<uint64_t> freqs(256, 0);
    for (int i = 0; i < uniqueSymbols; ++i) {
        uint8_t symbol = 0;
        uint64_t freq = 0;
        if (!in.read(reinterpret_cast<char*>(&symbol), sizeof(symbol))) return false;
        if (!in.read(reinterpret_cast<char*>(&freq), sizeof(freq))) return false;
        freqs[symbol] = freq;
    }

    HuffmanTree tree(freqs);
    BitReader reader(in);
    auto readBit = [&]() { return reader.readBit(); };

    for (uint64_t i = 0; i < totalSymbols; ++i) {
        int sym = tree.decodeSymbol(readBit);
        if (sym == -1) {
            std::cerr << "Ошибка: неожиданный конец потока при декодировании.\n";
            return false;
        }
        out.put(static_cast<char>(sym));
    }
    return true;
}
