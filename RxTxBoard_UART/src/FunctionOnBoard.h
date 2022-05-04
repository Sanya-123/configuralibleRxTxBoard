#ifndef FUNCTION_ON_BOARD_H
#define FUNCTION_ON_BOARD_H

#include <stdint.h>
#include <stdbool.h>


typedef enum{
    None_int = 0,
    SPI_int = 1,
    I2C_int = 2,
}UsePerepheria;

typedef uint32_t (*_boardReadWriteSPI)(uint8_t numSpi, uint8_t numCs, uint32_t data, uint8_t dataSizeBit);
typedef uint32_t (*_boardReadWriteInterface)(uint8_t numInterface, uint32_t data, uint8_t dataSizeBit);
typedef int (*_boarInitCS)(uint8_t pinPort);
typedef int (*_boarInitSPI)(uint8_t pinPortCLK, uint8_t pinPortMOSI, uint8_t pinPortMSIO);
typedef int (*_boarInitInterface)(UsePerepheria per, uint8_t spi, uint8_t cs, char *name);

typedef struct {
    _boardReadWriteSPI readWriteSPI;
    _boardReadWriteInterface readWriteInerface;
    _boarInitCS initCS;
    _boarInitSPI initSpi;
    _boarInitInterface initInterface;
}FunctionBoard;

FunctionBoard functionBoard;

typedef int (*pFunction)(FunctionBoard *funct, char *argyment, uint32_t sizeArgument);//указатель на функцию


#endif // FUNCTION_ON_BOARD_H
