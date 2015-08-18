#include "testcases.h"

void setupLPM()
{
  TA0CCTL0 |= CCIE;
  TA0CTL = TASSEL_2 + MC_2 + TACLR; // Set the timer A to SMCLCK, Continuous
  // Clear the timer and enable timer interrupt
    uint16_t timer = TA0R;
  __delay_cycles(10000000);

  __bis_SR_register(GIE); // interrupts enabled
}

void testLPM()
{
  
 __bis_SR_register(LPM0_bits + GIE); // Enter LPM0, interrupts enabled
}
// Timer A0 interrupt service routine
__attribute__((__interrupt__(TIMERA0_VECTOR)))
static void  isr_TA0 (void)
{
  uint16_t timer = TA0R;
  print("%x|", timer);
}
