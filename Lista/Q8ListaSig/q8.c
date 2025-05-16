//Escreva uma macro que retorne TRUE se seu parâmetro for divisível por 10 e FALSE caso contrário
#include <stdio.h>

#define DIVISIVEL(p) (p%10==0)

int main() {
    int p = 1;

    if(DIVISIVEL(p)) {
        printf("TRUE\n");
    } else {
        printf("FALSE\n");
    }

    return 0;
}