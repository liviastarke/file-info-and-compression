#ifndef __TABLE_H__
#define __TABLE_H__

#include "common.h"

int generate_codes(const HuffmanNode *root, HuffmanTable *out_table);
int huffman_encode(const HuffmanNode * node, uint32_t current_code, uint8_t depth, HuffmanTable * codes);
void print_table(HuffmanTable * table);
void print_code(Code * code);
// Optional/Bonus: direct tree-based decoder for verification
// void decode_stream(const HuffmanNode *root, const uint8_t *bits, size_t bit_len, FILE *out);

#endif
