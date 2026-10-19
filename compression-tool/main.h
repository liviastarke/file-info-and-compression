#ifndef MAIN_H
#define MAIN_H

#include "common.h"
#include <stdio.h>


// return EXIT_FAILURE/EXIT_SUCCESS; write and read but do not close files
int compress(FILE * in_file, FILE * out_file);
int decompress(FILE * compressed_file, FILE * out_file);


// this should go to file_io.h alognside count_frequencies
void write_file_header(FILE * file, uint64_t freqs[ALPHABET_SIZE], uint64_t num_symbols_in_file);

// read file header -> return freqs and file size, fail if magic bytes don't match

// Read magic bytes and verify it's a "HU" file
void check_file_signature(FILE * compressed_file);
// Get num of symbols to decode
uint64_t get_num_symbols_from_header(FILE * compressed_file);
// Initialize an empty freqs[256] = {0} array
// Read unique_chars
// Loop unique_chars times: read 1 byte (char), read 4 bytes (freq), update freqs array.
// Return to pass freqs array into your existing build_tree_from_frequencies()
void get_frequencies_from_header(FILE * compressed_file, uint64_t freqs[ALPHABET_SIZE]);

long calc_header_size(uint8_t num_freqs);


#endif