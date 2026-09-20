#ifndef __COMMON_H__
#define __COMMON_H__

#include <stddef.h>
#include <stdint.h>

#define ALPHABET_SIZE 256

// Represents a node in the Huffman Tree
typedef struct HuffmanNode {
    unsigned char ch;
    int freq;
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} HuffmanNode;

// Bit-level representation for a single character
typedef struct {
    uint32_t bits; // Up to 32 bits (Huffman paths rarely exceed 32)
    uint8_t len;   // Number of valid bits in `bits`
} Code;

// Global translation lookup: char code -> bit sequence
typedef struct {
    Code table[ALPHABET_SIZE];
} HuffmanTable;

#endif