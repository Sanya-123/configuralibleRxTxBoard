#include "uart1.h"
#include "defines_gpio.h"

void configureUart(uint32_t systemFreq, uint32_t baudrate)
{
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;	// Разрешаем тактирование UART
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    PIN_CONFIGURATION(UART1_TX);
    PIN_CONFIGURATION(UART1_RX);

	// Обычная конструкция вида USART1->BRR = systemFreq/baudrate; даёт ошибку округления,
	// чтобы этого избежать, прибавим 0,5 для получения ближайшего целого.
	// в целочисленной арифметике этому соответcвует следующая конструкция:
    USART1->BRR = (2*systemFreq/baudrate + 1)/2;

    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;	// включаем UART, разрешаем работу приёмника и передатчика

    USART1->CR1 |= USART_CR1_RXNEIE;//ENable Rx interrupr
    NVIC_EnableIRQ(USART1_IRQn);
}

void writeUartData(uint8_t data)
{
    while((USART1->SR & USART_SR_TXE) == RESET){};	// Дожидаемся освобождения буфера передатчика
    USART1->DR = data;
}

uint8_t readUartData()
{
    while((USART1->SR & USART_SR_RXNE) == RESET) {}; // Дожидаемся, пока буфер приёмника чем-то заполнится
    return USART1->DR;
}
bool uart_isFree()
{
    return ((USART1->SR & USART_SR_TXE));
}

void USART1_IRQHandler(void)//TODO
{
    if ((USART1->SR & (USART_SR_NE | USART_SR_FE | USART_SR_PE | USART_SR_ORE)) == 0) //проверяем нет ли ошибок
    {
//        bufferSetData(&bufferReadUART, USART2->DR);//по приходу данных записываб их в буффер
        uint8_t data =  USART1->DR;
        data = data;
        (void)data;
    }
    else
    {
        (void)USART1->DR;                                 // вычитываем данные и на всякий случай
        USART1->SR &= ~USART_SR_RXNE;               // ещё и сбрасываем флаг прерывания
    }
}


