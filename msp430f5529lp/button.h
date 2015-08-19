#ifndef BUTTON_H
#define BUTTON_H
#include "msp430.h"
#include "stdint.h"
#define BUTTONACTION 10000

void setupButton(void);
int b1pressed(uint16_t*);
int b2pressed(uint16_t*);
int b3pressed(uint16_t*);
#endif
