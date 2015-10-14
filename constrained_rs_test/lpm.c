#include "lpm.h"
uint16_t rs_value = LPM3_bits + GIE;
void setupLPM(uint16_t v)
{
  TA0CTL = TASSEL_1 | ID_3 | MC_1 | TACLR; // Set the timer A to ACLK, /8, up mode
  TA0CCR0 = 41*5; // 10ms *5
  //TA0CCR0 = 0x1000; //1s
  uint16_t timer = TA0R;

#ifdef LOG
  print("Begin testLPM: %x\n\r", timer);
  print("Begin testLPM: %x\n\r", TA0CCR0);
#endif


  rs_value = v;

  TA0CTL |= TACLR;
//TA0CCTL0 |= CCIE; // Enable interrupt
  TA0CTL |= TAIE;
  enterLPM();
}

// Timer A0 interrupt service routine
__attribute__((interrupt(TIMER0_A1_VECTOR)))
void isr_TA3 (void)
{
  __bic_SR_register(rs_value);
  //TA0CCTL0 &= ~CCIE; // Disable interrupt
  TA0CTL &= ~ TAIE;

#ifdef LOG 
  uint16_t timer = TA0R;
  print("Timer: %x\n\r", timer);
  print("Compare: %x\n\r", TA0CCR0);
#endif

#ifdef LOG 
  timer = TA0R;
  print("Timer: %x\n\r", timer);
#endif
  TA0CTL = TASSEL_1 | ID_3 | MC_1 | TACLR | TAIE; // Set the timer A to ACLK, /8, up mode
  TA0CCR0 = 41*5; // 10ms * 5
  //TA0CCTL0 |= CCIE; // Enable interrupt
#ifdef LOG 
  timer = TA0R;
  print("Timer: %x\n\r", timer);
#endif
}
