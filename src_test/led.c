#include "led.h"
#include "testcases.h"
void setupLED(void)
{
	P1DIR |= BIT0;					// Set P1.0 to output direction LED
	P4DIR |= BIT7;					// Set P4.7 to output direction LED
 
  // All leds off
  P1OUT &= ~BIT0;
  P4OUT &= ~BIT7;
}
void led1on()
{
  P1OUT |= BIT0;
}
void led1off()
{
  P1OUT &= ~BIT0;
}
void led1switch()
{
  P1OUT ^= BIT0;
}
void led4on()
{
  P4OUT |= BIT7;
}
void led4off()
{
  P4OUT &= ~BIT7;
}
void led4switch()
{
  P4OUT ^= BIT7;
}
int testLED1(uint16_t _state)
{
  if(_state >= LEDDELAY){
    led1switch();
    return 1;
  }
  return 0;
}
int testLED4(uint16_t _state)
{
  if(_state >= LEDDELAY){
    led4switch();
    return 1;
  }
  return 0;
}
