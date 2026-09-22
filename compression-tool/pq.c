/**
 * @file pq.c
 * @brief Implementação do Min-Heap para nós Huffman (HuffmanNode)
 */

#include "pq.h"

/**
 * @brief Insere um nó na fila de prioridade (Min-Heap)
 */
void pq_push(MinHeap *pq, HuffmanNode *nd) {
    pq->node[pq->size] = nd;
    pq->size++;  

    // Guarda a posição do novo nó
    int i = pq->size - 1;

    while (i > 0) { 

        // Calcula o índice do nó pai
        int pai = (i - 1) / 2;

        // Se o pai tiver frequência menor ou igual,
        if (pq->node[pai]->freq <= pq->node[i]->freq) {
            break;
        }

        // Troca o nó atual com o nó pai
        HuffmanNode *temp = pq->node[pai];
        pq->node[pai] = pq->node[i];
        pq->node[i] = temp;

        // o nó agora está na posição do antigo pai
        i = pai;
    }
}
