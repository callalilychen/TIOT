#include "board_ext.h"

/*!
 * \brief Set all pins as output pin
 *
 * Make sure there are no floating input I/Os.
 * 
 */
void ports_init(void){
  P1SEL = 0;
  P1DIR = 0xFF;
  P1OUT = 0;    /* Port1 Output: 00000000 = 0x00 */
  P2SEL = 0;
  P2DIR = 0xFF;
  P2OUT = 0;    /* Port2 Output: 00000000 = 0x00 */
  P3SEL = 0;
  P3DIR = 0xFF;
  P3OUT = 0;    /* Port3 Output: 00000000 = 0x00 */
  P4SEL = 0;
  P4DIR = 0xFF;
  P4OUT = 0;    /* Port4 Output: 00000000 = 0x00 */
  P5SEL = 0;
  P5DIR = 0xFF;
  P5OUT = 0;    /* Port5 Output: 00000000 = 0x00 */
  P6SEL = 0;
  P6DIR = 0xFF;
  P6OUT = 0;    /* Port6 Output: 00000000 = 0x00 */
  P7SEL = 0;
  P7DIR = 0xFF;
  P7OUT = 0;    /* Port7 Output: 00000000 = 0x00 */
  P8SEL = 0;
  P8DIR = 0xFF;
  P8OUT = 0;    /* Port8 Output: 00000000 = 0x00 */
}
