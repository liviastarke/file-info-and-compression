#include "main.h"

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

	// Count char frequencies from file to build Huffman tree
	uint64_t freqs[ALPHABET_SIZE] = {0};
	get_frequencies_from_header(compressed_file, freqs); // MAIN.H
	
	HuffmanNode *root = build_tree_from_frequencies(freqs); // TREE.H

	uint64_t num_symbols = get_num_symbols_from_header(compressed_file);
	fseek(compressed_file, calc_header_size(num_symbols), SEEK_SET);

	int success = decode_file(root, compressed_file, num_symbols * 8, out_file);

	free_tree(root);
	return success;
}

void write_file_header(FILE * file, uint64_t freqs[ALPHABET_SIZE], uint64_t num_symbols_in_file){
	fputs("OLIVE", file); // 5 byte magic number
	fwrite(&num_symbols_in_file, sizeof(uint64_t), 1, file); // write number of chars that were encoded / chars to decode
	fputc(0,file); //reserva o byte para a quantidade de registros
	
	uint8_t num_freqs = 0; // this must be reviewed if alphabet size changes (poor decision, I know)
	for (int i = 0; i < ALPHABET_SIZE; i++){
		if (freqs[i] > 0){
			fputc(i, file);
			fwrite(&(freqs[i]), sizeof(uint64_t), 1, file);
			// printf("char: %c; count: %lu\n", i, freqs[i]); // 73 chars for my sample aula5 file would be 0x49
			num_freqs++;
		}
	}
	fseek(file, 5 + sizeof(uint64_t), SEEK_SET);
	fwrite(&num_freqs, sizeof(uint8_t), 1, file);

	// Coloca o ponteiro após o header para começar a codificar os símbolos
	fseek(file, calc_header_size(num_freqs), SEEK_SET);
}


void check_file_signature(FILE * compressed_file){
	fseek(compressed_file, 0L, SEEK_SET);
	char signature[5];
	fread(signature, sizeof(char), 5, compressed_file);
	if (strncmp(signature, "OLIVE", 5)){
		printf("Failed: input file signature for decompression does not match.");
	}
}

uint64_t get_num_symbols_from_header(FILE * compressed_file){
	fseek(compressed_file, 5L, SEEK_SET);
	uint64_t num_symbols;
	fread(&num_symbols, sizeof(uint64_t), 1, compressed_file);
	return num_symbols;
}

void get_frequencies_from_header(FILE * compressed_file, uint64_t freqs[ALPHABET_SIZE]){
	fseek(compressed_file, 5 + sizeof(uint64_t), SEEK_SET);
	uint8_t num_freqs;
	fread(&num_freqs, sizeof(uint8_t), 1, compressed_file);
	for(uint8_t i = 0; i < num_freqs; i++){
		int index = fgetc(compressed_file);
		fread(&(freqs[index]), sizeof(uint64_t), 1, compressed_file);
	}
}

/**
 * Após o início do arquivo, o tamanho do header é, somando:
 * os 5 bytes do número mágico
 * + 1 byte que armazena o número de frequências diferentes de zero	(número de "registros")		= 6
 * + o tamanho do arquivo original (número de símbolos a serem codificados/decodificados)		= sizeof(uint64_t)
 * + tamanho dos registros (cada um composto de uma frequência (uint64_t) e um char)			= num_freqs * (sizeof(uint64_t) + sizeof(unsigned char))
 */
long calc_header_size(uint8_t num_freqs){
	return 6 + sizeof(uint64_t) + ( num_freqs * ( sizeof(uint64_t) + sizeof(unsigned char) ) );
}
