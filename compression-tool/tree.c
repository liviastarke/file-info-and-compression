#include "tree.h"

#include "pq.h"

HuffmanNode *create_leaf(unsigned char ch, int freq){
	HuffmanNode * new_leaf = malloc(sizeof(HuffmanNode));
	new_leaf.left = new_leaf.right = NULL;
	new_leaf.ch = ch;
	new_leaf.freq = freq;
	return new_leaf;
}

HuffmanNode *node_merge(HuffmanNode *left, HuffmanNode *right){
	HuffmanNode * new_parent = malloc(sizeof(HuffmanNode));
	new_parent.left = left;
	new_parent.right = right;
	new_parent.freq = left->freq + right->freq;
	new_parent.ch = 0;
	return new_parent;
}

HuffmanNode *build_tree_from_frequencies(const int freqs[ALPHABET_SIZE]){
	if (freqs == NULL){
		printf("Couldn't build tree: NULL frequencies array.\n");
		return NULL;
	}

	MinHeap * nodes;
	nodes.size = 0;
	for (int i = 0; i < ALPHABET_SIZE; i++){
		if (freqs[i] > 0){
			pq_push(create_leaf(i, freqs[i]));
		}
	}

	// empty file, 0 frequencies for all symbols
	if (nodes.size == 0){
		return NULL;
	}

	HuffmanNode * left;
	HuffmanNode * right; 
	HuffmanNode * parent;

	while (nodes.size >= 2){
		left = pq_pop(nodes);
		right = pq_pop(nodes);
		parent = node_merge(left, right);
		pq_push(nodes, parent);
	}

	parent = pq_pop(nodes);

	free(nodes);

	return parent; // the root of all the tree
}

//int build_tree(){
//	while(num_nodes > 1){
//		qsort(nodes, num_nodes, sizeof(struct node), compare_by_frequency); // sorts nodes by freq
//		struct node * left = pop_node(0); // struct node * pop_node(index) returns the pointer to the struct and deletes it from nodes
//		struct node * right = pop_node(0);
//
//		struct node merged;
//		merged.c = 0; // algum número para indicar que não é um node de caractere
//		merged.freq = left->freq + right->freq;
//		merged.left = left;
//		merged.right = right;
//		append_node(merged);
//	}
//
//	return 0;
//}

// int count_frequencies(FILE * file){
// 	// count frequencies
// 	int occurences[CHARS_TO_COUNT] = {0}; // chars de 0 a 255 do ASCII extendido (dá para fazer de 0 a 127 também)
// 	int c = fgetc(file);
// 	for (; c != EOF && c != '\0'; c = fgetc(file)){
// 		occurences[c]++; // adiciona uma ocorrência na posição do número correspondente ao caractere
// 	}
// 
// 	// // MOSTRAR NÚMERO DE OCORRÊNCIAS
// 	// for(int i = 0; i < CHARS_TO_COUNT; i++){
// 	// 	printf("O símbolo %c aparece %d vezes\n\n", i, occurences[i]);
// 	// 	// 	putchar(i);
// 	// }
// 
// 	// construct nodes
// 	struct node thisnode;
// 	for (int i = 0; i < CHARS_TO_COUNT; i++){
// 		if (occurences[i] != 0){
// 			thisnode.c = i;
// 			thisnode.freq = occurences[i];
// 			thisnode.left = thisnode.right = NULL;
// 
// 			num_nodes++;
// 			nodes = realloc(nodes, (num_nodes)*sizeof(struct node));
// 			check_pointer(nodes);
// 			
// 			nodes[num_nodes-1] = thisnode;
// 
// 			// show number of occurences
// 			printf("O símbolo %c aparece %d vezes\n\n", i, occurences[i]);
// 		}
// 	}
// 
// 	return 0;
// }

void free_tree(HuffmanNode *root){
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
