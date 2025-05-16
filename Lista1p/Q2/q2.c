#include <stdio.h>

// Macro para trocar os bits nas posições i e j de x, sem 1ULL
#define swap_bits(x, i, j) do { \
    if (((x >> (i)) & 1) != ((x >> (j)) & 1)) { \
        (x) ^= (1 << (i)) ^ (1 << (j)); \
    } \
} while (0)

// Função para imprimir x em binário (8 bits para simplicidade)
void print_binary(unsigned int x) {
    printf("0b");
    for (int i = 7; i >= 0; i--) {
        printf("%d", (x >> i) & 1);
    }
    printf("\n");
}

int main() {
    unsigned int x = 13; // 0b00001101
    int i = 2, j = 1;   // Trocar bits nas posições 2 e 1
    printf("Antes: ");
    print_binary(x);
    swap_bits(x, i, j);
    printf("Depois: ");
    print_binary(x); // Esperado: 0b00001011 (11)
    return 0;
}