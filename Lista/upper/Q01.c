#include <stdio.h>

#define is_upper(ch) ((ch) >= 'A'&& (ch) <='Z')

int main() {
   char ch  = 'i';

   if(is_upper(ch)) {
      printf("%c is upper case\n", ch);
   } else {
      printf("%c is not upper case\n", ch);
   }
   return 0;
}