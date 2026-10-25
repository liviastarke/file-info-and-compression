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

	while (pos < in_len) // enquanto ainda houver bytes para processar...
	{
		if (n_grupo == 0)
		{
			flag_pos = out_pos++;
			flags = 0;
		}

		size_t offset;
		size_t len = find_match(in, in_len, pos, &offset);
		// len recebe o comprimento da maior repetição achada e offset recebe a distância dela.

		if (len >= LZ77_MIN_MATCH) // comprimento da repetição for maior ou igual ao mínimo compensa
		{
			// o bit correspondente ao número deste token dentro do grupo
			flags |= (uint8_t)(1 << n_grupo); // |= liga o bit deste token sem apagar os outros bits já ligados

			// monto um número de 16 bits com o offset nos 12 bits de cima e o comprimento menos 3 nos 4 de baixo
			uint16_t ref = (uint16_t)((offset << LZ77_LENGTH_BITS) | (len - LZ77_MIN_MATCH));
			buf[out_pos++] = ref >> 8;     // byte alto
			buf[out_pos++] = ref & 0xFF;   // byte baixo
			pos += len;
		} // | vai juntar...
		else // senão, no caso menor que 3...
		{
			// copio o byte atual da entrada para a próxima posição livre da saída...
			buf[out_pos++] = in[pos];
			pos++;
		}

		n_grupo++; // mais um token entrou no grupo atual...

		// grupo cheio (8 tokens)!
		if (n_grupo == 8)
		{
			buf[flag_pos] = flags; 
			n_grupo = 0;  // próximo ref começa um grupo novo
		}
	}

	// sobrou um grupo incompleto...
	if (n_grupo > 0)
	{
		buf[flag_pos] = flags; // grava as flags dele também
	}

	*out = buf; 
	*out_len = out_pos; 
	return EXIT_SUCCESS;

}


int lz77_decompress(const uint8_t *in, size_t in_len, uint8_t **out, size_t *out_len) {
	// mas aqui tem que garantir que o dado existe
	// validação padrão como na compressão
	if (in == NULL || out == NULL || out_len == NULL)
		return EXIT_FAILURE;
	if (in_len < 4) // diferença que tem que ter pelo menos 4 bytes para ler o cabeçalho
		return EXIT_FAILURE;

	// aqui o descompressor tem os 4 pedaços e precisa colá-los de volta no número original
	size_t orig_len = ((size_t)in[0] << 24) | ((size_t)in[1] << 16)
	                  | ((size_t)in[2] << 8)  |  (size_t)in[3];

	*out = NULL; // se der erro, o chamador não fica com um endereço lixo
	*out_len = 0;
	size_t alloc_len;

	if (orig_len == 0) { // se o tamanho orginal estiver zerada
		alloc_len = 1;  // acrescenta 1 byte

	} else {
		alloc_len = orig_len; 

	}

	uint8_t *buf = malloc(alloc_len); // alocando o valor do buf para o resultado

	if (buf == NULL) // se o malloc falhou, ele é NULL...
		return EXIT_FAILURE;

	size_t in_pos = 4; 
	size_t out_pos = 0;  

	// aqui vai começar a ver cada caracter para "reconstruir"
	while (out_pos < orig_len)
	{
		//se a posição de leitura chegou ao fim da entrada e ainda não reconstruímos tudo, o arquivo está cortado erro!!!
		if (in_pos >= in_len)
				goto falha; // vou ousar e testar, espero que de certo...

		uint8_t flags = in[in_pos++];

		// agora verificando os 8 tokens para a reconstruir
		for (int n_grupo = 0; n_grupo < 8 && out_pos < orig_len; n_grupo++)
		{
			
			if ((flags & (1u << n_grupo)) == 0)
			{
				if (in_pos >= in_len) // erro de continuação...
					goto falha;

				buf[out_pos++] = in[in_pos++]; 
			}
			else // senão, o bit está ligado É uma referência
			{
				if (in_len - in_pos < LZ77_REF_BYTES) // se restam menos de 2 bytes na entrada, não dá para ler a referência: arquivo cortado
					goto falha;
					
				// pego a referência de 16 bits com a próxima posição
				uint16_t ref = ((uint16_t)in[in_pos] << 8) | (uint16_t)in[in_pos + 1];
				in_pos += LZ77_REF_BYTES;

				// separa os dois números de dentro da referência
				size_t offset = ref >> LZ77_LENGTH_BITS; // descarta os 4 bits de baixo (o comprimento) e sobram os 12 de cima (distância)
				size_t len = (ref & ((1u << LZ77_LENGTH_BITS) - 1)) + LZ77_MIN_MATCH; // agora com o valor da referência
				// o & guarda só os 4 bits de baixo
				// o + 3 desfaz o "- 3" da compressão

				if (offset == 0 || offset > out_pos) // não dá para voltar além do que já foi escrito...
					goto falha;

				if (len > orig_len - out_pos) // a cópia passaria do tamanho original...
					goto falha;

				for (size_t k = 0; k < len; k++)
				{
					buf[out_pos] = buf[out_pos - offset]; // copia o byte de 'offset' posições atrás
					out_pos++;
				}
			}
		}
	}

	*out = buf; // entrega o endereço da saída ao chamador
	*out_len = out_pos; // e o tamanho real (igual a orig_len)
	return EXIT_SUCCESS;

falha: // goto goto goto... limpeza única para todos os erros acima, gostei dele...
	free(buf);
	return EXIT_FAILURE;


}


/*
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
*/
