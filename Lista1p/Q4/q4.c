//Escreva uma função unsigned char mirror_bits(unsigned char value) que inverta os bits de um byte
#include <stdio.h>

void imprime_binario(unsigned char num) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

unsigned char mirror_bits(unsigned char value) {
    unsigned char result = 0;
    for (int i = 0; i < 8; i++) {
        result |= ((value >> i) & 1) << (7 - i);
    }
    return result;
}

int main() {
    unsigned char value = 0b11100101;

    printf("Original: ");
    imprime_binario(value);

    unsigned char mirrored = mirror_bits(value);

    printf("Invertido: ");
    imprime_binario(mirrored);
    
    return 0;
}
