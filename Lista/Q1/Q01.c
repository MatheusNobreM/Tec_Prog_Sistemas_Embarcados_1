#include <stdio.h>

#define is_upper(c) ((c)>='A' && (c)<='Z')

int main() {
     unsigned char ch = 'D';

     if(is_upper(ch)) {
        printf("%c é uma letra maiúscula\n", ch);
     } else {
        printf("%c é uma letra minúscula\n", ch);
     }

     return 0;
}