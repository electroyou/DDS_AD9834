// Author: Leonardo
#include "ad9834.h"

struct spi_device spi_device_conf = {
	.id = CONF_AD9834_FSYN_PIN
};

float _frequency = 1000.0;
enum ad9834_waveform _waveform = AD9834_SINE;
uint8_t _selectReg = 0;

// Send a 16bit word to DDS
void ad9834_write_register(uint16_t data)
{			
	uint8_t packed_data[2] = {((data>>8) & 0x00FF), (data & 0x00FF)}; // MS, LS
	spi_write_packet(CONF_AD9834_SPI_PORT, packed_data, 2);			
}

#define AD9834_BIT_RESET 1 << 8
#define AD9834_BIT_FREQ0 1 << 14
#define AD9834_BIT_FREQ1 1 << 15
#define AD9834_CONTROL_REGISTER(B28, HLB, FSEL, PSEL, PIN_SW, RESET, SLEEP1, SLEEP12, OPBITEN, SIGN_PIB, DIV2, MODE) ( 0 << 15 | 0 << 14 | B28 << 13 | HLB << 12 | FSEL << 11 | PSEL << 10 | PIN_SW << 9 | RESET << 8 | SLEEP1 << 7 | SLEEP12 << 6 | OPBITEN << 5 |SIGN_PIB << 4 | DIV2 << 3 | 0 << 2 | MODE << 1 | 0)

// Change the frequency and select AD9833 on-board register
// selectReg = register 0 or 1, triangMode = 0 sine, 1 triangle
// uses 14 bit filter and adds control words
void ad9834_configure(float frequency, uint8_t selectReg, Bool triangMode, Bool squareMode)
{				
	// 2^28 = 268435456, do frequency hop
	uint32_t freq_reg = (frequency * 268.435456f) / CONF_AD9834_MCLK; // frequency * 3.579139413f;	
	
	// Make 2 x 14 bit frequency words (discard 4 LSB bit) 
	uint16_t MS_reg = ((freq_reg >> 14) & 0x3FFF) | AD9834_BIT_FREQ0;
	uint16_t LS_reg = (freq_reg & 0x3FFF) | AD9834_BIT_FREQ0;		
		
	// TODO: Assure FSYNC-to-SCLK falling (5ns) and hold time (10ns min - 20ns max)			
	spi_select_device(CONF_AD9834_SPI_PORT, &spi_device_conf);
	
	ad9834_write_register(AD9834_CONTROL_REGISTER(1,0,selectReg,0,0,0,0,0,squareMode,0,0,triangMode));
	ad9834_write_register(LS_reg);
	ad9834_write_register(MS_reg);
	
	spi_deselect_device(CONF_AD9834_SPI_PORT, &spi_device_conf);
}

void ad9834_update(void)
{
	ad9834_configure(_frequency, _selectReg, _waveform == AD9834_TRIANGLE, _waveform == AD9834_SQUARE);
}

// Adjust output voltage from 0 to CONF_AD9834_MAX_VOUT
void ad9834_set_output_voltage(float vout)
{
	// Check if required vout is in admissible range
	Assert((vout < 0) || (vout > CONF_AD9834_MAX_VOUT));
				
	uint16_t dac_value = 4095 - (4095 * vout / CONF_AD9834_MAX_VOUT);	
	dac_wait_for_channel_ready(&CONF_AD9834_DAC, CONF_AD9834_DAC_CHANNEL);
	dac_set_channel_value(&CONF_AD9834_DAC, CONF_AD9834_DAC_CHANNEL, dac_value);
}

void ad9834_set_waveform(enum ad9834_waveform waveform)
{
	_waveform = waveform;
	ad9834_update();
}

void ad9834_set_frequency(uint16_t frequency)
{
	_frequency = frequency;
	ad9834_update();
}

void ad9834_set_frequency_register(uint8_t selectReg)
{
	_selectReg = selectReg;
	ad9834_update();
}

void dac_init(void)
{
	struct dac_config conf;
	dac_read_configuration(&CONF_AD9834_DAC, &conf);
	dac_set_conversion_parameters(&conf, CONF_AD9834_DAC_REF, DAC_ADJ_RIGHT);
	dac_set_active_channel(&conf, CONF_AD9834_DAC_CHANNEL, 0);	
	#ifdef XMEGA_DAC_VERSION_1
		dac_set_conversion_interval(&conf, 1); // 1uS between conversion
	#endif
	dac_write_configuration(&CONF_AD9834_DAC, &conf);
	dac_enable(&CONF_AD9834_DAC);
}

void spi_init(void)
{		
	// Set the pin used for slave select as output high
	//ioport_configure_pin(CONF_AD9834_SS_PIN, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	
	// Enable pull-up on own chip select (SS), If this pin is pulled low the SPI module will go into slave mode
	// If the SS pin is not used and is configured as input, it must be held high to ensure master operation.
	// If the SS pin is set as input and is being driven low, the SPI module will interpret this as another master trying to take control of the bus
	//ioport_configure_pin(CONF_AD9834_SS_PIN, IOPORT_PULL_UP | IOPORT_DIR_INPUT);
	
	// Set MOSI and SCL as output high, and set MISO as input
	ioport_configure_pin(CONF_AD9834_MOSI_PIN, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	ioport_configure_pin(CONF_AD9834_MISO_PIN, IOPORT_DIR_INPUT);
	ioport_configure_pin(CONF_AD9834_SCK_PIN,  IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
			
	ioport_configure_pin(CONF_AD9834_FSYN_PIN, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);

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
		
	// Keep DDS in reset to avoid spurious output	
	ad9834_write_register(AD9834_BIT_RESET);
	
	dac_init();		
			
	// 1KHz, SineWave, Reg. 0, Disable Reset
	ad9834_update();	
}