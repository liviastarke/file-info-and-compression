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

int decode_stream(const HuffmanNode * root, const uint8_t *bits, size_t bit_len, FILE * out){

    // exit if empty tree
    if (root == NULL){
        printf("Couldn't decode: Huffman tree was empty.\n");
        return EXIT_FAILURE;
    }

    // exit if no bits to decode
    if (bits == NULL){
        printf("Couldn't decode: No bit sequence to decode.\n");
        return EXIT_FAILURE;
    }

    // exit if empty bitstream
    if (bit_len == 0){
        printf("Couldn't decode: length zero bit sequence.\n");
        return EXIT_FAILURE;
    }

    // Special single node tree case
    if (root->left == NULL && root->right == NULL){
        for (size_t i = 0; i < bit_len; i++){
            fputc(root->ch, out);
        }
        return EXIT_SUCCESS;
    }

    const HuffmanNode * current_node = root;

    for (size_t i = 0; i < bit_len; i++){
        uint8_t byte = bits[i/8];

        if (byte >> (7 - i%8) & 1){
            current_node = current_node->right;
        }
        else{
            current_node = current_node->left;
        }

        if (current_node->left == NULL && current_node->right == NULL){
            fputc(current_node->ch, out);
            current_node = root;
        }
    }
}

// int main(){
//     // TEST: create fake tree
//     HuffmanNode * fake_tree = malloc(sizeof(HuffmanNode));
//     // fake_tree->ch = 'a'; fake_tree->left = NULL; fake_tree->right = NULL;
//     fake_tree->left = malloc(sizeof(HuffmanNode));
//     fake_tree->left->ch = 'a';
//     fake_tree->right = malloc(sizeof(HuffmanNode));
//     fake_tree->right->ch = 'b';

//     HuffmanTable * table = malloc(sizeof(HuffmanTable));
//     generate_codes(fake_tree, table);
//     print_table(table);

//     uint8_t compressed_data[] = {0x32, 0x32};
//     decode_stream(fake_tree, compressed_data, 16, stdout);
//     printf("\n\n");

//     compressed_data[0] = 0xff;
//     decode_stream(fake_tree, compressed_data, 16, stdout);
//     printf("\n\n");

//     return 0;
// }
