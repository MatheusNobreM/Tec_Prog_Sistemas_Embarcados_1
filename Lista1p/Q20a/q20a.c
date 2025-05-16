// Encontrar o bitwise e, ou e xor dos seguintes caso 0xC6 com 0x35
#include <stdio.h>
int main() {
    unsigned char x = 0xC6, y = 0x35;

    printf("and: 0x%02X\n", x & y); 
    printf("or: 0x%02X\n", x | y);  
    printf("xor: 0x%02X\n", x ^ y); 
    
    return 0;
}