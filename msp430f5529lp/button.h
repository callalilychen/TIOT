#ifndef BUTTON_H
#define BUTTON_H
#include <msp430.h>
#include <stdint.h>
#include "wdt.h"
#include "cli_uart.h"
#include "mutex.h"
#include "printString.h"

#define BUTTONACTION 10000
struct btstate{
  uint8_t pin;
  uint8_t pressed;
  uint16_t press;
  uint8_t released;
  uint16_t release;
  uint8_t expired;
  uint8_t type;
};
extern struct btstate b1state;
extern struct btstate b2state;
void setupButton(uint16_t,uint16_t);
int updateState();
int b1press();
int b2press();
int b1pressed();
int b2pressed();
int b1release();
int b2release();
#endif
