// Declaração da fila de prioridade (Min-Heap)
#ifndef PQ_H
#define PQ_H

#include "common.h"

//Estrutura do Min-Heap para armazenar nós da árvore de Huffman.
typedef struct {
    HuffmanNode *node[ALPHABET_SIZE];
    int size;
} MinHeap;


/*
 * Insere um nó na fila de prioridade.
 */
void pq_push(MinHeap *pq, HuffmanNode *nd) {
  int i = pq->size;

  pq->node[size] = pq->node;
  pq->size++;

  while() {
    int pai = ((i - 1) / 2); 
   
    //o que diabos é isso 
    if (pq->node[i]->freq >= pq->node[pai]->freq) {
      return 0;
    }

    HuffmanNode *temp = pq->node[i];

    pq->node[i] = pq->node[pai];
    node[pai] = temp;

    i = pai;
  }
}

// Remove e retorna o nó de menor frequência.
HuffmanNode *pq_pop(MinHeap *pq) {
  
}

#endif /* PQ_H */   
