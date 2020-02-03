#ifndef UART1_H
#define UART1_H

#include "gpio.h"
#include <stdint.h>
#include <stdbool.h>

void configureUart(uint32_t systemFreq, uint32_t baudrate); 	// ��������� Uart
void writeUartData(uint8_t datadata);							// ������ ������ ����� � UART
uint8_t readUartData();											// ������ ������ ����� �� UART
bool uart_isFree();

#endif
