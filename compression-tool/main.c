#include "common.h"

// Include modules
#include "pq.h"
#include "tree.h"
#include "table.h"
#include "bitstream.h"

// Include other
#include "check_pointer.h"

// Include C libraries
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){

	// UNHAPPY PATH: check correct arguments were passed
	if (argc != 2){
		printf("HUFFMAN COMPRESSION: use comand + file path to compress a text file\n");
		exit(EXIT_FAILURE);
	}

	// Open file
	char* path = argv[1];
	FILE * in_file = fopen(path, "r");
	check_pointer(in_file);

	// Count char frequencies from file to build Huffman tree
	int freqs[ALPHABET_SIZE] = {0};
	count_frequencies(in_file, freqs); // BITSTREAM.H
	
	HuffmanNode *root = build_tree_from_frequencies(freqs); // TREE.H
	
	// TABLE.H
	HuffmanTable table;
	generate_codes(root, &table);

	// TODO: create out_file based on in_file path
	FILE * out_file = fopen("test_out_file", "w");
	check_pointer(out_file);
	
	// BITSTREAM.H
	BitWriter bw;
	bw_init(&bw, out_file);
	// Rewind in_file
	fseek(in_file, 0L, SEEK_SET);
	// read chars from in_file and encode according to table
	char c;
	while ((c = fgetc(in_file)) != EOF) {
		// look up current char in table and write with bw_write_bits
		bw_write_bits(&bw, table.table[c].bits, table.table[c].len);
	}
	bw_flush(&bw);
	fclose(in_file);
	fclose(out_file);
	
	free_tree(root);
}

