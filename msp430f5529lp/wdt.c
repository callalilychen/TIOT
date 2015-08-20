#include "wdt.h"
volatile uint16_t wdt_counter = 0;

void initWDT(uint16_t init)
{
  WDTCTL = init;
  initLEDs();
  SFRIE1 |= WDTIE;
  __bis_SR_register(GIE);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(WDT_VECTOR)))
#endif
void WDT_ISR(void)
{
  wdt_counter ++;
}
