#include "uart1.h"
#include "defines_gpio.h"

void configureUart(uint32_t systemFreq, uint32_t baudrate)
{
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;	// ��������� ������������ UART
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    PIN_CONFIGURATION(UART2_TX);
    PIN_CONFIGURATION(UART2_RX);

	// ������� ����������� ���� USART1->BRR = systemFreq/baudrate; ��� ������ ����������,
	// ����� ����� ��������, �������� 0,5 ��� ��������� ���������� ������.
	// � ������������� ���������� ����� �������c���� ��������� �����������:
    USART2->BRR = (2*systemFreq/baudrate + 1)/2;

    USART2->CR1 = USART_CR1_M | USART_CR1_PCE;//�������� 9 ����������� ���
    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;	// �������� UART, ��������� ������ �������� � �����������

    USART2->CR1 |= USART_CR1_RXNEIE;//ENable Rx interrupr
    NVIC_EnableIRQ(USART2_IRQn);
}

void writeUartData(uint8_t data)
{
    while((USART2->SR & USART_SR_TXE) == RESET){};	// ���������� ������������ ������ �����������
    USART2->DR = data;
}

uint8_t readUartData()
{
    while((USART2->SR & USART_SR_RXNE) == RESET) {}; // ����������, ���� ����� �������� ���-�� ����������
    return USART2->DR;
}
bool uart_isFree()
{
    return ((USART2->SR & USART_SR_TXE));
}

//void USART2_IRQHandler(void)//TODO
//{
//    if ((USART2->SR & (USART_SR_NE | USART_SR_FE | USART_SR_PE | USART_SR_ORE)) == 0) //��������� ��� �� ������
//    {
////        bufferSetData(&bufferReadUART, USART2->DR);//�� ������� ������ ��������� �� � ������
//        uint8_t data =  USART2->DR;
//        data = data;
//        (void)data;
//    }
//    else
//    {
//        (void)USART2->DR;                                 // ���������� ������ � �� ������ ������
//        USART2->SR &= ~USART_SR_RXNE;               // ��� � ���������� ���� ����������
//    }
//}


