#include <stdio.h> // cuidado com launch.json
#include <stdlib.h>
#include <string.h> // memcpy
#include <arpa/inet.h> // ntohl() para converter big endian - little endian

#define TRUE 1;
#define FALSE 0;

// STRUCTS
#pragma pack(push, 1)
struct chunk{
	unsigned int length;
	unsigned int type;
    unsigned char *data;
	int crc;
};

struct pngFormat{
	uint64_t signature;
	struct chunk* chunks;
};

struct ihdrData{
	int width;
	int height;
	unsigned char bit_depth;
	unsigned char color_type;
	unsigned char compression;
	unsigned char filter;
	unsigned char interlace;
};

#pragma pack(pop)

// DECLARAÇÕES - PROCEDIMENTOS E FUNÇÕES
void print_ihdr(struct pngFormat*);
void print_iend(struct pngFormat*, int);

void checkpointer(void* ptr){
	if (ptr == NULL){
		printf("pnghandler: erro de alocação de memória\n");
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char **argv){
	if (argc != 2){
		printf("pnghandler: use comando + path para obter informações sobre o arquivo\n");
		exit(EXIT_FAILURE);
	}

	printf("Bem vindo ao png handler!\n\n");
	struct pngFormat pngdata;

	char* path = argv[1];
	
	FILE* file = fopen(path, "r");
	if (file == NULL){
		printf("pnghandler: erro, não foi possível abrir o arquivo\n");
		exit(EXIT_FAILURE);
	}

	// READ AND VALIDATE SIGNATURE
	fread(&pngdata.signature, sizeof(pngdata.signature), 1, file);

	if (pngdata.signature != 0xa1a0a0d474e5089){
		printf("Warning!\nO arquivo não é um arquivo PNG, os dados estarão incorretos.\n%lx\n", pngdata.signature);
	}

	// READ CHUNKS
	struct chunk thischunk;
	int elements_num = 0; // equals array index
	int reached_end = FALSE;

	while (!reached_end){
		fread(&thischunk, sizeof(unsigned int), 2, file); // READ length AND type
		thischunk.length = ntohl(thischunk.length);
		thischunk.type = ntohl(thischunk.type); // convert each separatedly

		if (thischunk.type == 0x49454e44){
			reached_end = TRUE;
		}
		thischunk.data = malloc(thischunk.length);
		if(thischunk.length != 0){
			checkpointer(thischunk.data);
			fread(thischunk.data, sizeof(unsigned char), thischunk.length, file);
		}

		fread(&thischunk.crc, sizeof(int), 1, file);

		pngdata.chunks = reallocarray(pngdata.chunks, sizeof(struct chunk), elements_num+1);
		checkpointer(pngdata.chunks);
		*(pngdata.chunks + elements_num) = thischunk;

		// pngdata.chunks = malloc(sizeof(struct chunk)*elements_num);
		elements_num++;
	}

	// struct chunk* thischunk;
	// while (){
	// 	pngdata.chunks = malloc(sizeof(struct chunk));
	// 	fread(thischunk, sizeof(unsigned int), 2, file); // READ length AND type
	// 	thischunk->data = malloc(thischunk->length);
	// 	fread(thischunk->data, sizeof(unsigned char), thischunk->length, file);
	// 	pngdata.chunks = thischunk;
	// }

	fseek(file, 0, SEEK_END);
    printf("O tamanho total do arquivo é %li bytes\n", ftell(file));

	fclose(file);

	print_ihdr(&pngdata);
	print_iend(&pngdata, elements_num);

	// FREE ALL ALLOCATIONS
	for (int i = 0; i < elements_num; i++){
		free(pngdata.chunks[i].data);
	}
	free(pngdata.chunks);
	
	return EXIT_SUCCESS;
}

void print_ihdr(struct pngFormat* pngdata){
	printf("\nIHDR data\n\n");
	struct ihdrData ihdr;
	
	if(pngdata->chunks[0].type == 0x49484452){
		memcpy(&ihdr, pngdata->chunks[0].data, sizeof(ihdr));
	}
	
	printf("Width is %d\n", ntohl(ihdr.width));
	printf("Height is %d\n", ntohl(ihdr.height));
	printf("Bit depth is %d\n", ihdr.bit_depth);
	printf("Color type is %d\n", ihdr.color_type);
	printf("Compression is %d\n", ihdr.compression);
	printf("Filter method is %d\n", ihdr.filter);
	printf("Interlace is %d\n", ihdr.interlace);
}

void print_iend(struct pngFormat* pngdata, int n_chunks){
	printf("\nIEND data\n\n");
	int iend_pos;
	for(int i = 0; i < n_chunks; i++){
		if (pngdata->chunks[i].type == 0x49454e44){
			iend_pos = i;
			break;
		}
	}
	printf("Data field is %d bytes long\n", ntohl(pngdata->chunks[iend_pos].length));

	int type = ntohl(pngdata->chunks[iend_pos].type);
	printf("Type is %x in hex, which in ASCII is equal to ", type);
	char str[5];
	memcpy(str, &type, sizeof(int));
	str[4] = '\0';
	printf("%s\n", str);

	printf("Data field should be empty\n");
	printf("CRC field is %x\n", pngdata->chunks[iend_pos].crc);
}