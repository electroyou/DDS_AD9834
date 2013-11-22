// Author: Leonardo
#include "usb_data.h"

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