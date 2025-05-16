// Crie uma macro chamada swap_bits(x, i, j) que troque os bits nas posições i e j da variável x
#include <stdio.h>

#define swap_bits(x,i,j) do{\
    if(((x>>(i)) & 1) != (((x>>(j)) & 1))){\
        (x) ^= (1<<(i)) ^ (1<<(j));\
    }\
}while(0)

void imprimiBit(unsigned char x){
    printf("0b");
    for(int i = 7; i>=0; i--){
        printf("%d", (x >>i) & 1);
    }
    printf("\n");
}

int main() {
    unsigned int x = 13; // 0b00001101
    int i = 2, j = 1;   
    printf("Antes: ");
    imprimiBit(x);
    swap_bits(x, i, j);
    printf("Depois: ");
    imprimiBit(x); // 0b00001011 (11)

    return 0;
}