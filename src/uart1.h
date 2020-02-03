#ifndef UART1_H
#define UART1_H

#include "gpio.h"
#include <stdint.h>
#include <stdbool.h>

void configureUart(uint32_t systemFreq, uint32_t baudrate); 	// настройка Uart
void writeUartData(uint8_t datadata);							// Запись одного байта в UART
uint8_t readUartData();											// Чтение одного байта из UART
bool uart_isFree();

#endif
