#include <stdio.h>

#define is_digit(c) (((c) >= '0') && ((c) <= '9'))

#define is_hex(c) (is_digit(c) || (((c) >= 'A') && ((c) <= 'F')) || (((c) >= 'a') && ((c) <= 'f')))

int main() {
    char c = '2';

    if (is_hex(c)) {
        printf("TRUE\n");
    } else {
        printf("FALSE\n");
    }

    return 0;
}