#include "huffman_tree.h"
#include <queue>

HuffmanNode::HuffmanNode(uint8_t character, uint64_t frequency) {
    ch = character;
    freq = frequency;
    left = nullptr;
    right = nullptr;
}

HuffmanNode::HuffmanNode(uint64_t frequency, HuffmanNode* l, HuffmanNode* r) {
    ch = 0;
    freq = frequency;
    left = l;
    right = r;
}

bool CompareNodes::operator()(HuffmanNode* n1, HuffmanNode* n2) {
    return n1->freq > n2->freq;
}

HuffmanNode* buildTree(const std::vector<uint64_t>& freqs) {

    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> pq;


    for (int i = 0; i < 256; i++) {
        if (freqs[i] > 0) {
            pq.push(new HuffmanNode(static_cast<uint8_t>(i), freqs[i]));
        }
    }

    if (pq.empty()) return nullptr;


    if (pq.size() == 1) {
        HuffmanNode* singleNode = pq.top();
        pq.pop();
        return new HuffmanNode(singleNode->freq, singleNode, nullptr);
    }


    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();

        HuffmanNode* right = pq.top();
        pq.pop();


        HuffmanNode* parent = new HuffmanNode(left->freq + right->freq, left, right);
        pq.push(parent);
    }


    return pq.top();
}

void getCodes(HuffmanNode* root, const std::string& currentCode, std::vector<std::string>& codes) {
    if (root == nullptr) return;


    if (root->left == nullptr && root->right == nullptr) {
        codes[root->ch] = currentCode.empty() ? "0" : currentCode;
        return;
    }

    getCodes(root->left, currentCode + "0", codes);
    getCodes(root->right, currentCode + "1", codes);
}

void freeTree(HuffmanNode* root) {
    if (root == nullptr) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}