// I think I will just put this in header files...
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CHARS_TO_COUNT 256
#define ENCODED_PREFIX_STR "encoded-"

struct node {
	char c;
	int freq;
	struct node * left;
	struct node * right;
};

struct char_code {
	char c;
	char code[CHARS_TO_COUNT]; // Enough space for the longest possible path + null terminator
	//int code;
};


struct node * pop_node(int index);

int num_nodes = 0; // the number of elements in nodes. Always bigger than max index of nodes by 1. Equal to the multiplier of last allocation.
struct node * nodes; // nodes list is global

int index_codes = 0;

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
	//printf("\nfreeing: NODE %d\nc: %c\nfreq: %d\n", n, node->c, node->freq);
	if(node->left){
		free_node(node->left, ++n);
	}
	if(node->right){
		free_node(node->right, ++n);
	}
	free(node);
}

void free_nodes(){
	int total_nodes = num_nodes;
	for (int i = 0; i < total_nodes; i++){
		free_node(&nodes[i], i);
		num_nodes--;
	}
}



int check_pointer(void * ptr){
	if (!ptr){
		printf("Allocation error!\n");
		perror(NULL);
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

int find_code_index(struct char_code *array, int size, char target_char) {
    for (int i = 0; i < size; i++) {
        if (array[i].c == target_char) {
            return i; // Found
        }
    }
    return -1; // Not found
}
int find_char_index(struct char_code *array, int size, char * target_code) {
    for (int i = 0; i < size; i++) {
        if (strcmp(array[i].code, target_code) == 0) {
            return i; // Found
        }
    }
    return -1; // Not found
}

// to be used by <stdlib.h>'s qsort
int compare_by_frequency(const void * a, const void * b){
	const struct node * node_a = (const struct node *) a;
	const struct node * node_b = (const struct node *) b;
	return (node_a->freq - node_b->freq); // ascending order, change a and b for descending order
}

int count_frequencies(FILE * file){
	// count frequencies
	int occurences[CHARS_TO_COUNT] = {0}; // chars de 0 a 255 do ASCII extendido (dá para fazer de 0 a 127 também)
	int c = fgetc(file);
	for (; c != EOF && c != '\0'; c = fgetc(file)){
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
		merged.c = 0; // algum número para indicar que não é um node de caractere
		merged.freq = left->freq + right->freq;
		merged.left = left;
		merged.right = right;
		append_node(merged);
	}
	
	return 0;
}

int huffman_encode(struct node * current_node, char * current_code, int depth, struct char_code * codes){
	if(!current_node){
		return 0;
	}
	if (current_node->c != 0 && !current_node->left && !current_node->right){
		current_code[depth] = '\0';
		codes[index_codes].c = current_node->c;
		strncpy(codes[index_codes].code, current_code, CHARS_TO_COUNT*sizeof(char)); // codes[index_codes].code = current_code;
		index_codes++;
	}
	current_code[depth] = '0';
	huffman_encode(current_node->left, current_code, depth+1, codes);
	current_code[depth] = '1';
	huffman_encode(current_node->right, current_code, depth+1, codes);
	return 1;
}


char is_in(struct char_code * codes, int size, char * code){
	int index = find_char_index(codes, size, code);
	if (index == -1){
		return 0;
	}
	return codes[index].c;
}

// char * huffman_decode(char * string, int size, struct char_code * codes, int num_codes){
// 	char current_code[CHARS_TO_COUNT]; // remember CHARS_TO_COUNT is equal to the max length of a code
// 	char * decoded = NULL;
// 	int decoded_len;
// 	for (int i = 0, decoded_len = 0; i < size && string[i] != EOF; i++){
// 		for (int j = 0; j < CHARS_TO_COUNT; j++){
// 			current_code[j] = string[i];
// 			current_code[j+1] = '\0';
// 			char c = is_in(codes, num_codes, current_code);
// 			if (c){
// 				decoded = realloc(decoded, sizeof(char)*(decoded_len+2));
// 				decoded[decoded_len] = c;
// 				decoded_len++;
// 				strncpy(current_code, "\0", 2*sizeof(char));
// 				j = 0;
// 			}
// 		}
// 	}
// 	decoded = realloc(decoded, sizeof(char)*(decoded_len+1));
// 	decoded[decoded_len] = '\0';
// 	return decoded;
// }
char * huffman_decode(char * string, int num_bits, struct char_code * codes, int num_codes){
    char current_code[CHARS_TO_COUNT]; 
    char * decoded = NULL;
    int decoded_len = 0;
    int j = 0; // This is our "accumulator" index

    for (int i = 0; i < num_bits; i++){
        current_code[j] = string[i];
        current_code[j + 1] = '\0'; // Manually null-terminate so strcmp works
        char c = is_in(codes, num_codes, current_code);
        
        if (c != 0){
            // Match found! Grow the decoded string
            decoded = realloc(decoded, decoded_len + 2); // +1 for char, +1 for \0
            decoded[decoded_len] = c;
            decoded_len++;
            
            j = 0; // Clear the accumulator to start the next character
        } 
		else {
            // No match yet, move to the next position in current_code
            j++;
            
            // Safety check to prevent overflow
            if (j >= CHARS_TO_COUNT - 1) {
                printf("Error detected: Code exceeded max length!\n");
                break;
            }
        }
    }

    if (decoded) {
        decoded[decoded_len] = '\0'; // Finalize the string
    }
    return decoded;
}

int main(int argc, char **argv){

	// UNHAPPY PATH
	if (argc != 2){
		printf("HUFFMAN COMPRESSION: use comand + file path to compress a text file\n");
		exit(EXIT_FAILURE);
	}

	char* path = argv[1];
	FILE* file = fopen(path, "r");

	count_frequencies(file);

	int total_chars = num_nodes;
	print_nodes();

	build_tree();

	//int codes[total_chars];
	struct char_code codes[total_chars];
	char buf[CHARS_TO_COUNT];
	huffman_encode(&nodes[0], buf, 0, codes);

	printf("\nGenerated codes:\n");
	for(int i = 0; i < total_chars; i++){
		printf("%c: %s\n", codes[i].c, codes[i].code);
	}

	fseek(file, 0, SEEK_END);
	char encoded_buf[256*ftell(file)]; // assuming the encoded file is smaller than the original

	printf("\nEncoded text:\n");
	fseek(file, 0, SEEK_SET);
	char c = fgetc(file);
	int num_comp_bits, num_bytes_file;
	for (num_bytes_file = num_comp_bits = 0; c != EOF && c != '\0'; c = fgetc(file), num_bytes_file++){
		strncpy(buf, codes[find_code_index(codes, total_chars, c)].code, CHARS_TO_COUNT);
		printf("%s\n", buf);
		strncpy(encoded_buf + num_comp_bits, buf, CHARS_TO_COUNT);

		// Calculate number of bits of compressed text
		int i;
		for (i = 0; buf[i] != '\0'; i++);
		num_comp_bits += i;
	}
	
	printf("\nTotal number of bits in compressed file is %d\n", num_comp_bits);
	printf("Estimated number of bits in original file is %d\n\n", num_bytes_file*8);

	printf("RESULTADOS: O número total de bits no arquivo original é %d\n", num_bytes_file*8);
	printf("RESULTADOS: O número total de bits no arquivo compactado é %d\n", num_comp_bits);
	printf("RESULTADOS: A taxa de compressão é %.2f%%\n\n", (float) 100 * (num_bytes_file*8 - num_comp_bits) / (num_bytes_file*8));

	printf("%s\n", encoded_buf);
	
	fclose(file);

	char * decoded = huffman_decode(encoded_buf, num_comp_bits, codes, total_chars);
	printf("%s\n", decoded);

	//
	// write file
	//

	char * dest = malloc(strlen(ENCODED_PREFIX_STR) + strlen(path) + 1);
	check_pointer(dest);
	strcpy(dest, ENCODED_PREFIX_STR);
	strcat(dest, path);
	file = fopen(dest, "w");
	free(dest); // free filename
	check_pointer(file);

	char byte = 0;
	for (int i = 0; i < strlen(encoded_buf); i++){
		if (encoded_buf[i] == '1'){
			byte++;
		}
		byte << 1;
		if (i%8 == 0){ // a cada byte (8 bits)
			fwrite(&byte, sizeof(char), 1, file);
			byte = 0;
		}
	}
	fclose(file);

	//
	// exit
	//

	free_nodes();
	print_nodes(); // shows nothing
	return 0;
}
