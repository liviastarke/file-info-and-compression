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

	// Verifica o retorno de 'generate_codes' e encerra o programa após liberar os recursos em caso de falha
	if (generate_codes(root, &table) != EXIT_SUCCESS) {
    	free_tree(root);
    	fclose(in_file);
    	return EXIT_FAILURE;
	}	

	// TODO: create out_file based on in_file path
	FILE * out_file = fopen("test_out_file", "w");
	check_pointer(out_file);
	
	// BITSTREAM.H
	BitWriter bw;
	bw_init(&bw, out_file);
	// Rewind in_file
	fseek(in_file, 0L, SEEK_SET);
	// read chars from in_file and encode according to table
	int c;
	while ((c = fgetc(in_file)) != EOF) {
		// look up current char in table
		Code code = table.table[(unsigned char) c];
		// write with bw_write_bits
		bw_write_bits(&bw, code.bits, code.len);
	}
	bw_flush(&bw);

	// in_file no longer necessary, close
	fclose(in_file);
	fclose(out_file);

	// Assess valid encoding by decoding
	// Open a file to write and reopen out_file in read mode
	FILE * reconstruct_file = fopen("test_reconstruct_in_file", "w");
	check_pointer(reconstruct_file);
	FILE * compressed_file = fopen("test_out_file", "r");
	check_pointer(compressed_file);

	fseek(compressed_file, 0L, SEEK_END);
	size_t file_len_bytes = ftell(compressed_file);
	fseek(compressed_file, 0L, SEEK_SET);
	decode_file(root, compressed_file, file_len_bytes*8, reconstruct_file);

	fclose(reconstruct_file);

	
	// out_file no longer necessary, close
	fclose(compressed_file);
	
	free_tree(root);
}

