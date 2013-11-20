/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

// GPIO Connections of LEDs
#define GPIO_LED_GREEN		IOPORT_CREATE_PIN(PORTB,1)	// Green
#define GPIO_LED_YELLOW		IOPORT_CREATE_PIN(PORTB,2)	// Yellow
#define GPIO_LED_RED		IOPORT_CREATE_PIN(PORTB,3)	// Red

// GPIO Connections of Push Buttons
#define GPIO_PUSH_BUTTON_0	IOPORT_CREATE_PIN(PORTC,3)

// Others GPIO Connections
#define GPIO_VBUS			IOPORT_CREATE_PIN(PORTA,7)	// VBUS detection voltage divider for self-powered device

// External oscillator settings.
#define BOARD_XOSC_HZ          12000000				// abm3cd2x http://www.abracon.com/Resonators/abm3c.pdf
#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL
#define BOARD_XOSC_STARTUP_US  1000

#endif // USER_BOARD_H