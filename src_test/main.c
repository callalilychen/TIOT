#include <stdio.h>
#include <stdint.h>

#include "board.h"


#define TEST

#ifdef TEST
#include "testcases.h"
#endif


int main(void) {
  board_init();

#ifdef LED
  setupLED();
  startSignal();

  led1on();
  __delay_cycles(500000);
  led1off();
  __delay_cycles(500000);
  led4on();
  __delay_cycles(500000);
  led4off();
  __delay_cycles(500000);
#endif

  setup();

#ifdef TEST
#ifdef TESTLPM
  testLPM(LPM3_bits + GIE);
#else
	while(1){
#ifdef TESTLMP
   __bis_SR_register(LPM3_bits + GIE); // Enter LPM3
#else
    tests();
#endif
	}
#endif
#endif
	
	return 0;
}
