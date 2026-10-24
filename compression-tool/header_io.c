#include "header_io.h"

#include "common.h"

#include <string.h>

void count_frequencies(FILE *in, uint64_t freqs[ALPHABET_SIZE]) {
    // Conta de 0 a 255, zerando os valores iniciais
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freqs[i] = 0;
    }

    int c;
    while ((c = fgetc(in)) != EOF) {
        freqs[(unsigned char)c]++;
    }

    fseek(in, 0L, SEEK_SET);
}

void write_file_header(FILE *file, uint64_t freqs[ALPHABET_SIZE],
                       uint64_t num_symbols_in_file) {
    fputs("OLIVE", file); // 5 bytes da assinatura
    fwrite(&num_symbols_in_file, sizeof(uint64_t), 1, file);
    fputc(0, file); // Reserva o byte para a quantidade de registros

    uint8_t num_freqs = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (freqs[i] > 0) {
            fputc(i, file);
            fwrite(&freqs[i], sizeof(uint64_t), 1, file);
            num_freqs++;
        }
    }

    // Atualiza a quantidade de registros no cabeçalho
    fseek(file, 5 + sizeof(uint64_t), SEEK_SET);
    fwrite(&num_freqs, sizeof(uint8_t), 1, file);

    // Posiciona o cursor após o cabeçalho
    fseek(file, calc_header_size(num_freqs), SEEK_SET);
}

void check_file_signature(FILE *compressed_file) {
    fseek(compressed_file, 0L, SEEK_SET);

    char signature[5];
    fread(signature, sizeof(char), 5, compressed_file);

    if (strncmp(signature, "OLIVE", 5)) {
        printf("Failed: input file signature for decompression does not match.");
    }
}

uint64_t get_num_symbols_from_header(FILE *compressed_file) {
    fseek(compressed_file, 5L, SEEK_SET);

    uint64_t num_symbols;
    fread(&num_symbols, sizeof(uint64_t), 1, compressed_file);

    return num_symbols;
}

void get_frequencies_from_header(
    FILE *compressed_file,
    uint64_t freqs[ALPHABET_SIZE]
) {
    fseek(compressed_file, 5 + sizeof(uint64_t), SEEK_SET);

    uint8_t num_freqs;
    fread(&num_freqs, sizeof(uint8_t), 1, compressed_file);

    for (uint8_t i = 0; i < num_freqs; i++) {
        int index = fgetc(compressed_file);
        fread(&freqs[index], sizeof(uint64_t), 1, compressed_file);
    }
}

/*
 * Tamanho do cabeçalho:
 * 5 bytes da assinatura "OLIVE"
 * + 8 bytes para a quantidade de símbolos do arquivo original
 * + 1 byte para a quantidade de registros de frequência
 * + 9 bytes por registro
 */
long calc_header_size(uint8_t num_freqs) {
    return 6 + sizeof(uint64_t) +
           (num_freqs * (sizeof(uint64_t) + sizeof(unsigned char)));
}