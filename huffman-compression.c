// I think I will just put this in header files...
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CHARS_TO_COUNT 256

struct node {
	char c;
	int freq;
	struct node * left;
	struct node * right;
};

struct node * pop_node(int index);

int num_nodes = 0; // the number of elements in nodes. Always bigger than max index of nodes by 1. Equal to the multiplier of last allocation.
struct node * nodes; // nodes list is global


// void print_node(struct node * node, char * identifier){
// 	printf("\nNODE %s\nc: %c\nfreq: %d\n", identifier, node->c, node->freq);
// 	if(node->left){
// 		print_node(node->left, strcat(identifier, " - left")); // concatenate strings
// 	}
// 	if(node->right){
// 		print_node(node->right, strcat(identifier, " - right"));
// 	}
// }
// void print_nodes(){
// 	for (int i = 0; i < num_nodes; i++){
// 		print_node(&nodes[i], (char[2]){'0'+i, '\0'});
// 	}
// } // stack smashing detected: terminated


void print_node(struct node * node, int depth) {
    if (!node || num_nodes == 0) return;

    // Print indentation based on depth
    for (int i = 0; i < depth; i++) printf("\t");
    
    printf("NODE: char: %c, freq: %d\n", node->c, node->freq);

    if (node->left) {
        printf("Go Left: \n");
        print_node(node->left, depth + 1);
    }
    if (node->right) {
        printf("Go Right: \n");
        print_node(node->right, depth + 1);
    }
}

void print_nodes(){
	for (int i = 0; i < num_nodes; i++){
		print_node(&nodes[i], 0);
	}
}


// void free_node(struct node * node, char * identifier){
// 	printf("\nfreeing: NODE %s\nc: %c\nfreq: %d\n", identifier, node->c, node->freq);
// 	if(node->left){
// 		print_node(node->left, strcat(identifier, " - left"));
// 	}
// 	if(node->right){
// 		print_node(node->right, strcat(identifier, " - right"));
// 	}
// 	free(node);
// }
// void free_nodes(){
// 	for (int i = 0; i < num_nodes; i++){
// 		free_node(&nodes[i], (char[2]){'0'+i, '\0'});
// 	}
// }

void free_node(struct node * node, int n){
	printf("\nfreeing: NODE %d\nc: %c\nfreq: %d\n", n, node->c, node->freq);
	if(node->left){
		free_node(node->left, ++n);
	}
	if(node->right){
		free_node(node->right, ++n);
	}
	free(node);
}

void free_nodes(){
	for (int i = 0; i < num_nodes; i++){
		free_node(&nodes[i], i);
		num_nodes--;
	}
}



int check_pointer(void * ptr){
	if (!ptr){
		printf("Allocation error!\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}
int check_nodes_pointer(){
	if(num_nodes == 0){
		if (nodes != NULL){
			printf("A problem was detected.\n");
			exit(EXIT_FAILURE);
		}
		return 0;
	}
	return check_pointer(nodes);
}

void append_node(struct node thisnode){
	printf("\nAppending nodes from state:\n"); print_nodes(); // DEPURAÇÃO

	num_nodes++;
	nodes = realloc(nodes, (num_nodes)*sizeof(struct node));
	check_pointer(nodes);
	nodes[num_nodes-1] = thisnode;
}

struct node * pop_node(int index){
	printf("\nPopping nodes from state:\n"); print_nodes(); // DEPURAÇÃO

	struct node * popped = malloc(sizeof(struct node));
	check_pointer(popped);
	*popped = nodes[0];

	if (num_nodes > 1){
		for (int i = 1; i < num_nodes; i++){ // copiar node por node ou será que é mais fácil mover o ponteiro uma posição para frente?
			nodes[i-1] = nodes[i];
		}
	}

	num_nodes--;
	nodes = realloc(nodes, num_nodes*sizeof(struct node));
	check_nodes_pointer();
	
	return popped;
}

// to be used by <stdlib.h>'s qsort
int compare_by_frequency(const void * a, const void * b){
	const struct node * node_a = (const struct node *) a;
	const struct node * node_b = (const struct node *) b;
	return (node_a->freq - node_b->freq); // ascending order, change a and b for descending order
}

int count_frequencies(){
	// count frequencies
	int occurences[CHARS_TO_COUNT] = {0}; // chars de 0 a 255 do ASCII extendido (dá para fazer de 0 a 127 também)
	int c = getchar(); // fgetc(file);
	for (; c != EOF && c != '\0'; c = getchar()){
		occurences[c]++; // adiciona uma ocorrência na posição do número correspondente ao caractere
	}

	// // MOSTRAR NÚMERO DE OCORRÊNCIAS
	// for(int i = 0; i < CHARS_TO_COUNT; i++){
	// 	printf("O símbolo %c aparece %d vezes\n\n", i, occurences[i]);
	// 	// 	putchar(i);
	// }

	// construct nodes
	struct node thisnode;
	for (int i = 0; i < CHARS_TO_COUNT; i++){
		if (occurences[i] != 0){
			thisnode.c = i;
			thisnode.freq = occurences[i];
			thisnode.left = thisnode.right = NULL;

			num_nodes++;
			nodes = realloc(nodes, (num_nodes)*sizeof(struct node));
			check_pointer(nodes);
			
			nodes[num_nodes-1] = thisnode;

			// show number of occurences
			printf("O símbolo %c aparece %d vezes\n\n", i, occurences[i]);
		}
	}

	return 0;
}

// this function operates directly on global var nodes and does not require args.
int build_tree(){
	while(num_nodes > 1){
		qsort(nodes, num_nodes, sizeof(struct node), compare_by_frequency); // sorts nodes by freq
		struct node * left = pop_node(0); // struct node * pop_node(index) returns the pointer to the struct and deletes it from nodes
		struct node * right = pop_node(0);

		struct node merged;
		// merged.c = 257; // algum número para indicar que não é um node de caractere?
		merged.freq = left->freq + right->freq;
		merged.left = left;
		merged.right = right;
		append_node(merged);
	}
	
	return 0;
}

int main(int argc, char **argv){

	// UNHAPPY PATH
	if (argc != 2){
		printf("HUFFMAN COMPRESSION: use comand + file path to compress a text file");
		exit(EXIT_FAILURE);
	}

	// char* path = argv[1];
	// FILE* file = fopen(path, "r");
	// fread(&buf, sizeof(), 1, file);

	count_frequencies();
	print_nodes();
	build_tree();
	free_nodes();
	print_nodes();
	return 0;
}