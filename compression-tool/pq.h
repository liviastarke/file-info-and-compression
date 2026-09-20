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
void pq_push(MinHeap *pq, HuffmanNode *node);

HuffmanNode *pq_pop(MinHeap *pq);

//PQ_H
#endif