// Firmware for DDS 
#include <asf.h>
#include "ad9834.h"

int main(void)
{ 	
	ioport_init();
	board_init();
	sysclk_init();
	
	ad9834_init();		
	ad9834_set_output_voltage(CONF_AD9834_MAX_VOUT / 4);
	
	// Power on LED on board
	ioport_set_pin_level(GPIO_LED_RED, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(GPIO_LED_GREEN, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(GPIO_LED_YELLOW, IOPORT_PIN_LEVEL_HIGH);
	
	uint32_t frequency = 1000;
	
    while(true)
    {        
		ad9834_configure(frequency, 0, 0, 0);
		delay_ms(200);		
		frequency += 100;
    }
}