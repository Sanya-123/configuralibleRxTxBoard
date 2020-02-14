#ifndef CMD_CONST_H
#define CMD_CONST_H

#include <stdint.h>
#include <stdbool.h>
#include "FunctionOnBoard.h"


//размеры
#define MAX_SIZE_NAME                   32              //максимальный размер всех имер
#define MAX_ARGUMENT_SIZE               32              //максимальное размер аргументов для функции

#define SIZE_SPI                        2
#define SIZE_CS                         10
#define SIZE_INTERFACE                  10
#define SIZE_FUNCTION                   10


#define KEY_ON                          1
#define KEY_OFF                         0

#define PARAM_READ                      1
#define PARAM_WRITE                     2


//CMD
#define INIT_PIN                        10              //инициализация 1 ножки example 10 2 0x0A(GPIOA,pin10) 0x0(input) 0x21(mode 2[0,1,2,3],val 1)
#define WRITE_GPIO                      11              //запись с ножи example 11 2 0x1B(запись GPIOB,pin11) 1;
#define READ_GPIO                       12              //чтение с ножи example 12 2 0x0A 0 0(чтение GPIOA,pin10);
#define INIT_SPI                        13              //инициализация SPI example: 13 2 0x12(CLK = GPIOB,pin2) 0x0B(MOSI = GPIOA,pin11) 0x24(MISO = GPIOC,pin4)
#define INIT_CS                         14              //иинциализация cs erxample 14 2 0x18(CS = GPIOB,pin8)
#define INIT_INTERFACE                  15              //инициализация интерфейса example 15 2 1(SPI) 3(num spi 3) 17(num cs 17) [32 byte name]
#define WARITE_SPI                      16              //отправка дынных на SPI example 16 2 2(SPI = 2) 3(CS = 3(255 unused cs)) 17 18 99 55 (DATA = 17 18 99 55)
#define WARITE_INTERFACE                17              //отправка дынных на интерфейс example 17 2 2(interface = 2) 17 18 99 55 (DATA = 17 18 99 55)
#define REMOVE_SPI                      18              //удаление spi example 18 2 5(remove spi 5)
#define REMOVE_CS                       19              //удаление cs example 19 2 8(remove cs 8)
#define REMOVE_INTERFAVE                20              //удаление intefrace example 20 2 2(remove intrerfave 2)
#define GET_SIZE_USE                    21              //количество cs example 21 2 0 0(CS) 0(SPI) 0(Interface) 0(function)
#define GET_INFO_CS                     22              //NOTE команда выполняеться в паре ;example 22 2 9(num cs 9);delay(~100ms);INIT_CS 1 0x18(CS = GPIOB,pin8)
#define GET_INFO_SPI                    23              //NOTE команда выполняеться в паре ;example 23 2 2(num spi 2);delay(~100ms);INIT_SPI 1 0x12(CLK = GPIOB,pin2) 0x0B(MOSI = GPIOA,pin11) 0x24(MISO = GPIOC,pin4)
#define GET_INFO_INTERFACE              24              //NOTE команда выполняеться в паре ;example 23 2 9(num interface 9);delay(~100ms);INIT_INTERFACE 1 1(SPI) 3(num spi 3) 17(num cs 17) [32 byte name]
#define WRITE_FUNCTION                  25              //записсать функцию в память example 25 2 0x00 0x10 0x00 0x08 (0x08001000 addres write) [0-1024 data] за 1 посылку допускаеться 1024 байт если больше то в несколько посылок
#define INIT_FUNCTION                   26              //проинициализировать функцию example 26 2 0x00 0x10 0x00 0x08 (0x08001000 addres) [32 byte name]
#define DO_FUNCTION                     27              //делать функцию examole 27 2 7(сделать функцию 7) [32 byte argyment] 15 00 00 00(фактический размер аргументов 15)
#define REMOVE_FUNCTION                 28              //удалить функцию example 28 2 7( удалить функцию 7)
#define FUNCTION_TIMER                  29              //циклически запускать функцию example 29 2 5(функцию 5) 0x00 0x04 0x00 0x00(0x00000400 taskDelay) 0x44 0x04 0x00 0x00(0x00000444 taskPeriod) [32 byte argyment] 15  00 00 00 (фактический размер аргументов 15)
#define FUNCTION_TIMER_STOP             30              //отключение таймера example 30 2 6 (остановить функцию 6)
#define GET_INFO_FUNCTION               31              //NOTE команда выполняеться в паре ;example 31 2 7(num function 7);delay(~100ms);INIT_FUNCTION 1 0x18(CS = GPIOB,pin8) 0x00 0x10 0x00 0x08 (0x08001000 addres) [32 byte name]
#define SAVE_CONFIG                     50              //сохранение настроек example 50 2 1



#define FEE_ERASE_PAGE                  158		//w 2 стирание калибровочной таблицы в FEE
#define FEE_WRITE			159		//rw 4 запись данных формат смотри в fee_write_pack_t
#define FEE_READ			160		//r 4 чтение флешки по 2 байта, начиная с выбранного адреса
//#define FEE_WRITE_ENABLE	161		//w 1 запись данных формат смотри в fee_write_pack_t
#define FEE_ERASE_ALL_PAGE  162     //w 1 стереть всю калибровку
//#define FEE_ERR				162		// на случай возникновения ошибок при записи (необходимость в параметре сомнительная)
#define BOARD_ID			163		// r 1+ идентификатор радиочасти

/* GPIO
 *        _
 *      7|PORT:
 *       |     0 - A
 *       |     1 - B
 *       |     2 - C
 *       |     ...
 * 1 byte|     F - F
 *       |PIN:
 *       |     0 - 0
 *       |     1 - 1
 *       |     ...
 *      0|_    15 - 15
 *
 *        _
 *      7|in/out:
 * 1 byte|      0 - input
 *      0|_     1 - output
 *
 *        _
 *      7|mode:
 *       | input:
 *       |      0 -
 *       |      1 - Floating
 *       |      2 - InputPullUp/InputPullDown
 *       | output:
 *       |      0 - pushpull
 *       |      1 - openDrain
 * 1 byte|val:
 *       | input:
 *       |      0 - pulldown
 *       |      1 - pullup
 *       | output:
 *       |      0 - 0
 *      0|_     1 - 1
 */

#pragma pack(push, 1)//сужени
typedef struct /*__attribute__((packed)) */{
    //
    uint8_t sizeCS;//сколько уже используеться
    uint8_t sizeSPI;
    uint8_t sizeInterfase;
    uint8_t sizeFunction;

    uint32_t initGPIO;
    uint16_t readWriteGPIO;

    uint32_t initSPI;
    uint8_t initCS;
    struct{
//        uint32_t initInterface;
        uint8_t interface;
        uint8_t numSPI;
        uint8_t numCS;
        char name[MAX_SIZE_NAME];
    } initInterface;

    struct{
        uint8_t spi;
        uint8_t cs;
        uint32_t data;
    }writeSpi;

    struct{
        uint8_t interface;
        uint32_t data;
    }writeInterface;

    uint8_t removeSpi;
    uint8_t removeCs;
    uint8_t removeInterface;

    uint32_t writeFunction;
    struct{
        uint32_t addres;
        char name[MAX_SIZE_NAME];
    } initFunction;
    struct {
        uint8_t function;
        char arguments[MAX_ARGUMENT_SIZE];
        uint32_t sizeArgument;
    }doFunction;
    uint8_t removeFunction;

    struct {
        uint8_t function;
        uint32_t taskDelay;//ms
        uint32_t taskPeriod;//ms
        char arguments[MAX_ARGUMENT_SIZE];
        uint32_t sizeArgument;
    }functionTimer;

    uint8_t stopTimerFunction;

    uint8_t saveConfig;

    //get info
    uint8_t getInfoCS;
    uint8_t getInfoSPI;
    uint8_t getInfoInterface;
    uint8_t getInfoFunction;

//    int8_t temperature;

//    //board spetific freq
//    uint32_t spanFreqMin;//begin diapazone
//    uint32_t spanFreqMax;//end diapazone
//    uint32_t spanFreqStep;//step diapazone
//    uint32_t spanFreqIF;//куакаета частота

//    //board spetific power
//    int32_t powInCalibr;
//    int32_t powOutMin;
//    int32_t powOutMax;
//    int32_t powOutCorrection;
//    int32_t powReal;
//    uint32_t vgaCorrection;

    //boiard spetific
    uint16_t flashEarsePage;//отчистка странички
    struct {
        uint16_t addres;
        uint16_t data;
    }flashMK;
//    uint32_t flashMK;//чтение/запись какихто данных с моего флеша

    uint32_t boadrID;

    //key
//    uint8_t key_RxTx;
//    uint8_t key_bypassIF;

    //flag
//    uint8_t flag_loadSyntRx;//флаг прогрузки синтезатора
//    uint8_t flag_loadSyntRx_2;//флаг прогрузки синтезатора
//    uint8_t flag_loadSyntTx;//флаг прогрузки синтезатора
//    uint8_t flag_hwUARTcontrolEN;//флаг EN UART
    uint8_t earseFlash;


} Parametrs;
Parametrs parametrs;//структура протокола общения
#pragma pack(pop)//разужение

typedef struct {//структура bool_ов показывающая кокой параметр изменился
    uint8_t theriseChange;//есть изменения
    uint32_t buteCome;//заложено для команд которые могут обрыбытывать оазное количество байт


    bool sizeSPI;
    bool sizeCS;
    bool sizeInterfase;

    bool initGPIO;
    bool readWriteGPIO;

    bool initSPI;
    bool initCS;
    bool initInterface;

    bool writeSpi;
    bool writeInterface;

    bool removeSpi;
    bool removeCs;
    bool removeInterface;

    bool writeFunction;
    bool initFunction;
    bool doFunction;
    bool removeFunction;
    bool functionTimer;
    bool stopTimerFunction;

    bool saveConfig;

    //get info
    bool getInfoCS;
    bool getInfoSPI;
    bool getInfoInterface;
    bool getInfoFunction;


//    //board spetific freq
//    bool spanFreqMin;//begin diapazone
//    bool spanFreqMax;//end diapazone
//    bool spanFreqStep;//step diapazone
//    bool spanFreqIF;//куакаета частота

//    //board spetific power
//    bool powInCalibr;
//    bool powOutMin;
//    bool powOutMax;
//    bool powOutCorrection;
//    bool powReal;
//    bool vgaCorrection;

    //boiard spetific
    bool flashEarsePage;
    bool flashMK;//чтение какихто данных с моего флеша
    bool boadrID;

    //key
//    bool key_RxTx;
//    bool key_bypassIF;

    //flag
//    bool flag_loadSyntRx;//флаг прогрузки синтезатора
//    bool flag_loadSyntRx_2;//флаг прогрузки синтезатора
//    bool flag_loadSyntTx;//флаг прогрузки синтезатора
//    bool flag_hwUARTcontrolEN;//флаг EN UART
    bool earseFlash;

} ParametrsChange;//структура указывающая изменилось
ParametrsChange parametrsChange;

//в пямяти контроллера заняты последнии 5к байт в пермом данная структура в остальных 4 таблица компенсаций

//typedef struct __attribute__((packed)){//структура параметров платы которая будет храниться по флеш

//    //board spetific freq
//    uint32_t spanFreqMin;//begin diapazone
//    uint32_t spanFreqMax;//end diapazone
//    uint32_t spanFreqStep;//step diapazone
//    uint32_t spanFreqIF;//куакаета частота

//    //board spetific power
//    int32_t powInCalibr;
//    int32_t powOutMin;
//    int32_t powOutMax;
//    int32_t powOutCorrection;
//    int32_t powReal;
//    uint32_t vgaCorrection;

//    //boiard spetific
//    uint32_t boadrID;

//    //контрольный суммы
//    uint32_t crc4k;//контрольная сумма таблицы компенсации
//    uint32_t crcParametrs;//контрольная сумма данной структуры

//}ParametrsStation;

//ParametrsStation parametrsStation;

#endif // CMD_CONST_H
