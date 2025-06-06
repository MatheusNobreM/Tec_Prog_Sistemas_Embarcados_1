#include <stdio.h>

#define IS_NEGATIVE(x) ((x) < 0)

int main() {
    int x = -95; 

    if (IS_NEGATIVE(x)) {
        printf("x é negativo\n");
    } else {
        printf("x Positivo\n");
    }

    return 0;
}
