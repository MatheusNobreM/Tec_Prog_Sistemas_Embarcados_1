//Escreva uma função unsigned char mirror_bits(unsigned char value) que inverta os bits de um byte
#include <stdio.h>

void imprimiBit(unsigned char x){
    printf("0b");
    for(int i = 7; i>=0; i--){
        printf("%d", (x >>i) & 1);
    }
    printf("\n");
}

unsigned char mirror_bits(unsigned char value){
    unsigned char result = 0;
    for(int i = 0; i<=7; i++){
        result |= (((value >> i) & 1) << (7 - i));
    }
    return result;
}

int main() {
    unsigned char value = 0b11100001;

    printf("Original: ");
    imprimiBit(value);

    unsigned char mirrored = mirror_bits(value);

    printf("Invertido: ");
    imprimiBit(mirrored);
    
    return 0;
}
