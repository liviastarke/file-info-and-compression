#ifndef __TABLE_H__ 
#define __TABLE_H__

#include <stdio.h>
#include <stdint.h> // para conseguir usar o uint8
typedef struct {
    
    FILE *file; 
    uint8_t current_byte; // inteiro sem sinal de 8 BITS
    int bit_count;
    
} BitWriter; // funções gerais

void bw_init(BitWriter *bw, FILE *f); 
void bw_write_bit(BitWriter *bw, int bit); // chamando a função se é 0 ou 1
void bw_flush(BitWriter *bw); 
void bw_write_bits(BitWriter *bw, uint32_t bits, uint8_t len); // verificando agora mais bits!
void count_frequencies(FILE *in, int freqs[256]); //  frequência de caracteres do arquivo de entrada

#endif
