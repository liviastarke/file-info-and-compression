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

HuffmanNode *pq_pop(MinHeap *pq) {

  //no com menor frequência (aparentemente)
  HuffmanNode *min = pq->node[0];

  pq->size--;
  pq->node[0] = pq->node[pq->size];

  int node_down = 0;

  while (2 * node_down + 1 < pq->size) {
    int esq = 2 * node_down + 1;
    int dir = 2 * node_down + 2;
    
    int menor = pq->node[esq] < pq->node[dir] ? esq : dir;
    //se já está no lugar certo:
    if ((pq->node[node_down]->freq) <= (pq->node[menor]->freq)) {
      return min;
    }

    HuffmanNode *temp = pq->node[node_down];
    pq->node[node_down] = pq->node[menor];
    pq->node[menor] = temp;

    node_down = menor;
  }

  return min;
}