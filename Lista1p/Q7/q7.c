#include <stdio.h>

void imprime_binario(unsigned char n) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}

// Contar os bits iguais
int compare_bits(unsigned char x, unsigned char y) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (((x >> i) & 1) == ((y >> i) & 1)) {
            count++;
        }
    }
    return count;
}

int main() {
    unsigned char x = 0b10101010;
    unsigned char y = 0b10011010;
    
    printf("x: ");
    imprime_binario(x);
    printf("y: ");
    imprime_binario(y);

    int z = compare_bits(x, y);
    printf("Quantidade bits iguais: %d\n", z);

    return 0;
}