/* STM32F100C6T6B, HSE 8, sysclk 24
 * USE interface: DAC, USART2, I2C, SPI, CRC;
 *
*/

#include "gpio.h"
#include "defines_gpio.h"



#define LED_TMP                     A, 1, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Select Rx BP 2 pin PB8
#define LED_TMP1                    A, 7, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Select Rx BP 2 pin PB8

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

int function(FunctionBoard *funct, char *argyment, uint32_t sizeArgument)
{
    (void)argyment;
    (void)sizeArgument;
    (void)funct;
    funct->readWriteInerface(1, 0x00d80005, 32);//off 0x00180005
//    PIN_TOGGLE(LED_TMP);

    return 0;
}

