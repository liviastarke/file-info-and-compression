#include "table.h"

#include<stdio.h>
#include <stdlib.h>

void print_code(Code * code){
    for (int i = 0; i < code->len; i++){
        printf("%b", (code->bits >> i) & 0x1);
    }
}

void print_table(HuffmanTable * table){
    for (int i = 0; i < ALPHABET_SIZE; i++){
        if (table->table[i].len > 0){
            printf("%c : ", i);
            print_code(&table->table[i]);
            printf("\n");
        }
    }
    printf("\n");
}

int huffman_encode(const HuffmanNode * node, uint32_t current_code, uint8_t depth, HuffmanTable * codes){
	if(node == NULL){
		return EXIT_FAILURE; // probably won't use the ret value, but I can change my mind later
	}

    // if node is leaf node, node is char
	if (node->left == NULL && node->right == NULL){
        codes->table[node->ch].bits = current_code;
        codes->table[node->ch].len = depth;
        return EXIT_SUCCESS;
	}

    depth++;
	huffman_encode(node->left, current_code << 1, depth, codes);
	huffman_encode(node->right, current_code << 1 | 0x1, depth, codes);
	return EXIT_SUCCESS;
}

int generate_codes(const HuffmanNode * root, HuffmanTable * out_table){

    // exit if empty tree
    if (root == NULL){
        printf("Couldn't generate codes: Huffman tree was empty.\n");
        return EXIT_FAILURE;
    }

    // else if single node tree
    if (root->left == NULL && root->right == NULL){
        return huffman_encode(root, 0, 1, out_table); // set depth to 1 in order to print it directly
    }

    // happy path: encode with starting depth of 0
    return huffman_encode(root, 0, 0, out_table);
}

// int main(){
//     // TEST: create fake tree
//     HuffmanNode * fake_tree = malloc(sizeof(HuffmanNode));
//     // fake_tree->ch = 'a';
//     fake_tree->left = malloc(sizeof(HuffmanNode));
//     fake_tree->left->ch = 'a';
//     fake_tree->right = malloc(sizeof(HuffmanNode));
//     fake_tree->right->ch = 'b';

//     HuffmanTable * table = malloc(sizeof(HuffmanTable));
//     generate_codes(fake_tree, table);
//     print_table(table);
//     return 0;
// }