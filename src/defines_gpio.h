#ifndef DEFINES_GPIO
#define DEFINES_GPIO

#include "gpio.h"

//UART1 use with interface
#define UART2_TX                        A, 2, MODE_AF_PUSH_PULL, SPEED_50MHZ, 0		//UART2 Tx PA2
#define UART2_RX                        A, 3 , MODE_INPUT_PULL_UP, 0, 1			//UART2 Rx PA3
#define EVENT                           B, 1, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //event for interface with uart PB1
#define UART_TX_RX                      A, 12, MODE_AF_PUSH_PULL, SPEED_50MHZ, 0        //CTS uart PA12

//UART2 users
#define UART_TXD                        A, 2, MODE_AF_PUSH_PULL, SPEED_50MHZ, 0		//UART2 Tx PA2          +
#define UART_RXD                        A, 3, MODE_INPUT_PULL_UP, 0, 1			//UART2 Rx PA3          +

//SPI
//NOTE: thats configurate for FULL duplex
#define HSPI_CLK                        B, 3, MODE_INPUT_PULL_DOWN, SPEED_50MHZ, 0      //SPI CLK PB3           +
#define HSPI_MISO                       B, 4, MODE_AF_PUSH_PULL, SPEED_50MHZ, 0         //SPI MISO PB4          +
//#define HSPI_MISO                       B, 4, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0        //SPI MISO PB4
#define HSPI_MOSI                       B, 5, MODE_INPUT_PULL_DOWN, SPEED_50MHZ, 0      //SPI MOSI PB5          +
#define HSPI_CS                         A, 15, MODE_INPUT_PULL_UP, SPEED_50MHZ, 0       //SPI CS PA15           +

//I2C
#define I2C_SCL                         B, 6, MODE_AF_OPEN_DRAIN, SPEED_50MHZ, 0        //I2C CLK PB6
#define I2C_SDA                         B, 7, MODE_AF_OPEN_DRAIN, SPEED_50MHZ, 0        //I2C DATA PB7

//SPI configurate pin (SSPI)
#define SPI_GPIO_CLK                    B, 13, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0    //my spi CLK        +
#define SPI_GPIO_DATA                   B, 15, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0  //my spi DATA       +
//#define SPI_GPIO_CLK                    B, 13, MODE_AF_PUSH_PULL, SPEED_50MHZ, 0    //my spi CLK        +
//#define SPI_GPIO_DATA                   B, 15, MODE_AF_PUSH_PULL, SPEED_50MHZ, 0  //my spi DATA       +
#define SPI_GPIO_IN_DA                  B, 14, MODE_INPUT_PULL_DOWN, SPEED_50MHZ, 0     //my spi DATA input                     +

//Pin cs for spi and perepheria
#define SYNT_TX_CS                       A, 8, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 1    //CS for syntizator ADF4351 - Tx PB13        +
#define SYNT_RX_CS                       A, 7, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 1     //CS for syntizator ADF4351 - Rx PB11       +
#define SYNT_RX1_CS                     A, 1, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 1     //CS for syntizator _______ - Rx1 PA1       +

//Pin MUXOUT
#define MUX_SUNT_LOW                    B, 15, MODE_INPUT_PULL_DOWN, SPEED_50MHZ, 0     //MUX for syntizator ADF4351- low PB15
#define MUX_SUNT_144                    A, 8, MODE_INPUT_PULL_DOWN, SPEED_50MHZ, 0      //MUX for syntizator ADF4351- 144 PA8
#define MUX_SUNT_HIGHT                  B, 0, MODE_INPUT_PULL_DOWN, SPEED_50MHZ, 0      //MUX for syntizator LMX2594- hight PB0

//DAC
#define DAC_CANAL1_OUT                  A, 4, MODE_ANALOG, 0, 0                         //DAC chanal 1 PA4                          +
//AD5304 DAC 4 chanale on SPI
#define AD5304_SYNC                     C, 14, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 1    //CS for DAC 4 chanale PC14                 +

//Temperature
//AD7814
#define AD7814_CS                       A, 5, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 1    //CS for temperature PB9     +

//Attenuator
//HRF-AT4611-FL
#define HRF_AT_CS                       B, 10, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 1    //CS for AT HRF-AT4611-FL - TX_ATT (TX_ATT_CS) PB10

//LED
#define LED_1                           A, 1, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Led 1 PA1
#define LED_2                           A, 7, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Led 2 PA7
#define LED_3                           B, 2, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Led 3 PB2

//Shleafe
#define SHL_ENABLE                      A, 4, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Shleafe pin PA4

//Select TX\RX
#define RX_TX                           B, 10, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0    //Select Tx freq pin PA11
#define RX_TX_CNTR                      B, 1, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Select Rx freq pin PC13       +

//BYPASS (yseliteli)
#define BYPASS_IF                       C, 13, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Select Rx BP 1 pin PB9           +

#define BYPASS_RX_2                     B, 8, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Select Rx BP 2 pin PB8


#endif // DEFINES_GPIO

