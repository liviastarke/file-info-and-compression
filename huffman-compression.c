#include <stdlib.h>
#include <stdio.h>

#define CHARS_TO_COUNT 256

struct node {
	char c;
	int freq;
	struct node * left;
	struct node * right;
};

struct node * pop_node(int index);

int num_nodes = 0;
struct node * nodes; // nodes list is global

// void count(char* string){
// 	int occurences[27] = {0};
// 	char c;
// 	for(int i = 0, c = string[i]; c != '\0'; i++){
// 		occurences[c - 'a']++;
// 		c = string[i];
// 	}
// 	for(int i = 0; i<27; i++){
// 		printf("O símbolo %c aparece %d vezes", 'a'+i, occurences[i]);
// 	}
// }

void append_node(struct node thisnode){
	nodes = realloc(nodes, (num_nodes)*sizeof(struct node));
	nodes[num_nodes-1] = thisnode;
}

struct node * pop_node(int index){
	struct node * popped = malloc(sizeof(struct node));
	*popped = nodes[0];

	for (int i = 1; i <= num_nodes; i++){ // copiar node por node ou será que é mais fácil mover o ponteiro uma posição para frente?
		nodes[i-1] = nodes[i];
	}

	num_nodes--;
	nodes = realloc(nodes, num_nodes*sizeof(struct node));
	
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
			nodes = realloc(nodes, (i+1)*sizeof(struct node));
			num_nodes = i + 1;
			nodes[i] = thisnode;

			// show number of occurences
			printf("O símbolo %c aparece %d vezes\n\n", i, occurences[i]);
		}
	}

	return 0;
}

int build_tree(){
	while(sizeof(*nodes) > 1){
		qsort(nodes, num_nodes, sizeof(struct node), compare_by_frequency); // sorts nodes by freq
		struct node * left = pop_node(0); // struct node * pop_node(index) returns the pointer to the struct and deletes it from nodes
		struct node * right = pop_node(0);

		struct node merged;
		merged.left = left;
		merged.left = right;
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
	return 0;
}