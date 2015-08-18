#include <stdio.h>
#include <stdint.h>
#include <msp430f5529/board.h>
#include <msp430f5529/cli_uart.h>

int main(void) {
  int myInt = 0;
 
  initClk();
  CLI_Configure();

	while(1) {
    for (int ii=0; ii<40; ii++ ){
      for (int jj=0; jj<100; jj++ ){
        __delay_cycles(1000);
      }
    }
    if (0== (myInt++%2) ) {
      CLI_Write("Hello, World\r\n");
    } else {
      CLI_Write("Goodbye, Cruel World....\r\n");
    }
  }
}
