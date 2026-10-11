#ifndef TREE_H
#define TREE_H

#include "common.h"

HuffmanNode *create_leaf(unsigned char ch, uint64_t freq);
HuffmanNode *node_merge(HuffmanNode *left, HuffmanNode *right);
HuffmanNode *build_tree_from_frequencies(const uint64_t freqs[ALPHABET_SIZE]);
void free_tree(HuffmanNode *root);

#endif

