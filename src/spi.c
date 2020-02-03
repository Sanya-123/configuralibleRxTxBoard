#include "spi.h"
#include "xprintf.h"
#include "Setting_devise.h"

#if USE_TX_BUFFER
// Буфер на передачу
static volatile uint8_t  tx_buffer[_SPI1_TX_BUFFER_SIZE];
static volatile uint16_t tx_wr_index = 0;           // индекс хвоста буфера (куда писать данные)
static volatile uint16_t tx_rd_index = 0;           // индекс начала буфера (откуда читать данные)
static volatile uint16_t tx_counter = 0;            // количество данных в буфере
#endif

void spi_write(uint8_t data) //
{
#if USE_TX_BUFFER
    //    while (tx_counter == _SPI1_TX_BUFFER_SIZE) {};      // если буфер переполнен, ждем
    if (tx_counter == _SPI1_TX_BUFFER_SIZE) return;      // если буфер переполнен, выходим
    if (tx_counter || (!(SPI1->SR & SPI_SR_TXE)))   // если в буфере уже что-то есть или что-то уже передается
    {
        tx_buffer[tx_wr_index++] = data;                   // то кладем данные в буфер
        if (tx_wr_index == _SPI1_TX_BUFFER_SIZE)
        {
            tx_wr_index = 0;                        //идем по кругу
        }
        SPI1->CR2 &= ~SPI_CR2_TXEIE;            // запрещаем прерывание, чтобы оно не мешало
        tx_counter++;                               // увеличиваем счетчик количества данных в буфере
        SPI1->CR2 |= SPI_CR2_TXEIE;             // разрешаем прерывание
    }
    else                                            //если SPI1 свободен
    {
        //while (!(SPI1->SR & SPI_SR_TXE)) {};    // Проверка что буфер на передачу опустел
        SPI1->DR = data;                              // передаем данные без прерывания
        //SPI1->CR2 |= SPI_CR2_TXEIE;             // разрешаем прерывание
    }

#else
    while ((SPI1->SR & SPI_SR_TXE) == RESET);   // Передатчик занят? ждём
    SPI1->DR = data;                            // Пишем в буфер передатчика
#endif
}

void spi_writeMas(uint8_t *mas, uint8_t size)
{
    for(int i = 0; i < size; i++)
    {
        spi_write(mas[i]);
    }
}

void spi_init()
{
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN ;//|  RCC_APB2ENR_SPI1EN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    AFIO->MAPR |= AFIO_MAPR_SPI1_REMAP;//перемапирую spi

    PIN_CONFIGURATION(HSPI_CLK);
    PIN_CONFIGURATION(HSPI_MISO);
    PIN_CONFIGURATION(HSPI_MOSI);
    PIN_CONFIGURATION(HSPI_CS);

    // SPIC clk = 24/16 = 1.5
    SPI1->CR1 = 0;
    SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 ;


    SPI1->CR2 |= SPI_CR2_RXNEIE | SPI_CR2_TXEIE;//Interupt spi

    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_MSTR | SPI_CR1_SSM);//disable

    SPI1->CR1 |=   SPI_CR1_CPHA | SPI_CR1_CPOL | SPI_CR1_SPE;

    NVIC_EnableIRQ(SPI1_IRQn);

    __enable_irq();

    //interupt pin
    AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI15_PA;
    EXTI->IMR |= EXTI_IMR_IM15;
    EXTI->RTSR |= EXTI_RTSR_RT15;
    //    EXTI->FTSR |= EXTI_FTSR_FT15;

    NVIC_EnableIRQ(EXTI15_10_IRQn);
}


void SPI1_IRQHandler(void)
{
    if(SPI1->SR & SPI_SR_RXNE)
    {
        uint8_t data = SPI1->DR;//read data
        spi_interupt(data);
        SPI1->SR &= ~SPI_SR_RXNE;//clsear falg
    }
#if USE_TX_BUFFER
    if(SPI1->SR & SPI_SR_TXE)//by Tx
    {
        if (tx_counter)                                 //если есть что передать
        {
            --tx_counter;                               // уменьшаем количество не переданных данных
            SPI1->DR = tx_buffer[tx_rd_index++];      //передаем данные инкрементируя хвост буфера
            if (tx_rd_index == _SPI1_TX_BUFFER_SIZE)
            {
                tx_rd_index = 0;                        //идем по кругу
            }
            SPI1->SR &= ~SPI_SR_TXE;//clsear falg
        }
        else                                            //если нечего передать,
        {
            //SPI1->DR = 0xCC;
            //SPI1->DR = 0;
            SPI1->CR2 &= ~SPI_CR2_TXEIE;            //запрещаем прерывание по передачи
            //SPI1->SR &= ~SPI_SR_TXE;//clsear falg
        }
    }
#endif
    else
        SPI1->SR = 0;//clear flag
}

void EXTI15_10_IRQHandler(void)
{
    //у меня используются прерывания по перднему фронту
    if (EXTI->PR & EXTI_PR_PR15)
    {
        cs_interupt();
        EXTI->PR |= EXTI_PR_PR15;//clear flag interupt
    }
}

bool spi_isFree()
{
    if(SPI1->SR & SPI_SR_TXE)//SPI_SR_BSY
        return true;
    return false;
}

#if USE_TX_BUFFER
void spi_flush()
{
    NVIC_DisableIRQ(SPI1_IRQn);
    tx_wr_index = 0;
    SPI1->DR = 0;
    tx_rd_index = 0;
    tx_counter = 0;
    NVIC_EnableIRQ(SPI1_IRQn);
}
#endif
