#include "i2c.h"

#define NACK 0
#define ACK 1
#define WRITE 0
#define READ 1

//--------------------------------------------------------------------------------------------------
// Низкоуровненвые операции

void i2c_start(void)
{
    I2C1->CR1 |= I2C_CR1_START;             // формирование сигнала "Старт"
    while (!(I2C1->SR1 & I2C_SR1_SB)) {};   // ждем окончания формирования сигнала "Старт"
}

void i2c_stop(void)
{
    I2C1->CR1 |= I2C_CR1_STOP;              // формирование сигнала "Стоп"
}

void i2c_setAdress(uint8_t adrees)
{
//    I2C1->DR = adrees;
//    while (!(I2C1->SR1 & I2C_SR1_ADDR)) {}; // ожидаем окончания передачи адреса
//    (void) I2C1->SR1;
//    (void) I2C1->SR2;
}

void i2c_write(uint8_t data)
{
    I2C1->DR = data;                        // передаем адрес десятой ячейки
    while (!(I2C1->SR1 & I2C_SR1_BTF)) {};  // ожидаем окончания передачи адреса
}

uint8_t i2c_read(uint8_t ack)
{
    if (ack)
    {
        I2C1->CR1 |= I2C_CR1_ACK;
    }
    else                                    // перед приемом последнего байта необходимо выполнить
    {
        I2C1->CR1 &= ~I2C_CR1_ACK;
    }

    while (!(I2C1->SR1 & I2C_SR1_RXNE)) {}; // ожидаем окончания приема данных
    return (uint8_t)I2C1->DR;                        // cчитываем приянтое значение
}

//=================================================================================================

void i2c_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;    // Включаем тактирование
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    PIN_CONFIGURATION(I2C_SCL);
    PIN_CONFIGURATION(I2C_SDA);

    // пирмер настройки https://github.com/Catethysis/stm32_i2c/blob/master/SPL/stm32f10x_i2c.c

    // прописываем значение системной частоты.
    I2C1->CR2 &= ~I2C_CR2_FREQ;
//    I2C1->CR2 |= SystemCoreClock / 1000000;

    // прописываем делитель частоты.
    I2C1->CCR &= ~I2C_CCR_CCR;
    I2C1->CCR |= I2C_CCR_FS ;                                   // for full speed 400kHz,
//    I2C1->CCR |= _I2C_CLK / (400000 * 3);                       // 400000, Tlow/Thigh = 2

//    I2C1->TRISE = (((_I2C_CLK / 1000000) * 300) / 1000) + 1;    // Set Max rise time for fast mode

    I2C1->CR1 |= I2C_CR1_PE;                                    // разрешаем работу
}

uint8_t i2c_readByte(uint8_t hwAddress, uint8_t regAddress)
{
    i2c_start();
    i2c_setAdress(hwAddress | WRITE);
    i2c_write(regAddress);
    i2c_start();
    i2c_setAdress(hwAddress | READ);
    uint8_t data = i2c_read(NACK);
    i2c_stop();

    return data;
}


void i2c_writeByte(uint8_t hwAddress, uint8_t regAddress, uint8_t data)
{
    i2c_start();
    i2c_setAdress(hwAddress | WRITE);
    i2c_write(regAddress);
    i2c_write(data);
    i2c_stop();
}

