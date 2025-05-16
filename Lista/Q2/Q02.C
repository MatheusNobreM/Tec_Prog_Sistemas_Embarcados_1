#include <stdio.h>

int main() {
    unsigned char reg = 0b00000000;

    reg |= (1<<2);

    for (int i = 7; i>=0; i--) {
        printf("%d", (reg>>i) & 1);

    }
    printf("\n");
    return 0;
}