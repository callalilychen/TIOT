
#ifndef __BOARD_EXT_H__
#define __BOARD_EXT_H__



#include "board.h"

void ports_init(void);

inline void __attribute__((always_inline))signalHigh(void){
  P2OUT |= BIT3;
  turnLedOn(LED1);
}

inline void __attribute__((always_inline))signalLow(void){
  P2OUT &= ~BIT3;
  turnLedOff(LED1);
}

#endif /* __BOARD_EXT_H__ */
