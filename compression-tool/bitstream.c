#include "bitstream.h" 

void bw_init(BitWriter *bw, FILE *f) { 
    
    bw->file = f; 
    bw->current_byte = 0; 
    bw->bit_count = 0; 
    
}
//Escreve UM bit (0 ou 1). Quando o byte junta 8 bits, ele é gravado no arquivo.
void bw_write_bit(BitWriter *bw, int bit){ 
    
    // "<< 1" abre espaço à direita; "bit & 1" mantém só o último bit (0 ou 1)
    // "|" encaixa esse bit na posição que foi aberta, isso ficou mais claro agora
    bw->current_byte = (bw->current_byte << 1) | (bit & 1); 
    bw->bit_count++; // conta todos os bits até os zeros
    
    if (bw->bit_count == 8){ // se estiver cheio
        
        fputc(bw->current_byte, bw->file); 
        bw->current_byte = 0; 
        bw->bit_count = 0;
        
    }
    
}

void bw_write_bits(BitWriter *bw, uint32_t bits, uint8_t len) {
    for (int i = len - 1; i >= 0; i--) {
        bw_write_bit(bw, (bits >> i) & 1);
    }
}


void count_frequencies(FILE *in, int freqs[256]) { // Conta de 0 a 255
    for (int i = 0; i < 256; i++) freqs[i] = 0;
    int c;
    while ((c = fgetc(in)) != EOF) {
        freqs[c]++;
    }
}

void bw_flush (BitWriter *bw) { // Grava o último byte incompleto (se sobrou algum bit), completando com zeros!!!
    
    if (bw->bit_count >0){ // sobrou um byte incompleto 
        
        bw->current_byte = bw->current_byte << (8 - bw->bit_count); 
        //Empurra os bits que existem para a esquerda do byte os espaços que ficam à direita viram zeros 
        //Por exemplo: 3 bits -> 8 - 3 = 5 posições, então vai ficar 00000101 << 5 = 10100000
        
        fputc(bw->current_byte, bw->file); 
        bw->current_byte = 0;
        bw->bit_count = 0; 
        
    }
    
}

// int main(){
//     FILE * file = fopen("testedeescrita", "w");
//     BitWriter writer;
//     bw_init(&writer, file);
//     bw_write_bits(&writer, 0b1011, 4);
//     bw_write_bits(&writer, 0b0101, 4);
//     fclose(file);
// }