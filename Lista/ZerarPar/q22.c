#include <stdio.h>

// Função que imprime os bits de um unsigned char
void print_binary(unsigned char val) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (val >> i) & 1);
    }
    printf("\n");
}

void clear_even_bits(unsigned char *x) {
    if (x == NULL) return;

    // Máscara com bits ímpares ligados: 0b10101010 = 0xAA
    *x &= 0b10101010;
}

int main() {
    unsigned char val = 0xFF; // 11111111
    printf("Antes: ");
    print_binary(val);

    clear_even_bits(&val);

    printf("Depois: ");
    print_binary(val);

    return 0;
}
