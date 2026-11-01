#include "main.h"

#include "common.h"

// Include Huffman modules
#include "pq.h"
#include "tree.h"
#include "table.h"
#include "bitstream.h"

// Include LZ77 module
#include "lz77/lz77.h"

// Include header module
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
		printf("OLIVE, a command-line file compression tool.\n\nUsage: ./compression-tool [option] file_path\n  -c\t\tcompress file\n  -d\t\tdecompress file\n\n");
		exit(EXIT_FAILURE);
	}

	int fail_flag = EXIT_SUCCESS;

	// Open file
	char* path = argv[2];
	FILE * in_file = fopen(path, "r");
	check_pointer(in_file);

	if (strcmp(argv[1], "-c") == 0){
		/* Compression */

		fseek(in_file, 0L, SEEK_END);
		size_t file_size = ftell(in_file);
		fseek(in_file, 0L, SEEK_SET);

		/* LZ77 */
		uint8_t * file_buffer = malloc(file_size); // TODO: Large files will break this. A buffer should be used inside the LZ module instead.
		fread(file_buffer, file_size, 1, in_file); // load all file into memory
		fclose(in_file);

		uint8_t * lz_out = NULL;
		size_t lz_out_len = 0;
		fail_flag = fail_flag || lz77_compress(file_buffer, file_size, &lz_out, &lz_out_len); // LZ compress the file
		// FAILURE
		if (fail_flag == EXIT_FAILURE){
			free(lz_out);
			return fail_flag;
		}
		
		FILE * lz_out_file = tmpfile();
		check_pointer(lz_out_file);
		fwrite(lz_out, lz_out_len, 1, lz_out_file); // create a temporary file Huffman can operate on
		free(lz_out);
		rewind(lz_out_file); // rewind for Huffman

		/* Huffman encode */
		// TODO: create out_file based on in_file path
		FILE * out_file = fopen("test_out_file.olv", "w");
		check_pointer(out_file);

		fail_flag = fail_flag || compress(lz_out_file, out_file);
		fclose(lz_out_file);
		fclose(out_file);
	}
	else if (strcmp(argv[1], "-d") == 0){
		/* Decompression */

		/* Huffman decode */

		FILE * huffman_out_file = tmpfile();
		check_pointer(huffman_out_file);

		fail_flag = fail_flag || decompress(in_file, huffman_out_file);
		fclose(in_file);
		size_t file_size = ftell(huffman_out_file); // calculate LZ77 compressed text size
		rewind(huffman_out_file); // rewind to apply LZ77 decompression
		
		/* LZ77 */
		uint8_t * file_buffer = malloc(file_size);
		fread(file_buffer, file_size, 1, huffman_out_file);
		fclose(huffman_out_file); // after loading LZ77 compressed/Huffman decompressed text into memory, discard temp file

		uint8_t * lz_out = NULL;
		size_t lz_out_len = 0;
		fail_flag = fail_flag || lz77_decompress(file_buffer, file_size, &lz_out, &lz_out_len); // LZ compress the file
		// FAILURE
		if (fail_flag == EXIT_FAILURE){
			free(lz_out);
			return fail_flag;
		}
		
		FILE * lz_out_file = fopen("test_reconstruct_in_file", "w");
		check_pointer(lz_out_file);
		fwrite(lz_out, lz_out_len, 1, lz_out_file); // write LZ decompressed data to out file
		free(lz_out);
		fclose(lz_out_file);
	}
	else{
		printf("Failed: Could not parse arguments.\n");
		fail_flag = EXIT_FAILURE;
	}

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
