// Author: Leonardo
#ifndef AD9834_H_
#define AD9834_H_

#include <asf.h>
#include "conf_ad9834.h"

enum ad9834_waveform {
	AD9834_SINE = 0,
	AD9834_TRIANGLE = 1,
	AD9834_SQUARE = 2
};

// Internal API
// void ad9834_configure(float frequency, uint8_t selectReg, Bool triangMode, Bool squareMode);
// void ad9834_write_register (uint16_t data);
// void ad9834_update(void);

// Public API
void ad9834_init(void);
void ad9834_set_output_voltage(float vout);
void ad9834_set_frequency(uint16_t frequency);
void ad9834_set_frequency_register(uint8_t selectReg);
void ad9834_set_waveform(enum ad9834_waveform waveform);
//void ad9834_set_phase(uint16_t frequency);
//void ad9834_set_phase_register(uint8_t selectReg);

#endif