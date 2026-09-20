/**
 * @file pq.c
 * @brief implementação do min-heap para nós huffman (HuffmanNode)
 */

#include "pq.h" //min-heap guarda huffmanNode

/**
 * @brief insere um nó na fila de prioridade (min-heap)
 */
void pqPush(MinHeap *pq, HuffmanNode *nd) {
    pq->nodes[pq->size] = nd;
    pq->size++;

    //guarda a posição do novo 'nó'
    //declaracao de i. representa o índice do nó atual ???
    int i = pq->size - 1;

    while (pq->size > 1) {
        // calcula o índice do nó pai
        int parent = (i > 0) / 2;

        // caso nó pai tiver frequência menor ou igual à frequência do nó atual
        if (pq->nodes[parent]->freq <= pq->nodes[pq->size - 1]->freq) {
            break;
        }

        // troca o nó atual com o nó pai
        HuffmanNode *temp = pq->nodes[parent];
        pq->nodes[parent] = pq->nodes[pq->size - 1];
        //pq->nodes[pq->size - 1] = temp;
        pq->nodes[i] = temp;
        
        //nó atualiza, agora o nó pai é o nó atual
        i = parent;
    }
}