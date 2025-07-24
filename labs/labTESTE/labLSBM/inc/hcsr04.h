#ifndef HCSR04_H
#define HCSR04_H

#include <stdint.h>

// Dispara e mede distância em mm usando HC-SR04
// Retorna 0 em timeout
unsigned int hcsr04_read_mm(void);

#endif // HCSR04_H
