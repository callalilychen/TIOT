/*
 * board.h - Zolertia Z1 Board.
 * Copyright (C) 2014 INRIA
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef _MSP430F5529_BOARD_H
#define _MSP430F5529_BOARD_H

/**
 * @defgroup    boards_z1 Zolertia Z1
 * @ingroup     boards
 * @brief       Support for the Zolertia Z1 board.
 *
<h2>Components</h2>
\li MSP430F2617
\li CC2420

* @{
*/

/**
 * @file
 * @brief       Zolertia Z1 board configuration
 *
 * @author      Kévin Roussel <Kevin.Roussel@inria.fr>
 *
 */

#include <stdint.h>
#include <msp430.h>

#include "config.h"
#include "uart.h"
#include "printString.h"
//#define INFO
//#define DEBUG
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __MSP430F5529__
#define __MSP430F5529__
#endif

// On board crystals frequencies (in Hz)
#define XT1_FREQ 32768
#define XT2_FREQ 4000000
#define XT1_KHZ XT1_FREQ/1000
#define XT2_KHZ XT2_FREQ/1000

#define SOURCE_XT2
// MSP430 core
#ifdef SOURCE_XT2
#define F_DCOCLKDIV   (1000000uL)
#define F_SMCLK   (F_DCOCLKDIV*8)
#define F_MCLK  (F_DCOCLKDIV*16)
#elif SOURCE_LFXT
#define F_DCOCLKDIV   (1000000uL)
#define F_SMCLK   (F_DCOCLKDIV*8)
#define F_MCLK  (F_DCOCLKDIV*16)
#endif

#define MSP430_INITIAL_CPU_SPEED   (F_MCLK)
#ifndef F_CPU
#define F_CPU                       MSP430_INITIAL_CPU_SPEED
#endif
#define F_RC_OSCILLATOR             32768 // default ACLK, Auxillary Clock = LFXT1CLK
#define MSP430_HAS_DCOR             0 // FIXME Not sure 
#define MSP430_HAS_EXTERNAL_CRYSTAL 1

// LEDs ports
#define LEDS_PxDIR_RED      P1DIR
#define LEDS_PxDIR_BLUE     P2DIR
#define LEDS_PxDIR_GREEN    P4DIR 
#define LEDS_PxOUT_RED      P1OUT
#define LEDS_PxOUT_BLUE     P2OUT
#define LEDS_PxOUT_GREEN    P4OUT 
#define LEDS_CONF_RED      BITO
#define LEDS_CONF_BLUE     BIT0
#define LEDS_CONF_GREEN    BIT7  

#define LED_RED_ON         LEDS_PxOUT_RED &=~LEDS_CONF_RED
#define LED_RED_OFF        LEDS_PxOUT_RED |= LEDS_CONF_RED
#define LED_RED_TOGGLE     LEDS_PxOUT_RED ^= LEDS_CONF_RED

#define LED_GREEN_ON       LEDS_PxOUT_GREEN &=~LEDS_CONF_GREEN
#define LED_GREEN_OFF      LEDS_PxOUT_GREEN |= LEDS_CONF_GREEN
#define LED_GREEN_TOGGLE   LEDS_PxOUT_GREEN ^= LEDS_CONF_GREEN

#define LED_BLUE_ON        LEDS_PxOUT_BLUE &=~LEDS_CONF_BLUE
#define LED_BLUE_OFF       LEDS_PxOUT_BLUE |= LEDS_CONF_BLUE
#define LED_BLUE_TOGGLE    LEDS_PxOUT_BLUE ^= LEDS_CONF_BLUE


// User-button port
#define USER_BTN1_PxIN      P1IN
#define USER_BTN1_MASK      BIT1

#define USER_BTN1_PRESSED   ((USER_BTN1_PxIN & USER_BTN1_MASK) == 0)
#define USER_BTN1_RELEASED  ((USER_BTN1_PxIN & USER_BTN1_MASK) != 0)

#define USER_BTN2_PxIN      P2IN
#define USER_BTN2_MASK      BIT1

#define USER_BTN2_PRESSED   ((USER_BTN2_PxIN & USER_BTN2_MASK) == 0)
#define USER_BTN2_RELEASED  ((USER_BTN2_PxIN & USER_BTN2_MASK) != 0)

typedef uint8_t radio_packet_length_t;

#ifdef __cplusplus
}
#endif

void msp430_init_dco(void);
void board_init(void);

/** @} */
#endif // _MSP430F5529_BOARD_H
