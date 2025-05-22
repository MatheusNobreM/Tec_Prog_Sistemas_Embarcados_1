#include <stdio.h>

// Função que ativa o bit 'bit' de *reg somente se ele estiver desligado
void set_bit_if_zero(unsigned int *reg, unsigned int bit) {
    if (((*reg) & (1 << bit)) == 0) {  // Verifica se o bit está desligado
        *reg |= (1 << bit);            // Ativa o bit
    }
}

// Função auxiliar para imprimir em binário (8 bits)
void print_binary(unsigned int num) {
    for (int i = 7; i >= 0; i--) {
        printf("%c", (num & (1 << i)) ? '1' : '0');
    }
}

int main() {
    unsigned int reg = 0b10100000; // Valor inicial: bit 2 está desligado

    printf("Antes: ");
    print_binary(reg);
    printf(" (0x%X)\n", reg);

    set_bit_if_zero(&reg, 2); // Vai ativar o bit 2 (começando do 0)

    printf("Depois: ");
    print_binary(reg);
    printf(" (0x%X)\n", reg);

    return 0;
}
