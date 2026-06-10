#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include "archiver.h"

using namespace std;
namespace fs = filesystem;

void printHelp() {
    cout << "Как использовать архиватор:\n"
         << "  Сжать:       ./huffman_archiver -c <что_сжать> <имя_архива>\n"
         << "  Распаковать: ./huffman_archiver -d <архив> <куда_распаковать>\n"
         << "  Тест:        ./huffman_archiver -b <файл_для_теста>\n";
}

void runExperiment(const string& filename) {
    if (!fs::exists(filename)) {
        cout << "Ошибка: файл  не найден\n";
        return;
    }

    string archiveName = filename + ".huff";
    string unpackedName = filename + ".unpacked";

    auto startCompress = chrono::high_resolution_clock::now();
    compressFile(filename, archiveName);
    auto endCompress = chrono::high_resolution_clock::now();

    auto startDecompress = chrono::high_resolution_clock::now();
    decompressFile(archiveName, unpackedName);
    auto endDecompress = chrono::high_resolution_clock::now();


    uintmax_t sizeOriginal = fs::file_size(filename);
    uintmax_t sizeArchive = fs::file_size(archiveName);
    double ratio = 0;
    if (sizeOriginal > 0) {
        ratio = (static_cast<double>(sizeArchive) / sizeOriginal) * 100.0;
    }

    chrono::duration<double, milli> timeCompress = endCompress - startCompress;
    chrono::duration<double, milli> timeDecompress = endDecompress - startDecompress;

    cout << "\n=== РЕЗУЛЬТАТЫ ЭКСПЕРИМЕНТА ===\n";
    cout << "Файл: " << filename << "\n";
    cout << "Размер оригинала: " << sizeOriginal << " байт\n";
    cout << "Размер архива:    " << sizeArchive << " байт\n";
    cout << "Степень сжатия:   " << ratio << "% от оригинала\n";
    cout << "Время сжатия:     " << timeCompress.count() << " мс\n";
    cout << "Время распаковки: " << timeDecompress.count() << " мс\n";
    cout << "===============================\n\n";

    fs::remove(archiveName);
    fs::remove(unpackedName);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc < 3) {
        printHelp();
        return 1;
    }

    string mode = argv[1];

    if (mode == "-b") {
        runExperiment(argv[2]);
    }
    else if (mode == "-c" && argc == 4) {
        cout << "Архивация началась\n";
        if (compressFile(argv[2], argv[3])) {
            cout << "Готово! Файл сжат.\n";
        }
    }
    else if (mode == "-d" && argc == 4) {
        cout << "Распаковка началась\n";
        if (decompressFile(argv[2], argv[3])) {
            cout << "Файл восстановлен.\n";
        }
    }
    else {
        printHelp();
        return 1;
    }

    return 0;
}