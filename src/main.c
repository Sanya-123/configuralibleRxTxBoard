/* STM32F100C6T6B, HSE 8, sysclk 24
 * USE interface: DAC, USART2, I2C, SPI, CRC;
 *
*/

#ifdef USE_RTOS
#include "FreeRTOS.h"
#include "task.h"
//#include "timers.h"
//#include "semphr.h"
#else
#include "sheduler.h"
#endif
#include "gpio.h"
#include "defines_gpio.h"
#include "uart1.h"
#include "dac.h"
#include "xprintf.h"
#include "spi.h"
#include "i2c.h"
#include "spi_GPIO.h"
#include "Setting_devise.h"
//#include "PortWake.h"


void initAll();
void taskDacTests();
void task_diod();


void delay(int i)
{
     for(;i>0;i--);
}

/* ++
 * GPIO init output
 * GPIO write
 * SPI init
 * SPI write
 * CS init
 * Interface init
 * Interface write
 *
 */

//#define LED_TMP                     C, 9, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Select Rx BP 2 pin PB8
#define LED_TMP                     A, 1, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Select Rx BP 2 pin PB8


//typedef int (*pFunction)(int *a, int *b, int c);

int main()
{
    initAll();
    RTOS_Init();
//    PIN_CONFIGURATION(LED_TMP);
//    parametrs.initFunction.addres = 0x08007000;
//    uint32_t addres = *((__IO uint32_t*)(parametrs.initFunction.addres + 4));
//    masFunction[parametrs.sizeFunction].function = (pFunction)addres;
//    strncpy(masFunction[parametrs.sizeFunction].name, parametrs.initFunction.name, MAX_SIZE_NAME);
//    parametrs.sizeFunction++;

//    int ret = masFunction[parametrs.doFunction.function].function(&functionBoard, parametrs.doFunction.arguments, parametrs.doFunction.sizeArgument);
//    (void)ret;

//    parametrs.functionTimer.function = 0;
//    parametrs.functionTimer.taskDelay = 1000;
//    parametrs.functionTimer.taskPeriod = 1000;

//    RTOS_SetTask(masFunction[parametrs.functionTimer.function].function, parametrs.functionTimer.taskDelay,  \
//            parametrs.functionTimer.taskPeriod, parametrs.functionTimer.arguments, \
//            parametrs.functionTimer.sizeArgument);



//    RTOS_Init();
//    delay(13);

//    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN |RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN;
//    PIN_CONFIGURATION(LED_TMP);
//    PIN_ON(LED_TMP);
//    return;


//    delay(10000000);
//    PIN_OFF(LED_TMP);



    while(1)
    {
        RTOS_DispatchTask();
        taskUprav();
    }
}

void SystemClock_Config(void)
{

//    //включаю HSE
//    RCC->CR |= RCC_CR_HSEON;
//    while((RCC->CR & RCC_CR_HSERDY) == RESET);//жду пока станет готово

    //HCLK = SYSCLK1
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    //PCLK1 = HCLK
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    //PCLK1 = HCLK
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;

    //множитель PLL настраеваю на 3 pllclk = HSI/2 * 6 = 24 при кварц 8 МГц
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
    RCC->CFGR |= RCC_CFGR_PLLMULL6;

    //включаю PLL
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == RESET);

    //выбираю PLL как источник системной частоты
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while((RCC->CFGR & RCC_CFGR_SWS_PLL) == RESET);

    SystemCoreClock = 24000000;
}

/* +
 * HSPI
 * HSPI_CS interupt
 * ADF1
 * ADF2
 * DAC
 * AD7814
 * AD5304
 * RX_2
 * HSPI protocol
 * HSPI uprav1
 * HSPI task
 * HSPI memory
 */

void initAll()
{
    SystemClock_Config();
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN |RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN;

//    //Thats I use becouse JTAG pin in spi pin
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; // JTAG-DP Disabled and SW-DP Enabled

    spi_init();
    initSetting();
}

void taskDacTests()
{
    xprintf("***123***\n");
    PIN_TOGGLE(LED_1);
//    PIN_TOGGLE(LED_2);
//    PIN_TOGGLE(LED_3);
}

void task_diod()
{
    PIN_TOGGLE(LED_3);
}



//CRC
/* RCC->AHBENR |= RCC_AHBENR_CRCEN;
 * CRC->DR = 0x6524BE5F;
 * CRC->DR = 0x76AC1213;
 * uint32_t crc = CRC->DR;// == 0xACB5CE34
 * //Сброс
 * CRC->CR = CRC_CR_RESET;//1
*/
