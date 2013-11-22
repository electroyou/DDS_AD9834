// Author: Leonardo
#ifndef AD9834_H_
#define AD9834_H_

#include <asf.h>
#include "conf_ad9834.h"

//enum ad9834_waveform {
	//SINE = 0,
	//TRIANGLE = 1		
//};

void ad9834_write_register (uint16_t data);
// Internal API
void ad9834_configure(uint32_t frequency, uint8_t selectReg, Bool triangMode, Bool squareMode);


void ad9834_init(void);
void ad9834_set_output_voltage(float vout);

// New API
//void ad9834_set_frequency(uint32_t frequency);
//void ad9834_set_frequency_register(uint8_t selectReg);
//void ad9834_set_waveform(ad9834_waveform waveform);

#endif