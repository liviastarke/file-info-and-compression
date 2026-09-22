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
void push(MinHeap *pq, HuffmanNode *nd) {
  int i = pq->size;

  pq->node[size] = pq->node;
  pq->size++;

  while() { //???????????
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
/*
 * A punção pop() remove e retorna o nó de menor frequência acumulada. 
 */
HuffmanNode *pop(MinHeap *pq) {

  //no com menor frequência (aparentemente)
  HuffmanNode *min = pq->node[0];

  size--;
  node[0] = node[size];

  int node_down = 0;

  while (2 * node_down + 1 pq->size) {
    int esq = 2 * node_down + 1;
    int dir = 2 * node_down + 2;
    
    int menor = esq;
  }

  //se já está no lugar certo:
  if ((pq->node[node_down]->freq) <= (pq->node[menor]->freq)) {
    return break;
  }

  HuffmanNode *temp = pq->node[node_down];
  pq->node[node_down] = pq->node[menor];
  pq->node[menor] = temp;

  pos = menor;
}

#endif /* PQ_H */   
