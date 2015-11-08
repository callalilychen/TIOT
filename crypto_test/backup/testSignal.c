#include "testcases.h"

inline void setupSignal(void){
  P2DIR |= BIT3;					// Set P2.3 to output direction
  P8DIR |= BIT1;					// Set P8.1 to output direction
  P2OUT &= ~(BIT0 + BIT2);

}
inline void startSignal(void){
  testSignalHigh();
  __delay_cycles(1000);
  testSignalLow();
  __delay_cycles(1000);
  testSignalHigh();
  __delay_cycles(1000);
  testSignalLow();
}
inline void testSignalHigh(void){
  P2OUT |= BIT3;
  P8OUT |= BIT1;
#ifdef LED
  led1on();
#endif
}

inline void testSignalLow(void){
  P2OUT &= ~BIT3;
  P8OUT &= ~BIT1;
#ifdef LED
  led1off();
#endif
}
