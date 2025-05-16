//Crie uma macro chamada CLEAR_BIT(x, b) que limpe o bit na posição b da variável x
#include <stdio.h>

#define Clear_Bit(x,b) ((x) &= ~(1 << (b)))

void imprimiBit(unsigned char x){
    printf("0b");
    for(int i = 7; i>=0; i--){
        printf("%d", (x >>i) & 1);
    }
    printf("\n");
}

int main() {
    unsigned char x = 0b11111111;
    int b = 0;
    printf("Valor inicial: ");
    imprimiBit(x);
    Clear_Bit(x,b);
    printf("Valor final: ");
    imprimiBit(x);

    return 0;
}