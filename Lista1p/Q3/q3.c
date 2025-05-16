//Considere unsigned char status = 0b11101111;. Escreva uma linha de código que zere o bit 4
#include <stdio.h>

void imprimiBit(unsigned char x){
    printf("0b");
    for(int i = 7; i>=0; i--){
        printf("%d", (x >>i) & 1);
    }
    printf("\n");
}

int main() {
    unsigned char status = 0b11111111;

    printf("Status inicial: ");
    imprimiBit(status);

    status &= ~(1 << 4);
    printf("Status final: ");
    imprimiBit(status);
    
    return 0;
}

