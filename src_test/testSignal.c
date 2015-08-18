#include "testcases.h"

inline void setupSignal(void){
  P2DIR |= BIT0;					// Set P2.0 to output direction
  P2DIR |= BIT2;					// Set P2.2 to output direction
  P2OUT &= ~(BIT0 + BIT2);

}
inline void startSignal(void){
  P2OUT |= (BIT0 + BIT2);
  __delay_cycles(1000);
  P2OUT &= ~(BIT0 + BIT2);
  __delay_cycles(1000);
  P2OUT |= (BIT0 + BIT2);
  __delay_cycles(1000);
  P2OUT &= ~(BIT0 + BIT2);
#ifdef DEBUG
  print("Start\n\r");
#endif
}
inline void testSignalHigh(void){
  P2OUT |= (BIT0 + BIT2);
}

inline void testSignalLow(void){
  P2OUT &= ~(BIT0 + BIT2);
}
