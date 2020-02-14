#ifndef BOARD_H
#define BOARD_H

//модуль Board основное связующее звено между HAL и soft ds\\выполнгяет и ту и ту функцию

#include <stdint.h>
#include "stm32f1xx.h"

#include "CMD_const.h"
#include "spi_GPIO.h"

#define TIME_OUT_IWDG                       2000        //время watchdog
#define SIZE_PORT_GPIO                      4
#define SPEED_UART                          652800
#define SIZE_UART_BUFF                      16

//memory
//0x08004000
#define PAGE_PARAMETRS_STATION              16      //номер ячейки памяти где храниться структура параметров ConfigBoard
#define ADDRES_PAGE_PARAMETRS_STATION       (MIN_ADRESS + (PAGE_PARAMETRS_STATION) * ONE_PAGE_SIZE)//NOTE пока размер структуры меньше или равен 1024

//0x08004800
#define PAGE_NUM_UART                       18 //FIXME
#define ADDRES_PAGE_UART                    (MIN_ADRESS + (PAGE_NUM_UART) * ONE_PAGE_SIZE)

//0x08005000
#define PAGE_NUM_4K                         22 //FIXME
#define ADDRES_PAGE_PARAMETRS_4K            (MIN_ADRESS + (PAGE_NUM_4K) * ONE_PAGE_SIZE)
#define SIZE_USE_FLASH_4K                   /*(MAX_ADRESS - ADDRES_PAGE_PARAMETRS_4K)*/1024



typedef struct __attribute__((packed)){
    char name[32];
    UsePerepheria perepheria1;
    SoftSpi *spi;
    ChipSelect *cs;
}Interface;

typedef struct __attribute__((packed)){
    char name[32];
    pFunction function;
}FunctionInMemory;

#pragma pack(push, 1)//сужени
typedef struct {
    uint8_t sizeCS;//сколько уже используеться
    uint8_t sizeSPI;
    uint8_t sizeInterfase;
    uint8_t sizeFunction;

    SoftSpi masSpi[SIZE_SPI];
    ChipSelect masCs[SIZE_CS];
    Interface masInterface[SIZE_INTERFACE];
    FunctionInMemory masFunction[SIZE_FUNCTION];
    uint32_t masAddresFunction[SIZE_FUNCTION];//массив аддресов функций используеться для получения информации о функциях
    struct {
        uint8_t start;//0 stop 1 start
        uint32_t taskDelay;
        uint32_t taskPeriod;
        //TODO
//        char argument[MAX_ARGUMENT_SIZE];
//        uint32_t argumentSize;
    }timeFunction[SIZE_FUNCTION];//добавочный массив к функциям для циклического запуска функций

    //state GPIO
    struct {
        uint32_t CRL;
        uint32_t CRH;
//        uint32_t ODR;//в случае с input возможно нужен
    }gpioState[SIZE_PORT_GPIO];

    uint32_t unused;//размер должен быть кратен 4

    uint32_t crc;

}ConfigBoard;//конфигурация платы; то что сохраняеться в памяти о прогрузки интерфейсов
#pragma pack(pop)//разужение
bool whileStructConfigBoard[(sizeof(ConfigBoard) <= 1024) - 1];//проверка размерности
//NOTE сейчас 1024 если станет больше то потребуейть еще 1 ячейка памяти и записывать с 14 ячейки
ConfigBoard configBoard;
FunctionBoard functionBoard;


//TODO add remove function
void initBoard();//начальная функция инициализируюшая структуру conf
bool loadConfig();//прогрузка структуры cpnf из памяти
void saveConfig();//сохранить струкуру в память
void initIWDG(uint16_t tw_ms);//инициализация вачдога
void IWDG_res(void);//ресет вачдога
void delay(uint32_t i);//задержка в цикле
void delay_us(uint16_t us);//задержка пой таймеру в мкс
void delay_ms(uint16_t ms);//задержка пой таймеру в мс
uint32_t calcCrc(void* data, uint32_t size);//расчет контрольной суммы
GPIO_TypeDef *getPort(uint8_t portNum);//получения указателя на порт ножек по номеру
uint8_t getPinPort(GPIO_TypeDef* port);//получения порта ножек по указателю
void init_GPIO_(uint8_t port_pin, uint8_t output, uint8_t mode_val);//инициализация ножек

int _init_function(uint32_t addres, char *name);//инициализация функции (предварительно ее нужно залить в память
//добавить функцию в таск
void functionTimeStart(uint8_t function, uint32_t taskDelay, uint32_t taskPeriod, char *arguments, uint32_t sizeArgument);
//убрать функцию с таска
void functionTimerStop(uint8_t function);

//функция отправления на spi с указание номера spi и номером cs если cs = 255 то не использую его dataSizeBit количество битов
uint32_t boardReadWriteSPI(uint8_t numSpi, uint8_t numCs, uint32_t data, uint8_t dataSizeBit);
//аналогично толдько с интерфейсом dataSizeBit количество битов
uint32_t boardReadWriteInterface(uint8_t numInterface, uint32_t data, uint8_t dataSizeBit);

int execUART(uint8_t data);//обработчик UART так же записываеться в в память как и другие вызываеться из прерывания NOTE по возможности переместить вызов из прерывания



#endif // BOARD_H
