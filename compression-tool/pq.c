/**
 * @file pq.c
 * @brief Implementação do Min-Heap para nós Huffman (HuffmanNode)
 */

/**
 * Objetivo: Implementa pq_push e pq_pop
 */

#include "pq.h"

/**
 * @brief Insere um nó na fila de prioridade (Min-Heap). O nó sobe quando a frequência é menor que o Nó Pai
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

/**
 * Existe uma implementação a ser modificada
 */
HuffmanNode *pq_pop(MinHeap *pq) {

  //no com menor frequência (aparentemente)
  HuffmanNode *min = pq->node[0];

  pq->size--;
  pq->node[0] = pq->node[pq->size];

  int node_down = 0;

  while (2 * node_down + 1 < pq->size) {
    int esq = 2 * node_down + 1;
    int dir = 2 * node_down + 2;
    
    int menor = esq;

    //Assume que o nó filho esq. é o menor, verifica se o nó filho dir. existe
    //SE (nó_filho_dir) < (nó_filho_esq) ENTAO: return (nó_filho_dir)
    if (dir < pq->size && pq->node[dir]->freq < pq->node[esq]->freq) {
        menor = dir;
    }
    //condição de parada
    //SE nó_lugar_certo ENTAO encerra_laço (break)
    if (pq->node[node_down]->freq <= pq->node[menor]->freq) {
      break; 
    }

    HuffmanNode *temp = pq->node[node_down];
    pq->node[node_down] = pq->node[menor];
    pq->node[menor] = temp;

    node_down = menor;
  }

  return min;
}