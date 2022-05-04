#include "soft_spi.h"
#include "Board.h"


void initSoftSpi(SoftSpi spi)
{
    if(spi.portPin_CLK)
    {
        if(spi.numPin_CLK < 8)
        {
            //mode speed
            spi.portPin_CLK->CRL &= (~((15) << (spi.numPin_CLK*4)));
            spi.portPin_CLK->CRL |= (3 << (spi.numPin_CLK*4));
        }
        else
        {
            spi.portPin_CLK->CRH &= (~((15) << ((spi.numPin_CLK - 8)*4)));
            spi.portPin_CLK->CRH |= (3 << ((spi.numPin_CLK - 8)*4));
        }
    }

    if(spi.portPin_MOSI)
    {
        if(spi.numPin_MOSI < 8)
        {
            //mode speed
            spi.portPin_MOSI->CRL &= (~((15) << (spi.numPin_MOSI*4)));
            spi.portPin_CLK->CRL |= (3 << (spi.numPin_MOSI*4));
        }
        else
        {
            spi.portPin_MOSI->CRH &= (~((15) << ((spi.numPin_MOSI - 8)*4)));
            spi.portPin_CLK->CRH |= (3 << ((spi.numPin_MOSI - 8)*4));
        }
    }

    if(spi.portPin_MISO)
    {
        if(spi.numPin_MISO < 8)
        {
            //mode speed
            spi.portPin_MISO->CRL &= (~((15) << (spi.numPin_MISO*4)));
            spi.portPin_CLK->CRL |= (8 << (spi.numPin_MOSI*4));
        }
        else
        {
            spi.portPin_MISO->CRH &= (~((15) << ((spi.numPin_MISO - 8)*4)));
            spi.portPin_CLK->CRH |= (8 << ((spi.numPin_MOSI - 8)*4));
        }
        spi.portPin_MISO->BSRR |= (1 << spi.numPin_MISO) << 16;
    }
}

void initCS(ChipSelect cs)
{
    if(cs.numPin < 8)
    {
        //mode speed
        cs.port->CRL &= (~((15) << (cs.numPin*4)));
        cs.port->CRL |= (3 << (cs.numPin*4));
    }
    else
    {
        cs.port->CRH &= (~((15) << ((cs.numPin - 8)*4)));
        cs.port->CRH |= (3 << ((cs.numPin - 8)*4));
    }
    cs.port->BSRR |= 1 << cs.numPin;
}

static void clkDo(uint32_t *data, SoftSpi spi)//тактирование
{
    spi.portPin_CLK->BSRR |= 1 << spi.numPin_CLK;
    //wait...
    __NOP();
    if(spi.portPin_MISO != 0)
        *data |= (spi.portPin_MISO->IDR >> spi.numPin_MISO) & 0x01;
    else
        *data |= 0;
    spi.portPin_CLK->BRR |= 1 << spi.numPin_CLK;
}

static uint32_t sendData(SoftSpi spi, uint32_t data, uint8_t dataBitSize)
{
    if((spi.portPin_CLK == 0) || (spi.portPin_MOSI == 0)/* || (spi.portPin_MISO == 0)*/)
        return 0;

    uint32_t inData = 0;
    for(int i = dataBitSize - 1; i >= 0; i--)//нагичаю отправку
    {
        inData = inData << 1;
        if((data >> i) & 1)//по одному байту
        {
            spi.portPin_MOSI->BSRR |= 1 << spi.numPin_MOSI;
        }
        else
        {
            spi.portPin_MOSI->BRR |= 1 << spi.numPin_MOSI;
        }
        clkDo(&inData, spi);//2xИнвентирую состояния тактирования
    }

    return inData;
}

uint8_t softSpiWrite(SoftSpi spi, ChipSelect cs, uint8_t data)
{
    return (uint8_t)softSpiWriteSomeBit(spi, cs, data, 8);
}

uint16_t softSpiWriteDouble(SoftSpi spi, ChipSelect cs, uint16_t data)
{
    return (uint16_t)softSpiWriteSomeBit(spi, cs, data, 16);
}

uint32_t softSpiWriteWorld(SoftSpi spi, ChipSelect cs, uint32_t data)
{
    return softSpiWriteSomeBit(spi, cs, data, 32);
}

uint32_t softSpiWriteSomeBit(SoftSpi spi, ChipSelect cs, uint32_t data, uint8_t dataSizeBit)
{
    if(cs.port)
        cs.port->BRR |= 1 << cs.numPin;

    uint32_t res = sendData(spi, data, dataSizeBit);

    if(cs.port)
        cs.port->BSRR |= 1 << cs.numPin;

    return res;
}

//hardware SPI



//void init_SPI_GPIO()
//{
//    PIN_CONFIGURATION(SPI_GPIO_CLK);
//    PIN_CONFIGURATION(SPI_GPIO_DATA);
//    PIN_CONFIGURATION(SPI_GPIO_IN_DA);

//    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

//    SPI2->CR1 = 0;
//    SPI2->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 ;


//    //    SPI2->CR2 |= SPI_CR2_RXNEIE | SPI_CR2_TXEIE;//Interupt spi

//    SPI2->CR1 |= (SPI_CR1_SPE | SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI);//disable
//}


//void spiGpioWrite(uint8_t data)
//{
//    while ((SPI2->SR & SPI_SR_TXE) == RESET);   // Передатчик занят? ждём
//    SPI2->DR = data;                            // Пишем в буфер передатчика
//    while ((SPI2->SR & SPI_SR_BSY) == SET);     // отправлено ждём
//}

//void spiGpioWriteDouble(uint16_t data)
//{
//    spiGpioWrite((uint8_t)(data >> 8));
//    spiGpioWrite((uint8_t)(data & 0x00FF));
//}

//void spiGpioWriteWorld(uint32_t data)
//{
//    spiGpioWriteDouble((uint16_t)(data >> 16));
//    spiGpioWriteDouble((uint16_t)(data & 0x0000FFFF));
//}

