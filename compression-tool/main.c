#include "main.h"

#include "common.h"

// Include modules
#include "pq.h"
#include "tree.h"
#include "table.h"
#include "bitstream.h"
#include "header_io.h"

// Include other
#include "check_pointer.h"

// Include C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){

	// UNHAPPY PATH: check correct arguments were passed
	if (argc != 3){
		printf("HUFFMAN COMPRESSION: use comand + file path to compress a text file\n");
		exit(EXIT_FAILURE);
	}

	int fail_flag;

	// Open file
	char* path = argv[2];
	FILE * in_file = fopen(path, "r");
	check_pointer(in_file);

	if (strcmp(argv[1], "-c") == 0){
		/* Compression (Huffman encode) */

		// TODO: create out_file based on in_file path
		FILE * out_file = fopen("test_out_file", "w");
		check_pointer(out_file);

		fail_flag = compress(in_file, out_file);
		fclose(out_file);
	}
	else if (strcmp(argv[1], "-d") == 0){
		/* Decompression (Huffman decode) */

		// TODO: create out_file based on in_file path
		FILE * reconstruct_file = fopen("test_reconstruct_in_file", "w");
		check_pointer(reconstruct_file);

		fail_flag = decompress(in_file, reconstruct_file);
		fclose(reconstruct_file);
	}
	else{
		printf("Failed: Could not parse arguments.\n");
		fail_flag = EXIT_FAILURE;
	}

	// in_file no longer necessary, close
	fclose(in_file);

	return fail_flag;
}

int compress(FILE * in_file, FILE * out_file){
	// Count char frequencies from file to build Huffman tree
	uint64_t freqs[ALPHABET_SIZE] = {0};
	count_frequencies(in_file, freqs); // BITSTREAM.H
	
	HuffmanNode *root = build_tree_from_frequencies(freqs); // TREE.H
	
	// TABLE.H
	HuffmanTable table = {0};
	// Verifica o retorno de 'generate_codes' e encerra o programa após liberar os recursos em caso de falha
	if (generate_codes(root, &table) != EXIT_SUCCESS) {
    	free_tree(root);
    	return EXIT_FAILURE;
	}

	// BITSTREAM.H
	BitWriter bw;
	bw_init(&bw, out_file);
	
	// get in_file size
	fseek(in_file, 0L, SEEK_END);
	long in_file_size = ftell(in_file);
	fseek(in_file, 0L, SEEK_SET); // back to begining of file

	// write file header
	write_file_header(out_file, freqs, in_file_size);

	// read chars from in_file and encode according to table
	int c;
	while ((c = fgetc(in_file)) != EOF) {
		// look up current char in table
		Code code = table.table[(unsigned char) c];
		// write with bw_write_bits
		bw_write_bits(&bw, code.bits, code.len);
	}
	bw_flush(&bw);

	free_tree(root);
	return EXIT_SUCCESS;
}

int decompress(FILE * compressed_file, FILE * out_file){
	// Exit if file signature doesn't match
	check_file_signature(compressed_file);
	uint64_t num_symbols = get_num_symbols_from_header(compressed_file);

	// Count char frequencies from file to build Huffman tree
	uint64_t freqs[ALPHABET_SIZE] = {0};
	get_frequencies_from_header(compressed_file, freqs); // MAIN.H
	
	HuffmanNode *root = build_tree_from_frequencies(freqs); // TREE.H

	// fseek(compressed_file, calc_header_size(num_freqs), SEEK_SET);
	// Could do it this way, but it cost me some much time debugging that
	// I feel like letting the pointer untouched after get_frequencies_from_header,
	// where it should end in the right spot

	int success = decode_file(root, compressed_file, num_symbols, out_file);

	free_tree(root);
	return success;
}
