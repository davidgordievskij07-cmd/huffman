#include "archiver.h"
#include "huffman_tree.h"
#include "bit_io.h"
#include <vector>
#include <fstream>
#include <iostream>

bool compressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл для чтения." << std::endl;
        return false;
    }

    std::vector<uint64_t> freqs(256, 0);
    uint64_t totalSymbols = 0;
    char ch;

    while (in.get(ch)) {
        freqs[static_cast<uint8_t>(ch)]++;
        totalSymbols++;
    }

    std::ofstream out(outputFile, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Ошибка: не удалось создать архив." << std::endl;
        return false;
    }


    out.write(reinterpret_cast<const char*>(&totalSymbols), sizeof(totalSymbols));
    if (totalSymbols == 0) return true;


    uint16_t uniqueSymbols = 0;
    for (int i = 0; i < 256; i++) {
        if (freqs[i] > 0) uniqueSymbols++;
    }


    out.write(reinterpret_cast<const char*>(&uniqueSymbols), sizeof(uniqueSymbols));
    for (int i = 0; i < 256; i++) {
        if (freqs[i] > 0) {
            uint8_t symbol = static_cast<uint8_t>(i);
            out.write(reinterpret_cast<const char*>(&symbol), sizeof(symbol));
            out.write(reinterpret_cast<const char*>(&freqs[i]), sizeof(freqs[i]));
        }
    }


    HuffmanNode* root = buildTree(freqs);
    std::vector<std::string> codes(256, "");
    getCodes(root, "", codes);


    in.clear();
    in.seekg(0, std::ios::beg);


    BitWriter writer(out);
    while (in.get(ch)) {
        std::string code = codes[static_cast<uint8_t>(ch)];
        for (char c : code) {
            if (c == '1') writer.writeBit(1);
            else writer.writeBit(0);
        }
    }
    writer.flush();

    freeTree(root);
    return true;
}

bool decompressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Ошибка: не удалось открыть архив." << std::endl;
        return false;
    }


    uint64_t totalSymbols = 0;
    if (!in.read(reinterpret_cast<char*>(&totalSymbols), sizeof(totalSymbols))) return false;

    std::ofstream out(outputFile, std::ios::binary);
    if (!out.is_open()) return false;

    if (totalSymbols == 0) return true;


    uint16_t uniqueSymbols = 0;
    in.read(reinterpret_cast<char*>(&uniqueSymbols), sizeof(uniqueSymbols));

    std::vector<uint64_t> freqs(256, 0);
    for (int i = 0; i < uniqueSymbols; i++) {
        uint8_t symbol;
        uint64_t freq;
        in.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
        in.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        freqs[symbol] = freq;
    }


    HuffmanNode* root = buildTree(freqs);
    BitReader reader(in);


    uint64_t decodedCount = 0;
    while (decodedCount < totalSymbols) {
        HuffmanNode* current = root;


        while (current->left != nullptr && current->right != nullptr) {
            int bit = reader.readBit();
            if (bit == -1) break;

            if (bit == 0) current = current->left;
            else current = current->right;
        }

        out.put(static_cast<char>(current->ch));
        decodedCount++;
    }

    freeTree(root);
    return true;
}