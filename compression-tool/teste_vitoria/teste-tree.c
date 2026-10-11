/**
 * É necessário tirar da pasta para funcionar
 */
#include <stdio.h>
#include "tree.h"

int main(void)
{
    int freqs[ALPHABET_SIZE] = {0};

    freqs['A'] = 5;
    freqs['B'] = 2;
    freqs['C'] = 8;

    HuffmanNode *root = build_tree_from_frequencies(freqs);

    if (root == NULL) {
        printf("Falha ao construir a arvore.\n");
        return 1;
    }

    printf("Frequencia da raiz: %d\n", root->freq);
    printf("Frequencia do filho esquerdo: %d\n", root->left->freq);
    printf("Frequencia do filho direito: %d\n", root->right->freq);

    free_tree(root);
    return 0;
}
