#include "uart.h"

int uart_init(void){
  P4SEL = BIT4 + BIT5;                      // P4.4 = TXD, P4.5 = RXD
  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL1 |= UCSSEL_2;                     // SMCLK fBRCLK = fSMCLK 
  UCA1BR0 = BAUD_RATE_MAJOR;                // 1MHz 115200 (see User's Guide)
  UCA1BR1 = BAUD_RATE_MINOR;                // 1MHz 115200
  UCA1MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  // TODO currently no need for rx intererrupt 
  // UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt

  /* configuration done, release reset bit => start UART */
  UCA1CTL1 &= ~UCSWRST;

 #if defined(DEBUG) || defined(INFO) || defined(LOG)
  setHandler(&uart_putByte);
#endif
}

int uart_putByte(char c)
{
//TODO RIOT unsigned sr = disableIRQ();

  /* the LF endline character needs to be "doubled" into CR+LF */
  if (c == '\n') {
      putchar('\r');
  }
  /* wait for a previous transmission to end */
  while ((UCA1IFG & UCTXIFG) == 0) {
      __asm__("nop");
  }
  /* load TX byte buffer */
  UCA1TXBUF = (char) c;

//  restoreIRQ(sr);
  return c;
}

//int uart_putString(const char *_ptr)
//{
//  unsigned int i, len;
//
//  len = strlen(_ptr);
//
//  while(*_ptr!='\0'){
//    uart_putByte(*_ptr);
//    _ptr++;
//  }
//  return len;
//}
