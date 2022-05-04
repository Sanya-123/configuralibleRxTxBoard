#include "Board.h"
#include <stddef.h>
#include <string.h>
#include "memory_conf.h"
#include "sheduler.h"

void timer_init()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;         //подать тактирование на TIM3
    TIM3->PSC     = SystemCoreClock/1000000-1;  //настроить делитель для формирования us
}

int _initCS(uint8_t pinPort)
{
    if(configBoard.sizeCS < SIZE_CS)
    {
        init_GPIO_(pinPort, 1, 0x01);
        configBoard.masCs[configBoard.sizeCS].numPin = pinPort & 0x0F;
        configBoard.masCs[configBoard.sizeCS].port = getPort(pinPort >> 4);
        configBoard.sizeCS++;
        return 0;
    }
    return -1;
}

int _initSPI(uint8_t pinPortCLK, uint8_t pinPortMOSI, uint8_t pinPortMSIO)
{
    if(configBoard.sizeSPI < SIZE_SPI)
    {
        configBoard.masSpi[configBoard.sizeSPI].numPin_CLK = pinPortCLK & 0x0F;
        configBoard.masSpi[configBoard.sizeSPI].numPin_MOSI = (pinPortMOSI) & 0x0F;
        configBoard.masSpi[configBoard.sizeSPI].numPin_MISO = (pinPortMSIO) & 0x0F;

        configBoard.masSpi[configBoard.sizeSPI].portPin_CLK = getPort((pinPortCLK >> 4) & 0x0F);
        configBoard.masSpi[configBoard.sizeSPI].portPin_MOSI = getPort((pinPortMOSI >> 4) & 0x0F);
        configBoard.masSpi[configBoard.sizeSPI].portPin_MISO = getPort((pinPortMSIO >> 4) & 0x0F);

        initSoftSpi(configBoard.masSpi[configBoard.sizeSPI]);
        configBoard.sizeSPI++;
        return 0;
    }
    return -1;
}

int _initInterface(UsePerepheria per, uint8_t spi, uint8_t cs, char *name)
{
    if(configBoard.sizeInterfase < SIZE_INTERFACE)
    {

        configBoard.masInterface[configBoard.sizeInterfase].perepheria1 = per;
        if(configBoard.masInterface[configBoard.sizeInterfase].perepheria1 == SPI_int)
        {
            uint8_t numSpi = spi;
            if(numSpi < configBoard.sizeSPI)//если наддый spi существует
            {
                configBoard.masInterface[configBoard.sizeInterfase].spi = &(configBoard.masSpi[numSpi]);
                configBoard.masInterface[configBoard.sizeInterfase].cs = NULL;
                uint8_t numCs = cs;
                if(numCs < configBoard.sizeCS)
                {
                    configBoard.masInterface[configBoard.sizeInterfase].cs = &(configBoard.masCs[numCs]);
                }
                strncpy(configBoard.masInterface[configBoard.sizeInterfase].name, name, MAX_SIZE_NAME);
                configBoard.sizeInterfase++;
                return 0;
            }
        }
    }
    return -1;
}

int _init_function(uint32_t addres, char *name)
{
    if(configBoard.sizeFunction < SIZE_FUNCTION)
    {
        configBoard.masAddresFunction[configBoard.sizeFunction] = addres;
        uint32_t _addres = *((__IO uint32_t*)(addres + 4));
        configBoard.masFunction[configBoard.sizeFunction].function = (pFunction)_addres;
        strncpy(configBoard.masFunction[configBoard.sizeFunction].name, name, MAX_SIZE_NAME);

        configBoard.sizeFunction++;
        return 0;
    }
    return -1;
}

void zerosConfig()
{
    for(uint8_t i = 0; i < SIZE_INTERFACE; i++)
    {
        configBoard.masInterface[i].cs = 0;
        configBoard.masInterface[i].spi = 0;
        configBoard.masInterface[i].perepheria1 = None_int;
    }
    for(uint8_t i = 0; i < SIZE_CS; i++)
    {
        configBoard.masCs[i].numPin = 0;
        configBoard.masCs[i].port = 0;
    }
    for(uint8_t i = 0; i < SIZE_SPI; i++)
    {
        configBoard.masSpi[i].portPin_CLK = 0;
        configBoard.masSpi[i].portPin_MOSI = 0;
        configBoard.masSpi[i].portPin_MISO = 0;

        configBoard.masSpi[i].numPin_CLK = 0;
        configBoard.masSpi[i].numPin_MOSI = 0;
        configBoard.masSpi[i].numPin_MISO = 0;
    }
    for(uint8_t i = 0; i < SIZE_FUNCTION; i++)
    {
        configBoard.timeFunction[i].start = false;
        configBoard.timeFunction[i].taskDelay = 0;
        configBoard.timeFunction[i].taskPeriod = 0;
    }

    configBoard.sizeCS = 0;
    configBoard.sizeFunction = 0;
    configBoard.sizeInterfase = 0;
    configBoard.sizeSPI = 0;
}

void initBoard()
{
    zerosConfig();
    timer_init();

    //функции которы доступны функциям в памяти
    functionBoard.readWriteInerface = boardReadWriteInterface;
    functionBoard.readWriteSPI = boardReadWriteSPI;
    functionBoard.initSpi = _initSPI;
    functionBoard.initCS = _initCS;
    functionBoard.initInterface = _initInterface;
    functionBoard.delay_v = delay;
    functionBoard.delay_us_v = delay_us;
    functionBoard.delay_ms_v = delay_ms;

}

bool loadConfig()//чтеие из памяти configa
{
    readFlashData32(ADDRES_PAGE_PARAMETRS_STATION, (uint32_t*)(&configBoard), (sizeof(configBoard))/4);
    if(configBoard.crc == calcCrc(&configBoard, sizeof(configBoard) - 4))//проверка crc
    {
        for(uint8_t i = 0; i < SIZE_PORT_GPIO; i++)//восстанавливаю регистры конфигурации gpio
        {
            getPort(i)->CRL = configBoard.gpioState[i].CRL;
            getPort(i)->CRH = configBoard.gpioState[i].CRH;
        }
        for(uint8_t i = 0; i < configBoard.sizeSPI; i++)
        {
            initSoftSpi(configBoard.masSpi[i]);//по сути не надо инициаровать еще раз ножки spi ведь проинициализированны все порты конфигурации
        }
        for(uint8_t i = 0; i < configBoard.sizeCS; i++)
        {
            initCS(configBoard.masCs[i]);
        }
        for(uint8_t i = 0; i < configBoard.sizeInterfase; i++)
        {
        }
        for(uint8_t i = 0; i < configBoard.sizeFunction; i++)
        {
            if(configBoard.timeFunction[i].start)
            {
                RTOS_SetTask(configBoard.masFunction[i].function, configBoard.timeFunction[i].taskDelay, \
                             configBoard.timeFunction[i].taskPeriod, "", 0);
            }
        }

        parametrs.sizeSPI = configBoard.sizeSPI;
        parametrs.sizeCS = configBoard.sizeCS;
        parametrs.sizeInterfase = configBoard.sizeInterfase;
        parametrs.sizeFunction = configBoard.sizeFunction;
        return true;
    }

    zerosConfig();//при неудачном чтении обнуляю config
    return false;
}

void saveConfig()
{
    for(uint8_t i = 0; i < SIZE_PORT_GPIO; i++)//сохраняю регистры конфигурации gpio
    {
        configBoard.gpioState[i].CRL = getPort(i)->CRL;
        configBoard.gpioState[i].CRH = getPort(i)->CRH;
    }
    configBoard.crc = calcCrc(&configBoard, sizeof(configBoard) - 4);
    writeFlashData32(ADDRES_PAGE_PARAMETRS_STATION, (uint32_t*)(&configBoard), (sizeof(configBoard))/4);
}

void functionTimeStart(uint8_t function, uint32_t taskDelay, uint32_t taskPeriod, char *arguments, uint32_t sizeArgument)
{
    if(function < configBoard.sizeFunction)
    {
        RTOS_SetTask(configBoard.masFunction[function].function, taskDelay, taskPeriod, arguments, sizeArgument);
        configBoard.timeFunction[function].start = 1;
        configBoard.timeFunction[function].taskDelay = taskDelay;
        configBoard.timeFunction[function].taskPeriod = taskPeriod;
        //TODO
//        strncpy(configBoard.timeFunction[function].arguments, arguments, MAX_ARGUMENT_SIZE);
//        configBoard.timeFunction[function].sizeArgument = sizeArgument;
    }
}

void functionTimerStop(uint8_t function)
{
    if(function < configBoard.sizeFunction)
    {
        if(configBoard.timeFunction[function].start)
        {
            RTOS_DeleteTask(configBoard.masFunction[function].function);
            configBoard.timeFunction[function].start = 0;
        }
    }
}

// Функция инициализации сторожевого таймера IWDG
// http://blog.myelectronics.com.ua/stm32-%D1%81%D1%82%D0%BE%D1%80%D0%BE%D0%B6%D0%B5%D0%B2%D1%8B%D0%B5-%D1%82%D0%B0%D0%B9%D0%BC%D0%B5%D1%80%D1%8B-wdt/
void initIWDG(uint16_t tw_ms) // Параметр tw от 7мс до 26200мс
{
    RCC->CSR |= RCC_CSR_LSION; // Пуск таймера

    // Для IWDG_PR=7 Tmin=6,4мс RLR=Tмс*40/256
    IWDG->KR = 0x5555; // Ключ для доступа к таймеру
    IWDG->PR = 7; // Обновление IWDG_PR
    IWDG->RLR = tw_ms * 40 / 256; // Загрузить регистр перезагрузки
    IWDG->KR = 0xAAAA; // Перезагрузка
    IWDG->KR = 0xCCCC; // Пуск таймера
}

void IWDG_res(void) // Функция перезагрузки сторожевого таймера IWDG
{
    IWDG->KR = 0xAAAA; // Перезагрузка
}

void delay(uint32_t i)
{
     for(;i>0;i--)
     {
         IWDG_res();
     }
}

void delay_us(uint16_t us)
{
    TIM3->ARR = us;                         //Устанавливаем значение переполнения таймера, а значит и значение при котором генерируется Событие обновления
    TIM3->EGR |= TIM_EGR_UG;                //Генерируем Событие обновления для записи данных в регистры PSC и ARR
    TIM3->CR1 |= TIM_CR1_CEN|TIM_CR1_OPM;	//Запускаем таймер записью бита CEN и устанавливаем режим Одного прохода установкой бита OPM
    while ((TIM3->CR1 & TIM_CR1_CEN) != 0){IWDG_res();};
}

void delay_ms(uint16_t ms)
{
    for(uint16_t i = 0; i < (ms/65); i++)
        delay_us(65*1000);
    delay_us((ms%65)*1000);
}

uint32_t calcCrc(void* data, uint32_t size)
{
    if(size%4)
        return 0;
    CRC->CR |= CRC_CR_RESET;
    for(uint32_t i = 0; i < size; i+=4)
    {
        CRC->DR = *((uint32_t*)(data + i));
    }

    uint32_t crc = CRC->DR;
    return crc;
}

GPIO_TypeDef *getPort(uint8_t portNum)//получение порка их его номера
{
    GPIO_TypeDef *port = 0;
    switch (portNum) {
    case 0:
        port = GPIOA;
        break;
    case 1:
        port = GPIOB;
        break;
    case 2:
        port = GPIOC;
        break;
    case 3:
        port = GPIOD;
        break;
    default:
        break;
    }
    return port;
}

uint8_t getPinPort(GPIO_TypeDef* port)//получение номера из его порта
{
    uint8_t portPin = 0xFF;
    if(port == GPIOA)
        portPin = 0;
    else if (port == GPIOB)
        portPin = 1;
    else if (port == GPIOC)
        portPin = 2;
    else if (port == GPIOD)
        portPin = 3;
    return portPin;
}

void init_GPIO_(uint8_t port_pin, uint8_t output, uint8_t mode_val)
{
    GPIO_TypeDef *port = getPort((port_pin) >> 4);
    if(port)
    {
        uint8_t pin = port_pin & 0x0F;
        uint8_t speed = (output) == 1 ? 3 : 0;// если на выход то скорость максимальная
        uint8_t mode = (mode_val >> 4) & 0x0F;
        uint8_t val = mode_val & 0x01;
        if(pin < 8)
        {
            port->CRL &= (~((15) << ((pin)*4)));
            port->CRL |= (((mode << 4) | speed) << ((pin)*4));
        }
        else
        {
            port->CRH &= (~((15) << ((pin - 8)*4)));
            port->CRH |= (((mode << 4) | speed) << ((pin - 8)*4));
        }
        if(val)
            port->BSRR |= val << pin;
        else
            port->BRR |= val << pin;
    }
}

uint32_t boardReadWriteSPI(uint8_t numSpi, uint8_t numCs, uint32_t data, uint8_t dataSizeBit)
{
    if(numSpi < configBoard.sizeSPI)
    {
        ChipSelect cs0/* = (ChipSelect){0}*/;
        cs0.numPin = 0;
        cs0.port = 0;
        return softSpiWriteSomeBit(configBoard.masSpi[numSpi], (numCs < configBoard.sizeCS ? configBoard.masCs[numCs] : cs0), data, dataSizeBit);
    }

    return 0;
}

uint32_t boardReadWriteInterface(uint8_t numInterface, uint32_t data, uint8_t dataSizeBit)
{
    if(numInterface < configBoard.sizeInterfase)
    {
        if(configBoard.masInterface[numInterface].perepheria1 == SPI_int)
        {
            return softSpiWriteSomeBit(*(configBoard.masInterface[numInterface].spi), *(configBoard.masInterface[numInterface].cs), data, dataSizeBit);
        }
    }
    return 0;
}

int execUART(uint8_t data)
{
    static _execUART mainUART = NULL;
    static uint8_t buff[SIZE_UART_BUFF] = {0}, pointBuff = 0;
    if(readFlash(ADDRES_PAGE_UART) == 0xFFFFFFFF)//если обработчика не существует то ничего не делаю
    {
        mainUART = NULL;
        return -10;
    }
    if(mainUART == NULL)//если еще не проинициализирован
    {
        mainUART = (_execUART)(*((__IO uint32_t*)(ADDRES_PAGE_UART + 4)));
    }
    return mainUART(&functionBoard, data, buff, &pointBuff);

}
