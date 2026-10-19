#ifndef MAIN_H
#define MAIN_H

#include "common.h"
#include <stdio.h>

// return EXIT_FAILURE/EXIT_SUCCESS; write and read but do not close files
int compress(FILE * in_file, FILE * out_file);
int decompress(FILE * compressed_file, FILE * out_file);


#endif