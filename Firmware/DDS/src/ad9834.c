// Author: Leonardo
#include "ad9834.h"

struct spi_device spi_device_conf = {
	.id = CONF_AD9834_SS_PIN, // 0
};

// Send a 16bit word to DDS and use fsync
void ad9834_write_register(uint16_t data)
{
	uint8_t MSdata = ((data>>8) & 0x00FF);  //filter out MS
	uint8_t LSdata = (data & 0x00FF);		//filter out LS
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
void ad9834_configure(uint32_t frequency, uint8_t selectReg, Bool triangMode, Bool squareMode)
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
	ad9834_write_register(ctrl_reg);	// prepare ad9833 control register
	ad9834_write_register(LS_reg);		// send the LS word first, to the ad9833
	ad9834_write_register(MS_reg);		// send the MS word last,  to the ad9833
}

// FS ADJUST = Full-Scale Adjust Control
// Adjust output voltage from 0 to MAX_VOUT
void ad9834_set_output_voltage(float vout)
{
	// Check if required vout is in admissible range
	Assert((vout < 0) || (vout > CONF_AD9834_MAX_VOUT));
	
	// Assuming RSET=6.8K, VREF=1.2V math behind:
	// i=18*(1.2-VDAC)/6.8k from datasheet so VDAC=(3400/9)*i+6/5
	// max i = 0.003, max VOUT = MAX_VOUT
	// MAX_VOUT:0.003 = VOUT:i => VOUT*0.003/MAX_VOUT=i
	
	// Calculate output current from DDS
	float i = vout * 0.003 / CONF_AD9834_MAX_VOUT;
	
	// calculate VDAC to obtain current
	float vdac = (3400/9)*i+(6/5);
	
	// Convert VDAC into 12bit DAC code
	// MAX_VOUT : 2^12 = VDAC : DAC_VALUE
	uint16_t dac_value = vdac * 4096 / CONF_AD9834_MAX_VOUT;
	
	dac_wait_for_channel_ready(&CONF_AD9834_DAC, CONF_AD9834_DAC_CHANNEL);
	dac_set_channel_value(&CONF_AD9834_DAC, CONF_AD9834_DAC_CHANNEL, dac_value);
}

void dac_init(void)
{
	struct dac_config conf;
	dac_read_configuration(&CONF_AD9834_DAC, &conf);
	dac_set_conversion_parameters(&conf, CONF_AD9834_DAC_REF, DAC_ADJ_LEFT);
	dac_set_active_channel(&conf, CONF_AD9834_DAC_CHANNEL, 0);					// TODO: Check if is initial value
	//dac_set_conversion_trigger(&conf, CONF_AD9834_DAC_CHANNEL, 3);
	#ifdef XMEGA_DAC_VERSION_1
	dac_set_conversion_interval(&conf, 1); // 1uS between conversion
	#endif
	dac_write_configuration(&CONF_AD9834_DAC, &conf);
	dac_enable(&CONF_AD9834_DAC);
}

void spi_init(void)
{
	ioport_set_pin_dir(CONF_AD9834_SS_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(CONF_AD9834_FSYN_PIN, IOPORT_DIR_OUTPUT);

	spi_flags_t spi_flags = SPI_MODE_2;	// SPI mode required by AD9834
	
	board_spi_select_id_t spi_select_id = 0;
	spi_master_init(CONF_AD9834_SPI_PORT);
	spi_master_setup_device(CONF_AD9834_SPI_PORT, &spi_device_conf, spi_flags, CONF_AD9834_SPI_PORT_CLOCK_SPEED, spi_select_id);
	spi_enable(CONF_AD9834_SPI_PORT);
	
	/* Send one dummy byte for the spi_is_tx_ok() to work as expected */
	spi_write_single(CONF_AD9834_SPI_PORT, 0);
}

void ad9834_init(void)
{
	spi_init();
	dac_init();	
}