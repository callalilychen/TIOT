#ifndef UART_H
#define UART_H
#include <msp430.h>				
#include <string.h>
#include "board.h"

#define BAUDRATE    (115200ul)
#define BAUD_RATE_MAJOR   (unsigned int)(F_SMCLK / BAUDRATE)
#define BAUD_RATE_MINOR   (unsigned int)((F_SMCLK / BAUDRATE) / 256)

int uart_init(void);
int uart_putByte(char);
int uart_putString(const char *);
#endif

