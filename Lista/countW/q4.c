#include <stdio.h>
#include <stdbool.h>

int countBits(bool w, unsigned int value) {
    int count = 0;
    for (int i = 0; i < 8; i++) {  // unsigned int tem 32 bits
        int bit = (value >> i) & 1;
        if (bit == w) {
            count++;
        }
    }
    return count;
}

int main() {
    unsigned int val = 0b10101000; 

    printf("Valor: %d (binário: 0b10101000)\n", val);
    printf("Número de bits 1: %d\n", countBits(true, val));
    printf("Número de bits 0: %d\n", countBits(false, val));

    return 0;
}
