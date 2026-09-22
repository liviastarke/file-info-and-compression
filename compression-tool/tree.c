#include "tree.h"

#include "pq.h"
#include "check_pointer.h"

#include <stdio.h>
#include <stdlib.h>

HuffmanNode * create_leaf(unsigned char ch, int freq){
	HuffmanNode * new_leaf = malloc(sizeof(HuffmanNode));
	check_pointer(new_leaf);
	new_leaf->left = new_leaf->right = NULL;
	new_leaf->ch = ch;
	new_leaf->freq = freq;
	return new_leaf;
}

HuffmanNode * node_merge(HuffmanNode * left, HuffmanNode * right){
	HuffmanNode * new_parent = malloc(sizeof(HuffmanNode));
	check_pointer(new_parent);
	new_parent->left = left;
	new_parent->right = right;
	new_parent->freq = left->freq + right->freq;
	new_parent->ch = 0;
	return new_parent;
}

HuffmanNode * build_tree_from_frequencies(const int freqs[ALPHABET_SIZE]){
	if (freqs == NULL){
		printf("Couldn't build tree: NULL frequencies array.\n");
		return NULL;
	}

	MinHeap * nodes = malloc(sizeof(MinHeap));
	check_pointer(nodes);
	nodes->size = 0;
	for (int i = 0; i < ALPHABET_SIZE; i++){
		if (freqs[i] > 0){
			pq_push(nodes, create_leaf(i, freqs[i]));
		}
	}

	// empty file, 0 frequencies for all symbols
	if (nodes->size == 0){
		return NULL;
	}

	HuffmanNode * left;
	HuffmanNode * right; 
	HuffmanNode * parent;

	while (nodes->size >= 2){
		left = pq_pop(nodes);
		right = pq_pop(nodes);
		parent = node_merge(left, right);
		pq_push(nodes, parent);
	}

	parent = pq_pop(nodes);

	free(nodes);

	return parent; // the root of all the tree
}

void free_tree(HuffmanNode * root){
	if (root->right){
		free_tree(root->right);
	}
	if (root->left){
                free_tree(root->left);
        }
	if (root){
		free(root);
	}
}

// roubei do Bruno
void print_tree(HuffmanNode * root, int depth){
    if (root == NULL)
        return;

    print_tree(root->right, depth + 1);

    for(int i = 0; i < depth; i++){
        printf("\t");
    }

	if (root->ch == 0){
		printf("\ufffd\n");
	}
	else{
    	printf(" %c \n", root->ch);
	}

    print_tree(root->left, depth + 1);
}

int main(){
	int mock_freqs[ALPHABET_SIZE] = {0};
    mock_freqs['A'] = 5; // 41
    mock_freqs['B'] = 9;
    mock_freqs['C'] = 12;
    mock_freqs['D'] = 13;
    mock_freqs['E'] = 16;
    mock_freqs['F'] = 45;

	HuffmanNode * tree = build_tree_from_frequencies(mock_freqs);
	print_tree(tree, 0);
	return 0;
}
