#include "lpm.h"
#include "treeconfig.h"

static uint16_t rs_value = LPM3_bits + GIE;
void setupLPM(uint16_t v)
{
  //TA0CTL = TASSEL_2 | ID_3 | MC_1 | TACLR | TAIE | TAIFG; // Set the timer A to SMCLK, /8, up mode
  TA0CTL = TASSEL_1 | ID_3 | MC_1 | TACLR; // Set the timer A to ACLK, /8, up mode
  //TA0CTL |= TAIE; // Enable TA interrupt, which interrupt by timer == 0
  TA0CCR0 = 41*5; // 10ms *5
  //TA0CCR0 = 0x1000; //1s
  uint16_t timer = TA0R;

  //TA0CCTL0 = CAP;
  
  rs_value = v;
  DEBUG("Begin testLPM: %x\n\r", TA0CCR0);
  TA0CCTL0 |= CCIE; // Enable CC interrupt
  _EINT ();
}

void enterLPM(){
  //TA0CTL |= TACLR;
  //TA0CCTL0 |= CCIE; // Enable CC interrupt
  LPM3;
}

// Timer A0 CC0 interrupt service routine
__attribute__((interrupt(TIMER0_A0_VECTOR)))
void isr_TA00 (void)
{
  //TA0CCTL0 |= CCIE; // Enable CC interrupt
   LPM3_EXIT;
   // signalToggle();
  //TA0CTL |= TACLR;
  //TA0CCTL0 &= ~CCIE; // Disable interrupt
  //TA0CTL &= ~ TAIE;
  //DEBUG("INTER Timer: 0x%x\n\r", TA0R);
  //TA0CTL |= TACLR;
}

// Timer A0 TA interrupt service routine
__attribute__((interrupt(TIMER0_A1_VECTOR)))
void isr_TA01 (void)
{
  if(TA0IV  == TA0IV_TAIFG){
    signalToggle();
    //TA0IV &= ~TA0IV_TAIFG;
    TA0CTL &=~TAIFG;
  }
  //LPM3_EXIT;
  //TA0CTL |= TACLR;
  //TA0CCTL0 &= ~CCIE; // Disable interrupt
  //TA0CTL &= ~ TAIE;
  //DEBUG("INTER Timer: 0x%x\n\r", TA0R);
  //TA0CTL |= TACLR;
}
