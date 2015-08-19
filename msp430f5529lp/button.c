#include "button.h"
void setupButton(void)
{
	P1DIR &= ~BIT1;					// Set P1.1 to input direction Button
  P1REN |= BIT1;          // with pullup resistor
  P1OUT |= BIT1;          // with pullup resistor
	P2DIR &= ~BIT1;					// Set P2.1 to input direction Button
  P2REN |= BIT1;          // with pullup resistor
  P2OUT |= BIT1;          // with pullup resistor
	P4DIR &= ~BIT1;					// Set P2.1 to input direction Button
  P4REN |= BIT1;          // with pullup resistor
  P4OUT |= BIT1;          // with pullup resistor
}

int b1pressed(uint16_t* _pstate){
  if(!(P1IN & BIT1)){
    if((*_pstate) < BUTTONACTION){
      (*_pstate)++;
      return 0;
    }
    return 1;
  }
  (*_pstate) = 0;
  return 0;
}
int b2pressed(uint16_t* _pstate){
  if(!(P2IN & BIT1)){
    if((*_pstate) < BUTTONACTION){
      (*_pstate)++;
      return 0;
    }
    return 1;
  }
  (*_pstate) = 0;
  return 0;
}
int b3pressed(uint16_t* _pstate){
  if(!(P4IN & BIT1)){
    if((*_pstate) < BUTTONACTION){
      (*_pstate)++;
      return 0;
    }
    return 1;
  }
  (*_pstate) = 0;
  return 0;
}
