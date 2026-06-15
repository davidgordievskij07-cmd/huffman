#pragma once
#include <string>

/**
 * @brief Сжимает файл алгоритмом Хаффмана.
 * @param inputFile  Путь к исходному файлу.
 * @param outputFile Путь к создаваемому архиву.
 * @return true при успехе, false при ошибке.
 */
bool compressFile(const std::string& inputFile, const std::string& outputFile);

/**
 * @brief Распаковывает архив, созданный compressFile().
 * @param inputFile  Путь к архиву.
 * @param outputFile Путь к выходному файлу.
 * @return true при успехе, false при ошибке.
 */
bool decompressFile(const std::string& inputFile, const std::string& outputFile);
