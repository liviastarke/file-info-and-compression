#include "LZ77.h"
#include <stdlib.h>   // malloc, free, EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>   // memcpy, se precisar

// procura a maior sequência de bytes que se repete em uma posição anterior à posição atual
static size_t find_match(const uint8_t *in, size_t in_len,size_t pos, size_t *best_offset) // daria para encaixar na compressão, mas ficaria enorme...
{

	size_t longest_match = 0; // guarda o tamanho da maior repetição encontrada até agora
	*best_offset = 0; // inicialmente, não temos nenhuma distância válida
	size_t max_retrocesso;

	if (pos < LZ77_WINDOW_SIZE)
	{
		// se a posição está perto do começo dos dados, só podemos voltar até a posição 0. Por isso, o limite é a própria posição
		max_retrocesso = pos;
	}
	else
	{
		// se já avançamos o suficiente na entrada, podemos usar toda a janela de busca permitida pelo algoritmo
		max_retrocesso = LZ77_WINDOW_SIZE;
	}

	// Continuamos até testar todas as distâncias permitidas
	for (size_t distancia = 1; distancia <= max_retrocesso; distancia++)
	{

		//compara o trecho anterior 'j' com o trecho atual 'pos'
		size_t j = pos - distancia; // calcula o trecho anterior, se posição = 10 e distancia = 3, começa na posição 7

		size_t comprimento_atual = 0;

		// Quando uma condição falhar, a comparação termina
		while ((pos + comprimento_atual < in_len) &&  // enquanto ainda existir bytes na entrada para comparar
		        (in[j + comprimento_atual] == // E os bytes de trecho anterior e da posição atual são iguais
		         in[pos + comprimento_atual]) &&
		        (comprimento_atual < LZ77_MAX_MATCH)) // E não atingimos o tamanho máximo permitido
		{
			comprimento_atual++;
		}


		// se o comprimento atual encontrou uma sequência maior do que a sequência encontrada anteriormente
		if (comprimento_atual > longest_match)
		{

			longest_match = comprimento_atual; 	// atualiza o tamanho da melhor repetição encontrada


			*best_offset = distancia; // guarda a distância correspondente a essa repetição
		}

	}

	// Devolve o tamanho da maior sequência encontrada
	return longest_match;
}

int lz77_compress(const uint8_t *in, size_t in_len, uint8_t **out, size_t *out_len) {


}

int lz77_decompress(const uint8_t *in, size_t in_len, uint8_t **out, size_t *out_len) {

}