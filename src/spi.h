#ifndef _SPI_H
#define _SPI_H

#include "gpio.h"
#include "defines_gpio.h"
#include <stdbool.h>

#define USE_TX_BUFFER       1

//#define _SPI1_RX_BUFFER_SIZE    20          // размер буфера на прием
#define _SPI1_TX_BUFFER_SIZE    20          // размер буфера на передачу

void spi_init();
void spi_write(uint8_t data);
void spi_writeMas(uint8_t *mas, uint8_t size);
uint16_t spiReadData();
bool spi_isFree();
#if USE_TX_BUFFER
void spi_flush();
#endif

#endif // _SPI_H
