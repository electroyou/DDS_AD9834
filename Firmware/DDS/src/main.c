// Author: Leonardo
// Firmware for DDS 
#include <asf.h>
#include "ad9834.h"

int main(void)
{ 	
	ioport_init();
	board_init();
	sysclk_init();		
	
	// Power on LED on board
	ioport_set_pin_level(GPIO_LED_RED, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(GPIO_LED_GREEN, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(GPIO_LED_YELLOW, IOPORT_PIN_LEVEL_HIGH);
	
	delay_ms(500);		
	
	ad9834_init();
		
	// Constant AM Modulation
	//ad9834_set_output_voltage(CONF_AD9834_MAX_VOUT / 4);
		
	uint32_t frequency = 100;
	
	// Frequency sweep
    while(true)
    {        		
		ad9834_configure(frequency, 0, 0, 0);
		frequency += 100;
		if(frequency>=20000000)
		{
			frequency = 100;
		}
		delay_ms(10);
    }
}