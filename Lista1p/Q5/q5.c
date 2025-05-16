//Implemente uma função int compare_bits(unsigned int a, unsigned int b) que retorna o número de bits iguais entre dois inteiros.
#include <stdio.h>

void imprime_binario(unsigned char n) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}

int compare_bits(unsigned char a, unsigned char b) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (((a >> i) & 1) == ((b >> i) & 1)) {
            count++;
        }
    }
    return count;
}

int main() {
    unsigned char a = 0b10101010;
    unsigned char b = 0b10011010;
    
    printf("a: ");
    imprime_binario(a);
    printf("b: ");
    imprime_binario(b);

    int iguais = compare_bits(a, b);
    printf("Número de bits iguais: %d\n", iguais);

    return 0;
}
