#include <stdio.h>
#include <stdbool.h>

unsigned char setBits(bool w, unsigned int i, unsigned int j, unsigned char value) {
    if (i > 7) return value;      // índice fora do byte
    if (j > 7) j = 7;             // limitar j a 7
    if (i > j) return value;      // intervalo inválido
    
    unsigned char mask = ((1 << (j - i + 1)) - 1) << i;
    
    if (w) {
        value |= mask;    // liga bits no intervalo
    } else {
        value &= ~mask;   // desliga bits no intervalo
    }
    
    return value;
}
void imprimiBit(unsigned char x) {
    printf("0b");
    for(int k = 7; k >= 0; k--) {
        printf("%d", (x >> k) & 1);
    }
    printf("\n");
}

int main() {
    unsigned char val = 0b00001111; // 15 decimal
    
    printf("Antes:  ");
    imprimiBit(val);
    
    val = setBits(true, 4, 7, val);
    printf("Depois (set bits 4-7 para 1): ");
    imprimiBit(val);
    
    val = setBits(false, 0, 3, val);
    printf("Depois (clear bits 0-3): ");
    imprimiBit(val);
    
    return 0;
}
