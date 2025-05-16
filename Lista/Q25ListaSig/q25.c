#include <stdio.h>

void imprimiBit(unsigned char x){
    printf("0b");
    for(int i = 7; i>=0; i--){
        printf("%d", (x >>i) & 1);
    }
    printf("\n");
}

unsigned char shift_bits_left(unsigned char x) {
    unsigned char count = 0; // Contador de bits 1
    unsigned char temp = x;
    
    // Conta o número de bits 1
    while (temp) {
        count += temp & 1; // Verifica o bit menos significativo
        temp >>= 1; // Desloca para a direita
    }
    
    // Cria o resultado com 'count' bits 1 à esquerda, 0xff garante o 00000000
    return (0xFF << (8 - count));
}

int main() {
    unsigned char input = 0x56; // 01010110 em binário
    unsigned char result = shift_bits_left(input);
    
    printf("Entrada: ");
    imprimiBit(input);
    printf("Saída: ");
    imprimiBit(result);
    return 0;
}