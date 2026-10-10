#ifndef TREE_H
#define TREE_H

#include "common.h"

HuffmanNode *create_leaf(unsigned char ch, int freq);
HuffmanNode *node_merge(HuffmanNode *left, HuffmanNode *right);
HuffmanNode *build_tree_from_frequencies(const int freqs[ALPHABET_SIZE]);
void free_tree(HuffmanNode *root);

#endif

