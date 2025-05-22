#include <stdio.h>

// Função para imprimir um número em binário (8 bits)
void print_binary(unsigned int num) {
    for (int i = 7; i >= 0; i--) {
        printf("%c", (num & (1 << i)) ? '1' : '0');
    }
}

// Função que retorna os bits de 'value' que estão ligados em 'mask'
unsigned int get_masked_bits(unsigned int value, unsigned int mask) {
    return value & mask;
}

int main() {
    unsigned int value = 0b11011010; // 218
    unsigned int mask  = 0b11110000; // 240

    unsigned int result = get_masked_bits(value, mask);

    printf("Valor original : 0x%X (%3u) | bin: ", value, value);
    print_binary(value);
    printf("\n");

    printf("Mascara        : 0x%X (%3u) | bin: ", mask, mask);
    print_binary(mask);
    printf("\n");

    printf("Resultado      : 0x%X (%3u) | bin: ", result, result);
    print_binary(result);
    printf("\n");

    return 0;
}
