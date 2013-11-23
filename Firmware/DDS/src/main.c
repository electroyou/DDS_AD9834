// Author: Leonardo
// Firmware for DDS 
#include <asf.h>
#include "ad9834.h"
#include "usb_data.h"

// Not definitives code, rely only on #define name, not value

// MCU Commands received from PC
#define CMD_FREQ		0
//#define CMD_FREQ_REG	1
//#define CMD_PHASE		2
//#define CMD_PHASE_REG	3
#define CMD_VOUT		4
//#define CMD_WAVEFORM	5
//#define CMD_SWEEP		6
//#define CMD_SLEEP		254
//#define CMD_RESET		255

// MCU Messages that send to PC to communicate status
#define MSG_DONE		100

int main(void)
{ 	
	ioport_init();
	board_init();
	sysclk_init();			
	irq_initialize_vectors();
	cpu_irq_enable();	
	stdio_usb_init();
			
	delay_ms(200);
	
	// Power on LED on board
	ioport_set_pin_level(GPIO_LED_GREEN, IOPORT_PIN_LEVEL_HIGH);		
	
	ad9834_init();	
	ad9834_configure(1000.0, 0, 0, 0);	
			
    while(true)
    {   
		float frequency, start_frequency, end_frequency, vout;
		uint32_t delay;
		     			
		// Read command from PC
		Byte cmd = usb_data_read_byte();		 				
		switch (cmd)
		{
			case CMD_FREQ:	
				frequency = usb_data_read_float();
				ad9834_configure(frequency, 0, 0, 0);
				break;
			case CMD_VOUT:
				vout = usb_data_read_float();												
				ad9834_set_output_voltage(vout);
				break;
			/*	Not implemented yet
			case CMD_FREQ_REG:
				// ad9834_set_frequency_register(usb_data_read_byte())
				break;
			case CMD_PHASE:
				// ad9834_set_phase()
				break;
			case CMD_PHASE_REG:
				// ad9834_set_phase_register(usb_data_read_byte())
				break;
			case CMD_WAVEFORM:
				// See ad9834_waveform enum				
				// ad9834_set_waveform(usb_data_read_byte());
				break;
			case CMD_SWEEP:
				start_frequency = usb_data_read_float();
				end_frequency = usb_data_read_float();
				delay = usb_data_read_uint32();
				for (uint32_t frequency = start_frequency; frequency < end_frequency; frequency += 1)
				{
					ad9834_set_frequency(frequency);
					delay_us(delay);
				}
				usb_data_write_byte(MSG_DONE);
				break;
			case CMD_RESET:
				break;
			*/
			default:			
				// Do nothing if not recognized command
				break;
		}						
    }
}