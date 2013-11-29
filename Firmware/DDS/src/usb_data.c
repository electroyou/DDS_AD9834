// Author: Leonardo
#include "usb_data.h"

float usb_data_read_float(void){		
	uint32_t tmp;	// 4 byte buffer	
	Byte *byte_ptr = (Byte*)&tmp;	// use byte pointer to advance 1 byte at times
	for (int i=0; i<4; i++)
	{
		// write data to buffer space throught pointer
		*byte_ptr = usb_data_read_byte();		
		byte_ptr++;		
	}		
	float *float_ptr = (float*)&tmp;	// get float pointer of buffer	
	return *float_ptr;					// get value into buffer as float
}
uint32_t usb_data_read_uint32(void){	
	// TODO: To Test
	return ((uint32_t)(usb_data_read_uint16()) | (uint32_t)(usb_data_read_uint16())) << 16;
}
uint16_t usb_data_read_uint16(void){
	Byte data[2];
	data[0] = usb_data_read_byte();
	data[1] = usb_data_read_byte();		
	return data[0] | (data[1] << 8);		
}
Byte usb_data_read_byte(void)
{		
	while(!udi_cdc_is_rx_ready());		
	return udi_cdc_getc();	
}
int usb_data_write_byte(Byte data)
{
	while(!udi_cdc_is_tx_ready());
	return udi_cdc_putc(data);
}