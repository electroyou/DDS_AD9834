// Firmware for DDS 
#include <asf.h>

#define CONF_AD9834_SPI_PORT_CLOCK_SPEED	8000000UL
#define CONF_AD9834_SPI_PORT				&SPIC
#define CONF_AD9834_MCLK					75

// TODO: Define pin number
#define CONF_AD9834_SS_PIN		IOPORT_CREATE_PIN(PORTB,0)	// Slave-select
#define CONF_AD9834_FSYN_PIN	IOPORT_CREATE_PIN(PORTB,1)

struct spi_device spi_device_conf = {
	.id = CONF_AD9834_SS_PIN, // 0
};

// Send a 16bit word to DDS and use fsync
void WriteDDS (uint16_t data)
{		
	uint8_t MSdata = ((data>>8) & 0x00FF);  //filter out MS
	uint8_t LSdata = (data & 0x00FF);	//filter out LS
	spi_select_device(CONF_AD9834_SPI_PORT, &spi_device_conf);
	ioport_set_pin_high(CONF_AD9834_FSYN_PIN);					// Fsync Low --> begin frame			
	spi_write_single(CONF_AD9834_SPI_PORT, MSdata);
	while (!spi_is_rx_full(CONF_AD9834_SPI_PORT)) {}								// Wait packet is sent
	spi_write_single(CONF_AD9834_SPI_PORT, LSdata);
	ioport_set_pin_low(CONF_AD9834_FSYN_PIN);					// Fsync High --> End of frame
	spi_deselect_device(CONF_AD9834_SPI_PORT, &spi_device_conf);
}
// Change the frequency and select AD9833 on-board register
// selectReg = register 0 or 1, triangMode = 0 sine, 1 triangle
// uses 14 bit filter and adds control words
void Freq_change (uint32_t frequency, uint8_t selectReg, Bool triangMode, Bool squareMode)
{
	// take base10 frequency and do frequency hop
	uint32_t freq_reg = frequency * (268.435456 / CONF_AD9834_MCLK);	// make freq register from frequency		
	uint16_t MS_reg = ((freq_reg >> 14) & 0x3FFF);			// filter out MS -- make 2 x 14 bit frequency words
	uint16_t LS_reg = (freq_reg & 0x3FFF);					// filter out LS -- make 2 x 14 bit frequency words
	MS_reg += 0x4000; 			// add control bits hex = 0x4000
	LS_reg += 0x4000; 			// add control bits hex = 0x4000
	uint16_t ctrl_reg = 0x0000;	// Sine output
	if(triangMode == true)
	{
		ctrl_reg = 0x0002;	// Triangle output
	}
	if(squareMode == true)
	{
		ctrl_reg += 0x0028;	// Square output (on pin SIGN OUT)
	}
	if(selectReg == 0 ) {	// receive full 28bit word for freq 0
		ctrl_reg += 0x2000;
	}
	if(selectReg == 1 ) {	// receive full 28bit word for freq 1
		ctrl_reg += 0x2800;
	}
	WriteDDS(ctrl_reg); // prepare ad9833 control register
	WriteDDS(LS_reg);	// send the LS word first, to the ad9833
	WriteDDS(MS_reg);	// send the MS word last,  to the ad9833
}
int main(void)
{	
	ioport_init();
	board_init();
	
	ioport_set_pin_dir(CONF_AD9834_SS_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(CONF_AD9834_FSYN_PIN, IOPORT_DIR_OUTPUT);		

	spi_flags_t spi_flags = SPI_MODE_2;	// SPI mode required by AD9834
		  
	board_spi_select_id_t spi_select_id = 0;	
	spi_master_init(CONF_AD9834_SPI_PORT);
	spi_master_setup_device(CONF_AD9834_SPI_PORT, &spi_device_conf, spi_flags, CONF_AD9834_SPI_PORT_CLOCK_SPEED, spi_select_id);
	spi_enable(CONF_AD9834_SPI_PORT);
	
	/* Send one dummy byte for the spi_is_tx_ok() to work as expected */
	spi_write_single(CONF_AD9834_SPI_PORT, 0);
	
	// Power on LED on board
	ioport_set_pin_level(GPIO_LED_RED, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(GPIO_LED_GREEN, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(GPIO_LED_YELLOW, IOPORT_PIN_LEVEL_HIGH);

    while(true)
    {
        //TODO:: Please write your application code 
    }
}