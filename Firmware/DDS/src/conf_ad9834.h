// Author: Leonardo
#ifndef CONF_AD9834_H_
#define CONF_AD9834_H_

#define CONF_AD9834_MCLK					75.0f	// AD9834 frequency in MHz

// SPI pinout XMega A4U
// x = PORTC or PORTD (SPIC or SPID)
// Px4 SS
// Px5 MOSI
// Px6 MISO
// Px7 SCK
#define CONF_AD9834_SPI_PORT_CLOCK_SPEED	8000000UL
#define CONF_AD9834_SPI_PORT				&SPIC

#define CONF_AD9834_MOSI_PIN				IOPORT_CREATE_PIN(PORTC,5)
#define CONF_AD9834_MISO_PIN				IOPORT_CREATE_PIN(PORTC,6)
#define CONF_AD9834_SCK_PIN					IOPORT_CREATE_PIN(PORTC,7)
#define CONF_AD9834_FSYN_PIN				IOPORT_CREATE_PIN(PORTC,1)	// Frame synchronization signal

// PROBLEM! On Dev-Board (http://www.embeddedwirelesssolutions.com/schematics/ews_atxmega32a4u_breakout_board.png) 
// PB1, PB2, PB3 are tied to led :(

// DAC pinout XMega A4U
// PA0 AREF B
// PB0 AREF A
// PB2 DAC0
// PB3 DAC1
#define CONF_AD9834_DAC				DACB			// FS_ADJ port B
#define CONF_AD9834_DAC_CHANNEL		DAC_CH0			// FS_ADJ pin PB2
#define CONF_AD9834_DAC_REF			DAC_REF_AREFB	// PB0 - External reference on AREF pin on port B connected to 1.2v Ref Out of DDS
#define CONF_AD9834_MAX_VOUT		0.6				// Voltage Output after amplification (0.6v with no amplification)

#endif