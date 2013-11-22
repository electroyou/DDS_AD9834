// Author: Leonardo
// Firmware for DDS 
#include <asf.h>
#include "ad9834.h"
#include "usb_data.h"

// MCU Commands
#define CMD_FREQ	0

int main(void)
{ 	
	ioport_init();
	board_init();
	sysclk_init();		
	
	irq_initialize_vectors();
	cpu_irq_enable();
	
	stdio_usb_init();
		
	// Power on LED on board
	ioport_set_pin_level(GPIO_LED_RED, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(GPIO_LED_GREEN, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(GPIO_LED_YELLOW, IOPORT_PIN_LEVEL_HIGH);
	
	delay_ms(200);
	
	ad9834_init();		
	
	// Constant AM Modulation
	//ad9834_set_output_voltage(CONF_AD9834_MAX_VOUT / 4);
		
	uint32_t frequency = 100;
	ad9834_configure(frequency, 0, 0, 0);	// Initial frequency
		
    while(true)
    {        	
		// Read command from PC
		Byte cmd = usb_data_read_byte();
		switch (cmd)
		{
			case CMD_FREQ:	
				frequency = usb_data_read_uint16();
				ad9834_configure(frequency, 0, 0, 0);
				break;
			default:			
				break;
		}						
		delay_ms(10);
    }
}