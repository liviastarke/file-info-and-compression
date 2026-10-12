#include <stdio.h>
#include "tree.h"

int main(void)
{
    int freqs[ALPHABET_SIZE] = {0};

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freqs[i] = 1;
    }

    HuffmanNode *root = build_tree_from_frequencies(freqs);

    if (root == NULL) {
        printf("Falha ao construir a arvore.\n");
        return 1;
    }

    printf("Frequencia da raiz: %d\n", root->freq);

    free_tree(root);
    return 0;
}