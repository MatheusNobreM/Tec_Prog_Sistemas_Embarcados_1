//Encontrar o complemento 1 do seguinte: 0xC6, 0x35, 0xd3 e 0xC7
#include <stdio.h>

int main(){
    unsigned char a = 0xC6;
    unsigned char b = 0x35;
    unsigned char c = 0xd3;
    unsigned char d = 0xC7;

    unsigned char complementoA = ~a;
    unsigned char complementoB = ~b;
    unsigned char complementoC = ~c;
    unsigned char complementoD = ~d;

    printf("Complemento de 0x%x: 0x%x\n", a, complementoA);
    printf("Complemento de 0x%x: 0x%x\n", b, complementoB);
    printf("Complemento de 0x%x: 0x%x\n", c, complementoC);
    printf("Complemento de 0x%x: 0x%x\n", d, complementoD);

}