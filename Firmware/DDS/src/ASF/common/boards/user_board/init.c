/**
 * \file
 *
 * \brief User board initialization template
 *
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */	
	ioport_set_pin_dir(GPIO_LED_RED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(GPIO_LED_GREEN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(GPIO_LED_YELLOW, IOPORT_DIR_OUTPUT);
	
	ioport_set_pin_dir(GPIO_VBUS, IOPORT_DIR_INPUT);
	
	ioport_set_pin_dir(GPIO_PUSH_BUTTON_0, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(GPIO_PUSH_BUTTON_0, IOPORT_MODE_PULLUP);
}