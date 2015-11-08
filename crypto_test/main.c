#include <stdio.h>
#include <stdint.h>
#include "treeconfig.h"
#ifdef MSP
#include "cli_uart.h"
#endif

#define TEST

#ifdef TEST
#include "testcases.h"
#endif


int main(void) {
#ifdef MSP
  ports_init();
  /* Stop WDT and initialize the system-clock of the MCU */
  stopWDT();
  initClk();
  CLI_Configure();
#ifdef LED
  initLEDs();
  turnLedOn(LED1);
  __delay_cycles(500000);
  turnLedOff(LED1);
  __delay_cycles(500000);
  turnLedOn(LED1);
  __delay_cycles(500000);
  turnLedOff(LED1);
  __delay_cycles(500000);
#endif
#endif
//initWDT(WDT_ADLY_250);
  setup();
#ifdef TEST
#ifdef TESTLPM
  setupLPM(LPM3_bits + GIE);
#endif
	while(1){
    tests();
#ifdef TESTLPM
   enterLPM();
#endif
	}
#endif
	
	return 0;
}
