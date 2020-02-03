#ifndef _I2C_H_
#define _I2C_H_

#include "gpio.h"
#include "defines_gpio.h"

void i2c_init(void);
void i2c_writeByte(uint8_t hwAddress, uint8_t regAddress, uint8_t data);
uint8_t i2c_readByte(uint8_t hwAddress, uint8_t regAddress);



#endif // _I2C_H_
