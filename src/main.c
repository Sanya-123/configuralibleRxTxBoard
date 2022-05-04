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
#include "soft_spi.h"
#include "Setting_devise.h"
#include "timer_PWM.h"


void initAll();

/*inline*/ void resetMK()
{
    __disable_irq();
    NVIC_SystemReset();
}

/* ++
 * GPIO init output
 * GPIO write
 * SPI init
 * SPI write
 * CS init
 * Interface init
 * Interface write
 * Function write
 * Function init
 * Function do
 * Function timer
 * save
 *
 */

//#define LED_TMP                     C, 9, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0     //Select Rx BP 2 pin PB8
//#define LED_TMP                     A, 1, MODE_AF_PUSH_PULL, SPEED_50MHZ, 0         //Select Rx BP 2 pin PB8


int main()
{
    uint32_t kk = sizeof(configBoard);
    kk = sizeof(parametrs);
    kk = sizeof(parametrsChange);
    (void)kk;
    initAll();
    RTOS_Init();
    loadConfig();
    spi_init();
//    configureUart(SystemCoreClock, SPEED_UART);
    int p = execUART(10);
    p = execUART(15);
    p = execUART(27);
    p = execUART(16);




    while(1)
    {
        RTOS_DispatchTask();
        taskUprav();
        IWDG_res();//TODO сломался watchdog
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

    initSetting();
    initBoard();
    initIWDG(TIME_OUT_IWDG);
}

//CRC
/* RCC->AHBENR |= RCC_AHBENR_CRCEN;
 * CRC->DR = 0x6524BE5F;
 * CRC->DR = 0x76AC1213;
 * uint32_t crc = CRC->DR;// == 0xACB5CE34
 * //Сброс
 * CRC->CR = CRC_CR_RESET;//1
*/
