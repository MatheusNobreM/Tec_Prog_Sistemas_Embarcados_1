#include <stdint.h>
#include "watchdog.h"

#define WDT1_BASE 0x44E35000u
#define WDT_WSPR   (*(volatile uint32_t *)(WDT1_BASE + 0x48))
#define WDT_WWPS   (*(volatile uint32_t *)(WDT1_BASE + 0x34))

void watchdog_disable(void) {
    WDT_WSPR = 0xAAAA;
    while (WDT_WWPS & (1 << 4));
    WDT_WSPR = 0x5555;
    while (WDT_WWPS & (1 << 4));
}
