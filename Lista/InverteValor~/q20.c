#include <stdio.h>

// Função para imprimir 8 bits em binário
void print_binary_8bits(unsigned char num) {
    for (int i = 7; i >= 0; i--) {
        printf("%c", (num & (1 << i)) ? '1' : '0');
    }
    printf("\n");
}

int main() {
    unsigned char x = 0b10101010;  // valor inicial 8 bits

    printf("Antes da inversão: ");
    print_binary_8bits(x);

    x = ~x;  // inverte todos os bits

    printf("Depois da inversão: ");
    print_binary_8bits(x);

    return 0;
}
