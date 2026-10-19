#ifndef HEADER_IO_H
#define HEADER_IO_H

#include "common.h"
#include <stdint.h>
#include <stdio.h>

void count_frequencies(FILE *in, uint64_t freqs[ALPHABET_SIZE]); //  frequência de caracteres do arquivo de entrada

void write_file_header(FILE * file, uint64_t freqs[ALPHABET_SIZE], uint64_t num_symbols_in_file);

// Read magic bytes and verify it's a valid file
void check_file_signature(FILE * compressed_file);

// Get num of symbols to decode
uint64_t get_num_symbols_from_header(FILE * compressed_file);

// read frequencies from original uncompressed file to freqs array
void get_frequencies_from_header(FILE * compressed_file, uint64_t freqs[ALPHABET_SIZE]);

// return header size in bytes
long calc_header_size(uint8_t num_freqs);

#endif