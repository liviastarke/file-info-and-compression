/**
 * É necessário tirar da pasta para funcionar
 */
#include <stdio.h>
#include "pq.h"

int main(void)
{
    MinHeap pq = {0};

    HuffmanNode a = {.ch = 'A', .freq = 5};
    HuffmanNode b = {.ch = 'B', .freq = 2};
    HuffmanNode c = {.ch = 'C', .freq = 8};

    pq_push(&pq, &a);
    pq_push(&pq, &b);
    pq_push(&pq, &c);

    while (pq.size > 0) {
        HuffmanNode *node = pq_pop(&pq);
        printf("%c: %d\n", node->ch, node->freq);
    }

    return 0;
}
