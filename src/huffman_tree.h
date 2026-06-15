#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

/**
 * @brief Дерево Хаффмана для статического кодирования/декодирования.
 *
 * Внутренняя структура узлов скрыта — доступен только публичный интерфейс.
 */
class HuffmanTree {
public:
    /**
     * @brief Строит дерево по таблице частот.
     * @param freqs Вектор из 256 элементов: freqs[i] — частота байта i.
     */
    explicit HuffmanTree(const std::vector<uint64_t>& freqs);

    ~HuffmanTree();
    HuffmanTree(const HuffmanTree&) = delete;
    HuffmanTree& operator=(const HuffmanTree&) = delete;

    /**
     * @brief Заполняет таблицу двоичных кодов.
     * @param[out] codes Вектор из 256 строк; codes[i] — код байта i.
     */
    void getCodes(std::vector<std::string>& codes) const;

    /**
     * @brief Декодирует один символ, читая биты через коллбэк.
     * @param readBit Функция, возвращающая 0, 1 или -1 при EOF.
     * @return Декодированный байт (0–255) или -1 при ошибке.
     */
    int decodeSymbol(const std::function<int()>& readBit) const;

private:
    struct Node;
    Node* root_{nullptr};

    static void buildCodes(const Node* node, const std::string& prefix,
                           std::vector<std::string>& codes);
    static void freeNodes(Node* node);
};
