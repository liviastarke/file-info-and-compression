#ifndef MAIN_H
#define MAIN_H

#include "common.h"
#include <stdio.h>

// should this write function go to bitstream.h instead?
void write_file_header(FILE * file, uint64_t freqs[ALPHABET_SIZE]);

#endif