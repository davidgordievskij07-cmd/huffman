#pragma once
#include <vector>
#include <string>
#include <cstdint>

struct HuffmanNode {
    uint8_t ch;
    uint64_t freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(uint8_t character, uint64_t frequency);

    HuffmanNode(uint64_t frequency, HuffmanNode* l, HuffmanNode* r);
};

struct CompareNodes {
    bool operator()(HuffmanNode* n1, HuffmanNode* n2);
};

HuffmanNode* buildTree(const std::vector<uint64_t>& freqs);

void getCodes(HuffmanNode* root, const std::string& currentCode, std::vector<std::string>& codes);

void freeTree(HuffmanNode* root);