#ifndef SPI_GPIO_H
#define SPI_GPIO_H

#include "gpio.h"
#include "defines_gpio.h"
#include "stm32f1xx.h"
#include "stdbool.h"

typedef struct __attribute__((packed)){
    GPIO_TypeDef *port;
    uint8_t numPin;
}ChipSelect;

typedef struct __attribute__((packed)){
    //порты выходных ножек
    GPIO_TypeDef *portPin_CLK;
    GPIO_TypeDef *portPin_MOSI;
    GPIO_TypeDef *portPin_MISO;

    //номера ножек портов
    uint8_t numPin_CLK;
    uint8_t numPin_MOSI;
    uint8_t numPin_MISO;
}SoftSpi;

#define ADD_CS_PIN(port,pin)        _PIN_CONFIGURATION(port, pin, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 1)//инициализация ножки Chip select

void init_SPI_GPIO(SoftSpi spi);

void initCS(ChipSelect cs);

uint8_t spiGpioWrite(SoftSpi spi, ChipSelect cs, uint8_t data);

uint16_t spiGpioWriteDouble(SoftSpi spi, ChipSelect cs, uint16_t data);

uint32_t spiGpioWriteWorld(SoftSpi spi, ChipSelect cs, uint32_t data);

uint32_t spiGpioWriteSomeBit(SoftSpi spi, ChipSelect cs, uint32_t data, uint8_t dataSizeBit);


#endif // SPI_GPIO_H
