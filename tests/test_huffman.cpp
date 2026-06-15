#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "archiver.h"
#include "bit_io.h"
#include "huffman_tree.h"

namespace fs = std::filesystem;

static std::string makeTempPath(const std::string& s) {
    return (fs::temp_directory_path() / ("huff_" + s)).string();
}
static void writeFile(const std::string& p, const std::string& c) {
    std::ofstream f(p, std::ios::binary);
    f.write(c.data(), static_cast<std::streamsize>(c.size()));
}
static std::string readFile(const std::string& p) {
    std::ifstream f(p, std::ios::binary);
    return {std::istreambuf_iterator<char>(f), {}};
}

TEST_CASE("BitWriter + BitReader: roundtrip", "[bitio]") {
    const std::vector<int> bits = {1, 0, 1, 1, 0, 0, 1, 0, 1, 1};
    std::stringstream ss;
    { BitWriter w(ss); for (int b : bits) w.writeBit(b); }
    BitReader r(ss);
    for (int expected : bits) REQUIRE(r.readBit() == expected);
}

TEST_CASE("BitWriter + BitReader: пустой поток", "[bitio]") {
    std::stringstream ss;
    { BitWriter w(ss); }
    BitReader r(ss);
    REQUIRE(r.readBit() == -1);
}

TEST_CASE("BitWriter + BitReader: ровно 8 бит", "[bitio]") {
    std::stringstream ss;
    { BitWriter w(ss); for (int b : {1,0,1,0,1,0,1,0}) w.writeBit(b); }
    BitReader r(ss);
    for (int e : {1,0,1,0,1,0,1,0}) REQUIRE(r.readBit() == e);
    REQUIRE(r.readBit() == -1);
}

TEST_CASE("HuffmanTree: пустые частоты", "[huffman]") {
    std::vector<uint64_t> freqs(256, 0);
    HuffmanTree tree(freqs);
    std::vector<std::string> codes(256);
    tree.getCodes(codes);
    for (const auto& c : codes) REQUIRE(c.empty());
}

TEST_CASE("HuffmanTree: единственный символ", "[huffman]") {
    std::vector<uint64_t> freqs(256, 0);
    freqs['X'] = 42;
    HuffmanTree tree(freqs);
    std::vector<std::string> codes(256);
    tree.getCodes(codes);
    REQUIRE(codes['X'] == "0");
}

TEST_CASE("HuffmanTree: коды prefix-free", "[huffman]") {
    std::vector<uint64_t> freqs(256, 0);
    freqs['A']=5; freqs['B']=3; freqs['C']=2; freqs['D']=1;
    HuffmanTree tree(freqs);
    std::vector<std::string> codes(256);
    tree.getCodes(codes);
    for (int i = 0; i < 256; ++i) {
        if (codes[i].empty()) continue;
        for (int j = 0; j < 256; ++j) {
            if (i == j || codes[j].empty()) continue;
            REQUIRE(codes[i].rfind(codes[j], 0) != 0);
        }
    }
}

TEST_CASE("Archiver: пустой файл", "[archiver]") {
    auto src=makeTempPath("e_src.bin"), arc=makeTempPath("e.huff"), dst=makeTempPath("e_dst.bin");
    writeFile(src, "");
    REQUIRE(compressFile(src, arc));
    REQUIRE(decompressFile(arc, dst));
    REQUIRE(readFile(dst) == "");
    fs::remove(src); fs::remove(arc); fs::remove(dst);
}

TEST_CASE("Archiver: один уникальный символ", "[archiver]") {
    auto src=makeTempPath("s_src.bin"), arc=makeTempPath("s.huff"), dst=makeTempPath("s_dst.bin");
    const std::string data(1000, '\xAB');
    writeFile(src, data);
    REQUIRE(compressFile(src, arc));
    REQUIRE(decompressFile(arc, dst));
    REQUIRE(readFile(dst) == data);
    fs::remove(src); fs::remove(arc); fs::remove(dst);
}

TEST_CASE("Archiver: текстовые данные", "[archiver]") {
    auto src=makeTempPath("t_src.txt"), arc=makeTempPath("t.huff"), dst=makeTempPath("t_dst.txt");
    const std::string data = "Huffman encoding is a lossless data compression algorithm.";
    writeFile(src, data);
    REQUIRE(compressFile(src, arc));
    REQUIRE(decompressFile(arc, dst));
    REQUIRE(readFile(dst) == data);
    fs::remove(src); fs::remove(arc); fs::remove(dst);
}

TEST_CASE("Archiver: все 256 байт", "[archiver]") {
    auto src=makeTempPath("b_src.bin"), arc=makeTempPath("b.huff"), dst=makeTempPath("b_dst.bin");
    std::string data;
    for (int i = 0; i < 256; ++i) data += static_cast<char>(i);
    writeFile(src, data);
    REQUIRE(compressFile(src, arc));
    REQUIRE(decompressFile(arc, dst));
    REQUIRE(readFile(dst) == data);
    fs::remove(src); fs::remove(arc); fs::remove(dst);
}

TEST_CASE("Archiver: ошибка при отсутствующем файле", "[archiver]") {
    REQUIRE_FALSE(compressFile("/no/such/file.txt", "/tmp/out.huff"));
    REQUIRE_FALSE(decompressFile("/no/such/archive.huff", "/tmp/out.bin"));
}
