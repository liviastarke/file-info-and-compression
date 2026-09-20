#include "bitstream.h" 

void bw_init(BitWriter *bw, FILE *f) { 
    
    bw->file = f; 
    bw->current_byte = 0; 
    bw->bit_count = 0; 
    
}

void bw_write_bit(BitWriter *bw, int bit){ // analisando se o bit é 0 ou 1
    
    bw->current_byte = (bw->current_byte << 1) | (bit & 1); // aqui seria um Comparação dos bits com número 1, se for maior que 1, tranforma, 
    bw->bit_count++; //passa para o próximo bit, o & vai verificar se é um 1 ou 0, como não sei
    
    if (bw->bit_count == 8){ //caso já tenha chegado a 8 bits
        
        fputc(bw->current_byte, bw->file); 
        bw->current_byte = 0; 
        bw->bit_count = 0;
        
    }
    
}

void bw_flush (BitWriter *bw) { //agora se o arquivo no final de tudo chegou mais que 8
    
    if (bw->bit_count >0){ // se ainda a valores dentro do contador
        
        bw->current_byte = bw->current_byte << (8 - bw->bit_count); // aqui ele vai "empurrar" os valores que sobraram para frente do valor do byte atual, tipo 8 - 3 
       // completando com zeros à direita 10100000
       
        fputc(bw->current_byte, bw->file); 
        bw->current_byte = 0;
        bw->bit_count = 0; 
        
    }
    
}