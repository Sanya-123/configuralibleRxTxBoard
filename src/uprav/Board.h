#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include "stm32f1xx.h"

#include "CMD_const.h"
#include "spi_GPIO.h"


typedef struct {
    char name[32];
    UsePerepheria perepheria1;
    SoftSpi *spi;
    ChipSelect *cs;
}Interface;

typedef struct {
    char name[32];
    pFunction function;
}FunctionInMemory;

SoftSpi masSpi[SIZE_SPI];
ChipSelect masCs[SIZE_CS];
Interface masInterface[SIZE_INTERFACE];
FunctionInMemory masFunction[SIZE_FUNCTION];


void initBoard();
uint32_t calcCrc(void* data, uint32_t size);
GPIO_TypeDef *getPort(uint8_t portNum);
void init_GPIO_(uint8_t port_pin, uint8_t output, uint8_t mode_val);

//функция отправления на spi с указание номера spi и номером cs если cs = 255 то не использую его
uint32_t boardReadWriteSPI(uint8_t numSpi, uint8_t numCs, uint32_t data, uint8_t dataSizeBit);
//аналогично толдько с интерфейсом
uint32_t boardReadWriteInterface(uint8_t numInterface, uint32_t data, uint8_t dataSizeBit);



#endif // BOARD_H
