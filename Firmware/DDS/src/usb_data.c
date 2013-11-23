// Author: Leonardo
#include "usb_data.h"

float usb_data_read_float(void){
	Byte data[4];
	data[0] = usb_data_read_byte();
	data[1] = usb_data_read_byte();
	data[2] = usb_data_read_byte();
	data[3] = usb_data_read_byte();
	return *((float*)data);
}
uint32_t usb_data_read_uint32(void){	
	// TODO: Test
	return usb_data_read_uint16() | (usb_data_read_uint16() << 16);
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