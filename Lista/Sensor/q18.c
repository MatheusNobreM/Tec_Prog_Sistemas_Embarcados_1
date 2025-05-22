#include <stdio.h>

// Definindo a estrutura SensorData
typedef struct {
    int id;
    float temperature;
    float humidity;
} SensorData;

// Função para imprimir os dados de um sensor
void print_sensor(SensorData s) {
    printf("Sensor ID     : %d\n", s.id);
    printf("Temperatura   : %.2f °C\n", s.temperature);
    printf("Umidade       : %.2f %%\n", s.humidity);
}

int main() {
    // Exemplo de uso
    SensorData sensor1 = {101, 26.5, 60.2};

    print_sensor(sensor1);

    return 0;
}
