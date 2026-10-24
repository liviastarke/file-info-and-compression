#include "lz77.h"
#include <stdlib.h>   // malloc, free, EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>   // memcpy, se precisar

// procura a maior sequência de bytes que se repete em uma posição anterior à posição atual
static size_t find_match(const uint8_t *in, size_t in_len,size_t pos, size_t *best_offset) // daria para encaixar na compressão, mas ficaria enorme...
{

	size_t longest_match = 0; // guarda o tamanho da maior repetição encontrada até agora
	*best_offset = 0; // inicialmente, não temos nenhuma distância válida
	size_t max_retrocesso;

	if (pos < LZ77_WINDOW_SIZE) {
		// se a posição está perto do começo dos dados, só podemos voltar até a posição 0. Por isso, o limite é a própria posição
		max_retrocesso = pos;
	} else {
		// se já avançamos o suficiente na entrada, podemos usar toda a janela de busca permitida pelo algoritmo
		max_retrocesso = LZ77_WINDOW_SIZE;
	}

	// Continuamos até testar todas as distâncias permitidas
	for (size_t distancia = 1; distancia <= max_retrocesso; distancia++) {

		//compara o trecho anterior 'j' com o trecho atual 'pos'
		size_t j = pos - distancia; // calcula o trecho anterior, se posição = 10 e distancia = 3, começa na posição 7

		size_t comprimento_atual = 0;

		// Quando uma condição falhar, a comparação termina
		while ((pos + comprimento_atual < in_len) &&  // enquanto ainda existir bytes na entrada para comparar
		        (in[j + comprimento_atual] == // E os bytes de trecho anterior e da posição atual são iguais
		         in[pos + comprimento_atual]) &&
		        (comprimento_atual < LZ77_MAX_MATCH)) {
			comprimento_atual++;
		}


		// se o comprimento atual encontrou uma sequência maior do que a sequência encontrada anteriormente
		if (comprimento_atual > longest_match) {

			longest_match = comprimento_atual; 	// atualiza o tamanho da melhor repetição encontrada


			*best_offset = distancia; // guarda a distância correspondente a essa repetição
		}

	}

	// Devolve o tamanho da maior sequência encontrada
	return longest_match;
}

int lz77_compress(const uint8_t *in, size_t in_len, uint8_t **out, size_t *out_len) {

	if (in == NULL || out == NULL || out_len == NULL) //se não me deram o endereço dos dados OU não me deram onde entregar o resultado OU não me deram onde anotar o tamanho, eu paro e retorno erro.
		return EXIT_FAILURE; // por exemplo aonde estão os dados? aonde eu devo colocar os dados? aonde eu devo anotar o tamanho dos dados?
	if (in_len > UINT32_MAX) // se o tamanho dos dados for maior que o maior número que cabe em 4 bytes...
		return EXIT_FAILURE;

	size_t max_out = 4 + in_len + (in_len + 7) / 8;
	uint8_t *buf = malloc(max_out);
	if (buf == NULL) 
		return EXIT_FAILURE;

	buf[0] = (in_len >> 24) & 0xFF;
	buf[1] = (in_len >> 16) & 0xFF;
	buf[2] = (in_len >> 8) & 0xFF;
	buf[3] = in_len & 0xFF;

	size_t out_pos = 4;        // próxima posição livre na saída 
	size_t pos = 0;            // qual byte estamos processando
	size_t flag_pos = 0;       // posição, na saída, onde fica o byte de flags do grupo atual
	uint8_t flags = 0;         // byte de flags sendo montado
	int n_grupo = 0;           // quantos tokens já entraram no grupo atual, de 0 a 8
	// tokens são as decições do compressor, quantos caracteres vão ser repetidos, andados...


	while (pos < in_len) {
		if (n_grupo == 0) {
			flag_pos = out_pos++;
			flags = 0;
		}

		size_t offset = 0;
		size_t len = find_match(in, in_len, pos, &offset);

		if (len >= LZ77_MIN_MATCH) {
			// Marca o token como referência
			flags |= (uint8_t)(1u << n_grupo);

			// Grava a distância e o comprimento da repetição
			buf[out_pos++] = (uint8_t)(offset >> 4);
			buf[out_pos++] = (uint8_t)(
				((offset & 0x0F) << 4) |
				(len - LZ77_MIN_MATCH)
			);

			pos += len;
		} else {
			// Não encontrou uma repetição útil: grava o byte original
			buf[out_pos++] = in[pos++];
		}

		n_grupo++;

		if (n_grupo == 8) {
			buf[flag_pos] = flags;
			n_grupo = 0;
		}
	}

	// Grava as flags do último grupo incompleto
	if (n_grupo > 0) {
		buf[flag_pos] = flags;
	}

	*out = buf;
	*out_len = out_pos;

	return EXIT_SUCCESS;
}

int lz77_decompress(const uint8_t *in, size_t in_len, uint8_t **out, size_t *out_len) {
    if (in == NULL || out == NULL || out_len == NULL || in_len < 4)
        return EXIT_FAILURE;

    *out = NULL;
    *out_len = 0;

    // Recupera o tamanho original, armazenado nos primeiros 4 bytes
    uint32_t original_size =
        ((uint32_t)in[0] << 24) |
        ((uint32_t)in[1] << 16) |
        ((uint32_t)in[2] << 8) |
        (uint32_t)in[3];

    uint8_t *buf = malloc(original_size > 0 ? original_size : 1);

    if (buf == NULL)
        return EXIT_FAILURE;

    size_t in_pos = 4;
    size_t out_pos = 0;

    while (out_pos < original_size) {
        // Lê as flags do grupo atual
        if (in_pos >= in_len) {
            free(buf);
            return EXIT_FAILURE;
        }

        uint8_t flags = in[in_pos++];

        for (int i = 0; i < 8 && out_pos < original_size; i++) {
            if (flags & (1u << i)) {
                // Token de referência: precisa de dois bytes
                if (in_len - in_pos < 2) {
                    free(buf);
                    return EXIT_FAILURE;
                }

                uint8_t first = in[in_pos++];
                uint8_t second = in[in_pos++];

                size_t offset =
                    ((size_t)first << 4) | (second >> 4);

                size_t len =
                    (second & 0x0F) + LZ77_MIN_MATCH;

                // Verifica se a referência é válida
                if (offset == 0 || offset > out_pos || len > original_size - out_pos) {
    				free(buf);
                    return EXIT_FAILURE;
                }

                // Copia a sequência referenciada
                for (size_t j = 0; j < len; j++) {
                    buf[out_pos] = buf[out_pos - offset];
                    out_pos++;
                }
            } else {
                // Token literal: copia um byte original
                if (in_pos >= in_len) {
                    free(buf);
                    return EXIT_FAILURE;
                }
                buf[out_pos++] = in[in_pos++];
            }
        }
    }
    *out = buf;
    *out_len = out_pos;
    return EXIT_SUCCESS;
}