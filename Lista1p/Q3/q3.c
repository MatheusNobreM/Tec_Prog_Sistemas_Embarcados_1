//Considere unsigned char status = 0b11101111;. Escreva uma linha de código que zere o bit 4
#include <stdio.h>

void imprime_binario(unsigned char n) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
    printf("\n");
}

int main() {
    unsigned char status = 0b11111111;

    printf("Status antes de zerar o bit 4: ");
    imprime_binario(status);

    status &= ~(1 << 4);
    printf("Status depois de zerar o bit 4: ");
    imprime_binario(status);
    
    return 0;
}
