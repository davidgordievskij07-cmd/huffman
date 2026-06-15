#include "huffman_tree.h"
#include <queue>

struct HuffmanTree::Node {
    uint8_t ch{0};
    uint64_t freq{0};
    Node* left{nullptr};
    Node* right{nullptr};

    Node(uint8_t c, uint64_t f) : ch(c), freq(f) {}
    Node(uint64_t f, Node* l, Node* r) : freq(f), left(l), right(r) {}
};

HuffmanTree::HuffmanTree(const std::vector<uint64_t>& freqs) {
    auto cmp = [](const Node* a, const Node* b) {
        return a->freq > b->freq;
    };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> pq(cmp);

    for (int i = 0; i < 256; ++i) {
        if (freqs[i] > 0) {
            pq.push(new Node(static_cast<uint8_t>(i), freqs[i]));
        }
    }

    if (pq.empty()) return;

    if (pq.size() == 1) {
        Node* single = pq.top(); pq.pop();
        root_ = new Node(single->freq, single, nullptr);
        return;
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(new Node(left->freq + right->freq, left, right));
    }
    root_ = pq.top();
}

HuffmanTree::~HuffmanTree() {
    freeNodes(root_);
}

void HuffmanTree::freeNodes(Node* node) {
    if (!node) return;
    freeNodes(node->left);
    freeNodes(node->right);
    delete node;
}

void HuffmanTree::getCodes(std::vector<std::string>& codes) const {
    buildCodes(root_, "", codes);
}

void HuffmanTree::buildCodes(const Node* node, const std::string& prefix,
                              std::vector<std::string>& codes) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes[node->ch] = prefix.empty() ? "0" : prefix;
        return;
    }
    buildCodes(node->left, prefix + "0", codes);
    buildCodes(node->right, prefix + "1", codes);
}

int HuffmanTree::decodeSymbol(const std::function<int()>& readBit) const {
    if (!root_) return -1;
    const Node* current = root_;
    while (current->left || current->right) {
        int bit = readBit();
        if (bit == -1) return -1;
        const Node* next = (bit == 0) ? current->left : current->right;
        if (!next) return -1;
        current = next;
    }
    return static_cast<int>(static_cast<uint8_t>(current->ch));
}
