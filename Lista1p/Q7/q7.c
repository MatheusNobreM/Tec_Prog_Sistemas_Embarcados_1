// Implemente uma função unsigned int circular_right(unsigned int x, int n) que rotacione x para a direita n vezes
#include <stdio.h>

void imprimiBit(unsigned char x){
    printf("0b");
    for(int i = 7; i>=0; i--){
        printf("%d", (x >>i) & 1);
    }
    printf("\n");
}

unsigned int circular_right(unsigned int x, int n){
    unsigned int rotacionado = 0;
    rotacionado = (x >> n) | (x<< (8-n));
    return rotacionado;
}

int main() {
    unsigned int x = 0b10110011;
    int n = 3;
    int rotacionado = 0;
    
    printf("x: ");
    imprimiBit(x);

    rotacionado = circular_right(x, n);
    printf("Rotacionado %d: ", n);
    imprimiBit(rotacionado);

    return 0;
}