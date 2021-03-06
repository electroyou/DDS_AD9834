// Author: Leonardo
// Blocking functions to read and write data over USB
#ifndef USB_DATA_H_
#define USB_DATA_H_

#include "asf.h"

Byte		usb_data_read_byte(void);
uint16_t	usb_data_read_uint16(void);
uint32_t	usb_data_read_uint32(void);
float		usb_data_read_float(void);
int			usb_data_write_byte(Byte data);

#endif