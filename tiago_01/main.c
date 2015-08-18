#include <stdio.h>
#include <stdint.h>

#include "board.h"

int main(void) {
  int myInt = 0;
  board_init();

	while(1) {
    for (int ii=0; ii<40; ii++ ){
      for (int jj=0; jj<100; jj++ ){
        __delay_cycles(1000);
      }
    }
    print("Hello, World %d\n\r", myInt++);
  }
}
