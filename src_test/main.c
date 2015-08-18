#include <stdio.h>
#include <stdint.h>

#include "board.h"


#define TEST

#ifdef TEST
#include "testcases.h"
#endif

#ifdef LED
void testleds(uint16_t *_pstate){
  if(testLED1(*_pstate) | testLED4(*_pstate)){
    *_pstate = 0;
  } else{
    (*_pstate)++;
  }

}
#endif

int main(void) {
  board_init();

  setup();
//#ifndef TESTLED
//  #define LED
//  setupLED();
//#endif
#ifdef LED
  led1on();
  __delay_cycles(500000);
  led1off();
  led4on();
  P2OUT &= ~BIT0;
  __delay_cycles(500000);
  led4off();
#endif

#ifdef TEST
  int counter = 1;
	while(1){
#ifdef SIGNAL
  startSignal();
#endif
#ifdef SHA256
    testSHA256(counter);
#endif

#ifdef AES
    testAES(counter);
#endif
#ifdef DES
    test3DES(counter);
#endif

#ifdef ECC
    testECC(counter);
#endif


#ifdef BUTTON
  testButton1();
  testButton2();
#endif 
#ifdef TESTLED
  testleds(&state);
#endif
  counter++;
	}
#endif
	
	return 0;
}
