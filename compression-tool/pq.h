// Declaração da fila de prioridade (Min-Heap)
#ifndef PQ_H
#define PQ_H

#include "common.h"

//Estrutura do Min-Heap para armazenar nós da árvore de Huffman.
typedef struct {
    HuffmanNode *node[ALPHABET_SIZE];
    int size;
} MinHeap;

void pq_push(MinHeap *pq, HuffmanNode *nd);
HuffmanNode *pq_pop(MinHeap *pq);

#endif /* PQ_H */   
