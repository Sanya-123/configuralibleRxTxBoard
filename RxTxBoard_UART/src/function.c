/* STM32F100C6T6B, HSE 8, sysclk 24
 * USE interface: DAC, USART2, I2C, SPI, CRC;
 *
*/

#include "gpio.h"
#include "defines_gpio.h"
#include "FunctionOnBoard.h"



#define LED_TMP                     A, 1, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Select Rx BP 2 pin PB8
#define LED_TMP1                    A, 7, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Select Rx BP 2 pin PB8



int mainUART(FunctionBoard *funct, uint8_t data, uint8_t *buff, uint8_t *pointBuff)
{
    (void)data;
    (void)funct;
    buff[*pointBuff] = data;
    *pointBuff = *pointBuff + 1;
//    PIN_TOGGLE(LED_TMP);

    return buff[0] + buff[1];
}

