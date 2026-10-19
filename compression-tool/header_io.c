#include "header_io.h"

#include "common.h"

#include <string.h>

void count_frequencies(FILE *in, uint64_t freqs[ALPHABET_SIZE]) {
    // Conta de 0 a 255, zerando os valores iniciais
    for (int i = 0; i < ALPHABET_SIZE; i++) freqs[i] = 0;
    int c;
    while ((c = fgetc(in)) != EOF) {
        freqs[ (unsigned char) c ]++;
    }

    fseek(in, 0L, SEEK_SET);
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
