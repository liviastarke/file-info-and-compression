/**
 * @file pq.c
 * @brief Implementação do Min-Heap para nós Huffman (HuffmanNode)
 */

#include "pq.h"

/**
 * @brief Insere um nó na fila de prioridade (Min-Heap)
 */
void pq_push(MinHeap *pq, HuffmanNode *nd)
{
    pq->nodes[pq->size] = nd;
    pq->size++;

    // Guarda a posição do novo nó
    int i = pq->size - 1;

    while (i > 0) {

        // Calcula o índice do nó pai
        int parent = (i - 1) / 2;

        // Se o pai tiver frequência menor ou igual,
        if (pq->nodes[parent]->freq <= pq->nodes[i]->freq) {
            break;
        }

        // Troca o nó atual com o nó pai
        HuffmanNode *temp = pq->nodes[parent];
        pq->nodes[parent] = pq->nodes[i];
        pq->nodes[i] = temp;

        // o nó agora está na posição do antigo pai
        i = parent;
    }
}