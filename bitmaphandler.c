#include <stdio.h> // cuidado com launch.json
#include <stdlib.h>

// STRUCTS
#pragma pack(push, 1) // Diretiva de pré-processador: Salva o alinhamento (padding) atual e define o novo para 1 byte (CÓDIGO DE IA)
struct header{
	short signature;
	int fileSize;
	int reserved;
	int dataOffset;
};

struct infoHeader{
	int size;
	int width;
	int height;
	short planes;
	short bitsPerPixel; // relaciona-se com colorsUsed
	int compression;
	int imageSize;
	int xPixelsPerM; // resolução em pixels por metro
	int yPixelsPerM;
	int colorsUsed; // quantas cores realmente usadas
	int importantColors;
};

struct bmpHeader{
	struct header header;
	struct infoHeader infoHeader;
};
#pragma pack(pop) // Restaura o alinhamento original do sistema (CÓDIGO DE IA)

// DECLARAÇÕES - PROCEDIMENTOS E FUNÇÕES
void calcDataSize(int px);
int getWidth(FILE *);
int getHeight(FILE *);
int getImageDimensions(FILE *);
void printBmpHeader(struct bmpHeader bmp);
void printResumo(struct bmpHeader bmp);


int main(int argc, char **argv){
	if (argc != 2){
		printf("bitmaphandler: use comando + path para estimar o tamanho em bytes");
		exit(EXIT_FAILURE);
	}

	printf("Bem vindo ao bitmap handler!\n\n");
	struct bmpHeader bmpdata;

	char* path = argv[1];
	
	FILE* file = fopen(path, "r");

	fread(&bmpdata, sizeof(struct bmpHeader), 1, file);
	// printf("%lu\n\n", sizeof(struct bmpHeader)); // checagem de padding correto, deve sair 54

	// int px = getImageDimensions(file); // exercício anterior
	// calcDataSize(px);

	fclose(file);

	if (bmpdata.header.signature != 0x4d42){
		printf("Warning!\nO arquivo não é um arquivo bmp, os dados estarão incorretos.\n");
	}

	if (bmpdata.header.reserved != 0 || bmpdata.infoHeader.planes != 1){
		printf("Warning!\nO arquivo não é válido.\n");
	}

	printBmpHeader(bmpdata);
	printResumo(bmpdata);
	
	return EXIT_SUCCESS;
}

void printResumo(struct bmpHeader bmp){
	printf("\n\nInformações importantes do header:\n\n");
	printf("Tamanho do arquivo em bytes: %5d\n", bmp.header.fileSize);
	printf("Lagura da imagem em pixels: %5d\n", bmp.infoHeader.width);
	printf("Altura da imagem em pixels: %5d\n", bmp.infoHeader.height);
	printf("Número de planos da imagem: %5d\n", bmp.infoHeader.planes);
	printf("\n");
}

// Função para exibir todos os dados da struct (GERADO POR IA para fins de verificação)
void printBmpHeader(struct bmpHeader bmp) {
    printf("=== BMP HEADER ===\n");
    printf("Signature:      0x%X\n", bmp.header.signature); // Exibido em Hex (ex: 0x4D42 para 'BM')
    printf("File Size:      %d bytes\n", bmp.header.fileSize);
    printf("Reserved:       %d\n", bmp.header.reserved);
    printf("Data Offset:    %d\n", bmp.header.dataOffset);

    printf("\n=== INFO HEADER ===\n");
    printf("Header Size:    %d\n", bmp.infoHeader.size);
    printf("Width:          %d px\n", bmp.infoHeader.width);
    printf("Height:         %d px\n", bmp.infoHeader.height);
    printf("Planes:         %d\n", bmp.infoHeader.planes);
    printf("Bits Per Pixel: %d\n", bmp.infoHeader.bitsPerPixel);
    printf("Compression:    %d\n", bmp.infoHeader.compression);
    printf("Image Size:     %d bytes\n", bmp.infoHeader.imageSize);
    printf("X Pixels/M:     %d\n", bmp.infoHeader.xPixelsPerM);
    printf("Y Pixels/M:     %d\n", bmp.infoHeader.yPixelsPerM);
    printf("Colors Used:    %d\n", bmp.infoHeader.colorsUsed);
    printf("Important Col:  %d\n", bmp.infoHeader.importantColors);
    printf("-------------------\n");
}



// FUNÇÕES DO EXERCÍCIO ANTERIOR

void calcDataSize(int px){
	printf("O número de pixels é %d\n", px);

	float black_white = (float) 1 / 8;
	int grayscale = 1;
	int paletted = 1;
	int truecolor = 3;

	black_white *= px;
	grayscale *= px;
	paletted *= px;
	truecolor *= px;

	printf("Black and white:\t%.2f\n", black_white);
	printf("Grayscale:\t%d\n", grayscale);
	printf("Paletted:\t%d\n", paletted);
	printf("True color (RGB):\t%d\n", truecolor);
}

int getImageDimensions(FILE * file){
	int dim = getWidth(file) * getHeight(file);
	return dim;
}

int getWidth(FILE * file){
	int buf;
	fseek(file, 0x12, SEEK_SET); // SEEK_SET = começo do arquivo, offset = 0x0e + 0x12
	
	fread(&buf, 1, sizeof(int),  file);
	printf("width é %d\n", buf);
	return buf;
}

int getHeight(FILE * file){
	int buf;
	fseek(file, 0x16, SEEK_SET); // SEEK_SET = começo do arquivo, offset = 0x0e + 0x12
	
	fread(&buf, 1, sizeof(int),  file);
	printf("height é %d\n", buf);
	return buf;
}