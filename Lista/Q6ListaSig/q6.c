//Crie uma macro MASK_BITS(x, m) que aplique uma máscara m sobre x e retorne o resultado
#include <stdio.h>

#define MASK_BITS(x,y) (x & y)

void imprimeBit(unsigned char x){
    printf("0b");
    for(int i = 7; i>=0; i--){
        printf("%d", (x >> i)& 1);
    }
    printf("\n");
}

int main() {
    unsigned char x = 0b01010101;
    unsigned char y = 0b00000000;

    printf("x: ");
    imprimeBit(x);
    printf("y: ");
    imprimeBit(y);

    unsigned char mask = MASK_BITS(x, y);
    printf("Mascara: ");
    imprimeBit(mask);


}