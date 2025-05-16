//Implemente uma função int compare_bits(unsigned int a, unsigned int b) que retorna o número de bits iguais entre dois inteiros.
#include <stdio.h>

void imprimiBit(unsigned char x){
    printf("0b");
    for(int i = 7; i>=0; i--){
        printf("%d", (x >>i) & 1);
    }
    printf("\n");
}

int compare_bits(unsigned int a, unsigned int b) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (((a >> i) & 1) == ((b >> i) & 1)) {
            count++;
        }
    }
    return count;
}

int main() {
    unsigned int a = 0b10101010;
    unsigned int b = 0b10011010;
    
    printf("a: ");
    imprimiBit(a);
    printf("b: ");
    imprimiBit(b);

    int iguais = compare_bits(a, b);
    printf("Número de bits iguais: %d\n", iguais);

    return 0;
}
