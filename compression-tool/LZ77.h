#ifndef LZ77_H
#define LZ77_H

#include <stdint.h>  
#include <stddef.h>  // em principal para o size_t (tipo para tamanhos)

#define LZ77_OFFSET_BITS 12 // bits usados para guardar a distância, quantas casas andar...
#define LZ77_LENGTH_BITS 4  // bits para guardar o comprimento, o tamanho da frase repetida...
#define LZ77_MIN_MATCH 3 // tamanho mínimo de repetição, abaixo disso não compensa comprimir, por exemplo 1 ou 2 caracteres



// tamanho máximo do histórico armazenado na memória (2^12 - 1 = 4095 bytes)
#define LZ77_WINDOW_SIZE ((1 << LZ77_OFFSET_BITS) - 1)  // o << desloca os bits, no caso vai ser 1 << 12, 4095 posições para trás...

// comprimento máximo de uma repetição usando o truque de soma, ficando 18 caracteres
#define LZ77_MAX_MATCH (((1 << LZ77_LENGTH_BITS) - 1) + LZ77_MIN_MATCH) 



// comprime in[0..in_len). Aloca *out e grava o tamanho em *out_len, o chamador libera *out com free(). Retorna EXIT_SUCCESS ou EXIT_FAILURE
int lz77_compress(const uint8_t *in, size_t in_len, uint8_t **out, size_t *out_len);

// faz o inverso: in é o dado comprimido, *out é o original reconstruído, mesmas regras de memória e retorno.
int lz77_decompress(const uint8_t *in, size_t in_len, uint8_t **out, size_t *out_len);
// se necessário acrescentar mais algumas funções aqui
#endif