#include <stdio.h>
#include "imprime.h"
#include "funcao01.h"
#include "funcao02.h"
#include "funcao03.h"

int main() {
    int n1, n2;
    
    imprime("Matheus Moreira");
    
    printf("Digite o primeiro valor: ");
    scanf("%d", &n1);
    printf("Digite o segundo valor: ");
    scanf("%d", &n2);
    
    int resultado01 = funcao01(n1, n2);
    printf("Resultado da FUNCAO1 soma eh: %d\n", resultado01);
    
    int resultado02 = funcao02(n1, n2);
    printf("Resultado da FUNCAO2 subtracao eh: %d\n", resultado02);
    
    int resultado03 = funcao03(n1, n2);
    printf("Resultado da FUNCAO3 multiplicacao eh: %d\n", resultado03);
    
    return 0;
}