/** 
 * @file pq.h
 * @brief declaração de fila de prioridade (min-heap) para armazenar nós da árvore de Huffman.
 */

#ifndef PQ_H
#define PQ_H

#include "common.h" //min-heap guarda huffmanNode*

/**
 * @brief estrutura do , min-heap para armazenar nós da árvore de Huffman
 */
typedef struct {
    HuffmanNode *nodes[ALPHABET_SIZE];
    int size;
} MinHeap;


/**
 * @brief inicializa a fila de prioridade (min-heap)
 */
void pq_push(MinHeap *pq, HuffmanNode *nd) {

    //colocar o nó na última posição

    //inserir o nó na última posição do heap

    //fazer nó subir até a posição correta enquanto a frequência do nó pai for maior que a frequência do nó atual
    /*
    while() {
        //sei la
    }
    */
    //trocar o nó atual com o nó pai

    //continuar a partir da posição do nó pai
    
}

HuffmanNode *pq_pop(MinHeap *pq) {

    if (pq->size == 0) {
        //printf("Fila vazia.\n");
        return NULL;
    }    
}

//PQ_H
#endif
