//Escreva uma macro is_digit que retorne TRUE se seu argumento for um dígito decimal
#include <stdio.h>

#define is_digit(c) (((c) >= 0) && ((c) <= 9))

int main(){
    int c=3;

    if(is_digit(c)){
        printf("TRUE\n");
    } else {
        printf("FALSE\n");
    }

    return 0;
}