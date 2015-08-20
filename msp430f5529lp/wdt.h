#include "stdint.h"
#include "printString.h"
#include "board.h"
#include "button.h"

extern volatile uint16_t wdt_counter;

void initWDT(uint16_t);
