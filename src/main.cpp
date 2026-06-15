#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include "archiver.h"

namespace fs = std::filesystem;

static void printHelp() {
    std::cout << "Использование:\n"
              << "  Сжатие:    huffman_archiver -c <файл> <архив.huff>\n"
              << "  Распаковка: huffman_archiver -d <архив.huff> <файл>\n"
              << "  Тест:      huffman_archiver -b <файл>\n";
}

static void runBenchmark(const std::string& filename) {
    if (!fs::exists(filename)) {
        std::cerr << "Ошибка: файл не найден: " << filename << '\n';
        return;
    }
    const std::string archiveName = filename + ".huff";
    const std::string unpackedName = filename + ".unpacked";

    auto t0 = std::chrono::high_resolution_clock::now();
    const bool compressOk = compressFile(filename, archiveName);
    auto t1 = std::chrono::high_resolution_clock::now();
    if (!compressOk) return;

    const bool decompressOk = decompressFile(archiveName, unpackedName);
    auto t2 = std::chrono::high_resolution_clock::now();
    if (!decompressOk) { fs::remove(archiveName); return; }

    const auto sizeOriginal = fs::file_size(filename);
    const auto sizeArchive  = fs::file_size(archiveName);
    const double ratio = sizeOriginal > 0
        ? static_cast<double>(sizeArchive) / sizeOriginal * 100.0 : 0.0;

    std::chrono::duration<double, std::milli> tC = t1 - t0;
    std::chrono::duration<double, std::milli> tD = t2 - t1;

    std::cout << "\n=== БЕНЧМАРК ===\n"
              << "Оригинал:       " << sizeOriginal << " байт\n"
              << "Архив:          " << sizeArchive  << " байт\n"
              << "Степень сжатия: " << ratio        << "%\n"
              << "Сжатие:         " << tC.count()   << " мс\n"
              << "Распаковка:     " << tD.count()   << " мс\n"
              << "================\n";

    fs::remove(archiveName);
    fs::remove(unpackedName);
}

int main(int argc, char* argv[]) {
    if (argc < 3) { printHelp(); return 1; }
    const std::string mode = argv[1];
    if (mode == "-b") {
        runBenchmark(argv[2]);
    } else if (mode == "-c" && argc == 4) {
        if (!compressFile(argv[2], argv[3])) return 1;
        std::cout << "Готово: файл сжат в " << argv[3] << '\n';
    } else if (mode == "-d" && argc == 4) {
        if (!decompressFile(argv[2], argv[3])) return 1;
        std::cout << "Готово: файл распакован в " << argv[3] << '\n';
    } else {
        printHelp(); return 1;
    }
    return 0;
}
