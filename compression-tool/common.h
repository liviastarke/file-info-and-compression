#ifndef __COMMON_H__
#define __COMMON_H__

#include <stddef.h>
#include <stdint.h>

#define ALPHABET_SIZE 256

#include <stddef.h>

// Represents a node in the Huffman Tree
typedef struct HuffmanNode {
    unsigned char ch;
    uint64_t freq;
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} HuffmanNode;

// Bit-level representation for a single character
typedef struct {
    uint32_t bits; // Huffman codes up to 32 bits
    uint8_t len;   // Number of valid bits in the code (length is variable, remember?)
} Code;

// Global translation lookup: char code -> bit sequence
typedef struct {
    Code table[ALPHABET_SIZE];
} HuffmanTable;

#endif