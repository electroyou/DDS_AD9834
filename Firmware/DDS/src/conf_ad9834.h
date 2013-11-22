// Author: Leonardo

#ifndef CONF_AD9834_H_
#define CONF_AD9834_H_

#define CONF_AD9834_SPI_PORT_CLOCK_SPEED	8000000UL
#define CONF_AD9834_SPI_PORT				&SPIC
#define CONF_AD9834_MCLK					75

// TODO: Define pin number
#define CONF_AD9834_SS_PIN		IOPORT_CREATE_PIN(PORTB,0)	// Slave-select
#define CONF_AD9834_FSYN_PIN	IOPORT_CREATE_PIN(PORTB,1)

#define CONF_AD9834_DAC				DACB			// FS_ADJ port
#define CONF_AD9834_DAC_CHANNEL		DAC_CH0			// FS_ADJ pin
#define CONF_AD9834_DAC_REF			DAC_REF_AREFB	// External reference on AREF pin on port B
#define CONF_AD9834_MAX_VOUT		0.6				// Voltage Output after amplification (0.6v with no amplification)

#endif