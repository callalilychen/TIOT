
#ifndef __BOARD_EXT_H__
#define __BOARD_EXT_H__



#include "board.h"

void ports_init(void);

inline void __attribute__((always_inline))signalHigh(void){
  P8OUT |= BIT1;
  turnLedOn(LED1);
}

inline void __attribute__((always_inline))signalLow(void){
  P8OUT &= ~BIT1;
  turnLedOff(LED1);
}

inline void __attribute__((always_inline))signalToggle(void){
  P8OUT ^= BIT1;
  toggleLed(LED1);
}
#endif /* __BOARD_EXT_H__ */
