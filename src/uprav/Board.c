#include "Board.h"

void initBoard()
{
    //TODO чтение из памяти конфигурации
    for(uint8_t i = 0; i < SIZE_INTERFACE; i++)
    {
        masInterface[i].cs = 0;
        masInterface[i].spi = 0;
        masInterface[i].perepheria1 = None_int;
    }
    for(uint8_t i = 0; i < SIZE_CS; i++)
    {
        masCs[i].numPin = 0;
        masCs[i].port = 0;
    }
    for(uint8_t i = 0; i < SIZE_SPI; i++)
    {
        masSpi[i].portPin_CLK = 0;
        masSpi[i].portPin_MOSI = 0;
        masSpi[i].portPin_MISO = 0;

        masSpi[i].numPin_CLK = 0;
        masSpi[i].numPin_MOSI = 0;
        masSpi[i].numPin_MISO = 0;
    }
}

uint32_t calcCrc(void* data, uint32_t size)
{
    if(size%4)
        return 0;
    CRC->CR |= CRC_CR_RESET;
    for(uint32_t i = 0; i < size; i+=4)
    {
        CRC->DR = *((uint32_t*)(data + i));
    }

    uint32_t crc = CRC->DR;
    return crc;
}

GPIO_TypeDef *getPort(uint8_t portNum)
{
    GPIO_TypeDef *port = 0;
    switch (portNum) {
    case 0:
        port = GPIOA;
        break;
    case 1:
        port = GPIOB;
        break;
    case 2:
        port = GPIOC;
        break;
    case 3:
        port = GPIOD;
        break;
    default:
        break;
    }
    return port;
}

void init_GPIO_(uint8_t port_pin, uint8_t output, uint8_t mode_val)
{
    GPIO_TypeDef *port = getPort((port_pin) >> 4);
    if(port)
    {
        uint8_t pin = port_pin & 0x0F;
        uint8_t speed = (output) == 1 ? 3 : 0;// если на выход то скорость максимальная
        uint8_t mode = (mode_val >> 4) & 0x0F;
        uint8_t val = mode_val & 0x01;
        if(pin < 8)
        {
            port->CRL &= (~((15) << ((pin)*4)));
            port->CRL |= (((mode << 4) | speed) << ((pin)*4));
        }
        else
        {
            port->CRH &= (~((15) << ((pin - 8)*4)));
            port->CRH |= (((mode << 4) | speed) << ((pin - 8)*4));
        }
        if(val)
            port->BSRR |= val << pin;
        else
            port->BRR |= val << pin;
    }
}

uint32_t boardReadWriteSPI(uint8_t numSpi, uint8_t numCs, uint32_t data, uint8_t dataSizeBit)
{
    if(numSpi < parametrs.sizeSPI)
    {
        ChipSelect cs0/* = (ChipSelect){0}*/;
        cs0.numPin = 0;
        cs0.port = 0;
        return spiGpioWriteSomeBit(masSpi[numSpi], (numCs < parametrs.sizeCS ? masCs[numCs] : cs0), data, dataSizeBit);
    }

    return 0;
}

uint32_t boardReadWriteInterface(uint8_t numInterface, uint32_t data, uint8_t dataSizeBit)
{
    if(numInterface < parametrs.sizeInterfase)
    {
        if(masInterface[numInterface].perepheria1 == SPI_int)
        {
            return spiGpioWriteSomeBit(*(masInterface[numInterface].spi), *(masInterface[numInterface].cs), data, dataSizeBit);
        }
    }
    return 0;
}
