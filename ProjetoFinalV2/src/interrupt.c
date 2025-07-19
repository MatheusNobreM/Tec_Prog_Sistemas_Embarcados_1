#include "interrupt.h"
#include "gpio.h"
#include "timer.h"
#include "hw_types.h"
#include "soc_AM335x.h"

int gpioIsrHandler(int level, int pino) {
    if (!level) {
        // Nível baixo: limpando interrupção no IRQSTATUS_0
        HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_0) |= (1 << pino);
    } else {
        // Nível alto: limpando interrupção no IRQSTATUS_1 (não usado aqui)
        HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_1) |= (1 << pino);
    }
    return 1;
}

// Configura pino para interrupção por GPIO com rising edge (por padrão)
void pinInterrup(int irqSet, gpiomod mod, int pino) {
    switch (mod) {
        case GPIO1:
            if (irqSet == 0) {
                // Habilita interrupção e rising edge
                HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_SET_0) |= (1 << pino);
                HWREG(SOC_GPIO_1_REGS + GPIO_RISINGDETECT) |= (1 << pino);
                HWREG(SOC_GPIO_1_REGS + GPIO_FALLINGDETECT) &= ~(1 << pino); // começa só com subida
            } else {
                HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_SET_1) |= (1 << pino);
                HWREG(SOC_GPIO_1_REGS + GPIO_RISINGDETECT) |= (1 << pino);
                HWREG(SOC_GPIO_1_REGS + GPIO_FALLINGDETECT) &= ~(1 << pino);
            }
            break;

        // Outros GPIOs (GPIO0, GPIO2, GPIO3) não usados neste projeto
        default:
            break;
    }
}

// Ativa interrupção global correspondente no controlador INTC
int Interrupt_Setup(unsigned int inter) {
    if (inter > 127) return 0;

    unsigned int reg_offset = inter >> 5;
    unsigned int bit = inter % 32;

    switch (reg_offset) {
        case 0:
            HWREG(INTC_BASE + MIR_CLEAR0) |= (1 << bit);
            break;
        case 1:
            HWREG(INTC_BASE + MIR_CLEAR1) |= (1 << bit);
            break;
        case 2:
            HWREG(INTC_BASE + MIR_CLEAR2) |= (1 << bit);
            break;
        case 3:
            HWREG(INTC_BASE + MIR_CLEAR3) |= (1 << bit);
            break;
        default:
            return 0;
    }
    return 1;
}
