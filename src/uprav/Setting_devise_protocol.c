#include "Setting_devise.h"
#include "xprintf.h"
//#include "Wake.h"
#include <stdbool.h>
#include <unistd.h>

//для инициализации
#ifdef USE_RTOS
    #include "FreeRTOS.h"
    #include "task.h"
    //#include "timers.h"
    //#include "semphr.h"
#else
    #include "sheduler.h"
#endif

typedef enum {
    WaytCmd = 0,
    WaytReadWrite,
    WaytDataWrite,
    WaytDataRead
}StateSpi;


bool flagSpi = false;


void USART2_IRQHandler(void)//TODO
{
    if ((USART2->SR & (USART_SR_NE | USART_SR_FE | USART_SR_PE | USART_SR_ORE)) == 0) //проверяем нет ли ошибок
    {
//        bufferSetData(&bufferReadUART, USART2->DR);//по приходу данных записываб их в буффер
        uint8_t data = USART2->DR;
        execUART(data);
    }
    else
    {
        (void)USART2->DR;                                 // вычитываем данные и на всякий случай
        USART2->SR &= ~USART_SR_RXNE;               // ещё и сбрасываем флаг прерывания
    }
}
//void USART1_IRQHandler(void)//TODO
//{
//    if ((USART1->SR & (USART_SR_NE | USART_SR_FE | USART_SR_PE | USART_SR_ORE)) == 0) //проверяем нет ли ошибок
//    {
////        bufferSetData(&bufferReadUART, USART2->DR);//по приходу данных записываб их в буффер
//        uint8_t data =  USART1->DR;
//        data = data;
//        (void)data;
//    }
//    else
//    {
//        (void)USART1->DR;                                 // вычитываем данные и на всякий случай
//        USART1->SR &= ~USART_SR_RXNE;               // ещё и сбрасываем флаг прерывания
//    }
//}

void wrireDatInterface(uint8_t data)//универсальная фкнкция для отправки данных в ней устанавливаеться интерфейс отправаки
{
//    writeUartData(data);
    //NOTE: if spi is free
//    if (spi_isFree())
//    {
        spi_write(data);
//    }
}


//#define SIZE_SPI_RX_BUF                 64
//uint8_t hwSpiRxBuf[SIZE_SPI_RX_BUF];//буфер spi
//uint8_t hwSpiRxPoint = 0;//положение буфера spi

StateSpi state = WaytCmd;//состояния машины конечных состояний
uint8_t cmd = 0;
uint32_t iterationWaytCMD = 0;//счетчик ожидания команды
uint32_t sizeCMD = 0;//размер команды для записи/отправления
int32_t offset = -1;

void resetMachineState()//сброс состояний по CS или другим осбоятельствам
{
#if USE_TX_BUFFER
    spi_flush();//отчищаю буфер spi
#endif
    state = WaytCmd;
    cmd = 0;
    iterationWaytCMD = 0;
    sizeCMD = 0;
    offset = -1;
//    hwSpiRxPoint = 0;
}

uint32_t getSizeCmd(uint8_t cmd)//получения ожидаемого размера
{
    if((cmd == WRITE_GPIO) || (cmd == READ_GPIO))
        return 2;
    if((cmd >= INIT_PIN) && (cmd <= INIT_SPI))
        return 3;
    if((cmd == INIT_CS))
        return 1;
    if(cmd == INIT_INTERFACE)
        return 3 + MAX_SIZE_NAME;
    if(cmd == INIT_FUNCTION)
        return 4 + MAX_SIZE_NAME;
    if(cmd == WARITE_SPI)
        return 6;
    if(cmd == WARITE_INTERFACE)
        return 5;
    if((cmd == REMOVE_INTERFAVE) || (cmd == REMOVE_SPI) || (cmd == REMOVE_CS))
        return 1;
    if((cmd == GET_SIZE_USE))
        return 5;

    if((cmd == WRITE_FUNCTION))
        return SIZE_USE_FLASH_4K;
    if((cmd == DO_FUNCTION))
        return 1 + MAX_ARGUMENT_SIZE + 4;
    if((cmd == FUNCTION_TIMER))
        return 9 + MAX_ARGUMENT_SIZE + 4;
    if((cmd == FUNCTION_TIMER_STOP))
        return 1;

    if(cmd == SAVE_CONFIG)
        return 1;

    if((cmd == GET_INFO_CS) || (cmd == GET_INFO_SPI) || (cmd == GET_INFO_INTERFACE) || (cmd == GET_INFO_FUNCTION))
        return 1;

    if((cmd == FEE_WRITE) || (cmd == FEE_READ))//при этой команде возможно до SIZE_USE_FLASH_4K байт
        return SIZE_USE_FLASH_4K;

    if((cmd == FEE_ERASE_PAGE))
        return 2;
    if((cmd == FEE_ERASE_ALL_PAGE))
        return 1;
    if(cmd == BOARD_ID)//FIXME возможно 1 байтные команды
        return 4;

    return 0;
}

/**
 * @brief getOfsetData
 * @param cmd - пришедшая команда
 * @return - если команда не корректная то -1 иначе аддрес места(в структуре) где находиться этот парасетр
 */

//получить смещение y в Parametrs по команде x
#define GET_OFFSE_PARAMERT(x,y)          case x:offset = offsetof(Parametrs, y);break;

int32_t getOfsetData(uint16_t cmd)
{
    int32_t offset = -1;

//    offsetof(EpromStruct, masNPT1[0]);
    switch (cmd) {
        GET_OFFSE_PARAMERT(INIT_PIN, initGPIO);
        GET_OFFSE_PARAMERT(WRITE_GPIO, readWriteGPIO);
        GET_OFFSE_PARAMERT(READ_GPIO, readWriteGPIO);
        GET_OFFSE_PARAMERT(INIT_SPI, initSPI);
        GET_OFFSE_PARAMERT(INIT_CS, initCS);
        GET_OFFSE_PARAMERT(INIT_INTERFACE, initInterface);

        GET_OFFSE_PARAMERT(WARITE_SPI, writeSpi);
        GET_OFFSE_PARAMERT(WARITE_INTERFACE, writeInterface);

        GET_OFFSE_PARAMERT(REMOVE_CS, removeCs);
        GET_OFFSE_PARAMERT(REMOVE_SPI, removeSpi);
        GET_OFFSE_PARAMERT(REMOVE_INTERFAVE, removeInterface);

        GET_OFFSE_PARAMERT(GET_SIZE_USE, sizeCS);

        GET_OFFSE_PARAMERT(WRITE_FUNCTION, writeFunction);
        GET_OFFSE_PARAMERT(INIT_FUNCTION, initFunction);
        GET_OFFSE_PARAMERT(DO_FUNCTION, doFunction);
        GET_OFFSE_PARAMERT(REMOVE_FUNCTION, removeFunction);
        GET_OFFSE_PARAMERT(FUNCTION_TIMER, functionTimer);
        GET_OFFSE_PARAMERT(FUNCTION_TIMER_STOP, stopTimerFunction);

        GET_OFFSE_PARAMERT(SAVE_CONFIG, saveConfig);

        GET_OFFSE_PARAMERT(GET_INFO_CS, getInfoCS);
        GET_OFFSE_PARAMERT(GET_INFO_SPI, getInfoSPI);
        GET_OFFSE_PARAMERT(GET_INFO_INTERFACE, getInfoInterface);
        GET_OFFSE_PARAMERT(GET_INFO_FUNCTION, getInfoFunction);

    //*****board param*****
        GET_OFFSE_PARAMERT(BOARD_ID, boadrID);
    //**********
    //*****flash*****
        //ффункция чтения
        GET_OFFSE_PARAMERT(FEE_READ, flashMK);
        GET_OFFSE_PARAMERT(FEE_WRITE, flashMK);
        GET_OFFSE_PARAMERT(FEE_ERASE_PAGE, flashEarsePage);
        GET_OFFSE_PARAMERT(FEE_ERASE_ALL_PAGE, earseFlash);
    //**********
    //*****key*****
//******
    default:
        break;
    }

    return offset;
}

//выставить параметр y о каманде x на true
#define SET_TRUE_PARAMERT(x,y)          case x:parametrsChange.y=true;break;

void setChangeCMD(uint8_t cmd)
{
    parametrsChange.theriseChange++;
    switch (cmd) {
        SET_TRUE_PARAMERT(INIT_PIN, initGPIO);
        SET_TRUE_PARAMERT(WRITE_GPIO, readWriteGPIO);
        SET_TRUE_PARAMERT(INIT_SPI, initSPI);
        SET_TRUE_PARAMERT(INIT_CS, initCS);
        SET_TRUE_PARAMERT(INIT_INTERFACE, initInterface);

        SET_TRUE_PARAMERT(WARITE_SPI, writeSpi);
        SET_TRUE_PARAMERT(WARITE_INTERFACE, writeInterface);

        SET_TRUE_PARAMERT(REMOVE_CS, removeCs);
        SET_TRUE_PARAMERT(REMOVE_SPI, removeSpi);
        SET_TRUE_PARAMERT(REMOVE_INTERFAVE, removeInterface);

        SET_TRUE_PARAMERT(INIT_FUNCTION, initFunction);
        SET_TRUE_PARAMERT(DO_FUNCTION, doFunction);
        SET_TRUE_PARAMERT(REMOVE_FUNCTION, removeFunction);
        SET_TRUE_PARAMERT(FUNCTION_TIMER, functionTimer);
        SET_TRUE_PARAMERT(FUNCTION_TIMER_STOP, stopTimerFunction);

        SET_TRUE_PARAMERT(SAVE_CONFIG, saveConfig);

        SET_TRUE_PARAMERT(GET_INFO_CS, getInfoCS);
        SET_TRUE_PARAMERT(GET_INFO_SPI, getInfoSPI);
        SET_TRUE_PARAMERT(GET_INFO_INTERFACE, getInfoInterface);
        SET_TRUE_PARAMERT(GET_INFO_FUNCTION, getInfoFunction);

    //*****board param*****
        SET_TRUE_PARAMERT(BOARD_ID, boadrID);
    //**********
    //*****flash*****
        SET_TRUE_PARAMERT(FEE_ERASE_PAGE, flashEarsePage);
        SET_TRUE_PARAMERT(FEE_ERASE_ALL_PAGE, earseFlash);
    //**********
//******
    default://если не произошло изменений в косандах которые можно е\менять
        parametrsChange.theriseChange--;
        break;
    }
}

inline void spi_interupt(uint8_t data)
{
    /* 0 - параметр
     * 1 - 1 на чтение 2 на запись
     * 2...N-1 - данные
     *
     * N для каждого параметра свой
     */

//    if(hwSpiRxPoint < (SIZE_SPI_RX_BUF - 1))
//        hwSpiRxBuf[hwSpiRxPoint++] = data;//добавляю данные в буфер

    //OK
    switch (state) {
    case WaytReadWrite://состояние ожидания rw
        //NOTE это самая приоритетная задача и она должна выполнять как можно быстрее чтобы успеть отправить данные на SPI
        //поэтому больщинство задачь решаються по получению команды а сдесть только 1 проверка
        if(data == PARAM_READ)//пришол байт что нужно прочитать
        {//NOTE сдесь нужно минимальное количество команд чтобы происходило максимаотно быстро

            uint8_t *pointDataForSend = ((uint8_t*)(&parametrs) + offset);//получаю точку в структуре с которой считывать данные
            spi_writeMas(pointDataForSend, sizeCMD);
            state = WaytDataRead;
        }
        else if(data == PARAM_WRITE)//пришол байт что нужно записать
        {
            state = WaytDataWrite;
        }
        else//пришол не коректный байт
        {
            resetMachineState();
        }
        break;
    case WaytCmd://состояние ожидания команды
        //OK
        cmd = data;
        sizeCMD = iterationWaytCMD = getSizeCmd(cmd);//получаю размер данной команды
        if(iterationWaytCMD != 0)//если размер корректный то иду на состояние ожидания rw
        {
            state = WaytReadWrite;
            offset = getOfsetData(cmd);
            if(offset < 0)
            {
                resetMachineState();
            }
        }
        else
        {
            resetMachineState();
        }
        break;
    case WaytDataWrite://состояния записи в память
        //OK
        iterationWaytCMD--;
        //bcrk.xz.obt rjvfyls
        if((cmd == FEE_WRITE) || (cmd == FEE_READ))//если команды связаные со флешкой
        {
            if(iterationWaytCMD < (SIZE_USE_FLASH_4K - 2))
            {//NOTE сдесь нужно минимальное количество команд чтобы происходило максимаотно быстро
                if((cmd == FEE_READ))//есть 1 исключение когда нужно читать из флешки
                {
                    spi_write(*((__IO uint8_t *)(ADDRES_PAGE_PARAMETRS_STATION + parametrs.flashMK.addres + SIZE_USE_FLASH_4K - iterationWaytCMD - 3)));
                }
                else if((cmd == FEE_WRITE))//есть 1 исключение когда нужно читать из флешки
                {
                    static uint32_t dataFl = 0;//сдесь требуеться сначало копить 2 байта потом записывать
    //                spi_write(*((__IO uint8_t *)(ADDRES_PAGE_PARAMETRS_4K + parametrs.flashMK.addres + iterationWaytCMD - 2)));
                    if(iterationWaytCMD%2)
                        dataFl = data;
                    else
                    {
                        dataFl |= data << 8;
                        writeFlash(((uint32_t)dataFl) | (0xFFFF << 16), ADDRES_PAGE_PARAMETRS_STATION + parametrs.flashMK.addres/2*2 + SIZE_USE_FLASH_4K - iterationWaytCMD - 4);
                        dataFl = 0;
                    }
                }
                else
                    *(((uint8_t*)(&parametrs) + offset) + sizeCMD - iterationWaytCMD - 1) = data;//записываю в структуру данные
            }
            else
                *(((uint8_t*)(&parametrs) + offset) + sizeCMD - iterationWaytCMD - 1) = data;//записываю в структуру данные
        }
        else if(cmd == READ_GPIO)//если команда читать значение ножки
        {
            if(iterationWaytCMD == (sizeCMD - 1))//если уже пришла команда
            {
                GPIO_TypeDef *port = getPort(parametrs.readWriteGPIO >> 4);
                spi_write((port->IDR & (1 << (parametrs.readWriteGPIO & 0x0F))) != 0);
            }
            else
            {
                *(((uint8_t*)(&parametrs) + offset) + sizeCMD - iterationWaytCMD - 1) = data;//записываю в структуру данные
            }
        }
        else if(cmd == WRITE_FUNCTION)
        {
            if(iterationWaytCMD < (SIZE_USE_FLASH_4K - 4))
            {
                static uint32_t dataFl = 0;//сдесь требуеться сначало копить 2 байта потом записывать
//                spi_write(*((__IO uint8_t *)(ADDRES_PAGE_PARAMETRS_4K + parametrs.flashMK.addres + iterationWaytCMD - 2)));
                if(iterationWaytCMD%2)
                    dataFl = data;
                else
                {
                    dataFl |= data << 8;
                    writeFlash(((uint32_t)dataFl) | (0xFFFF << 16), parametrs.writeFunction/2*2 + ADDRES_PAGE_PARAMETRS_STATION - iterationWaytCMD - 6);
                    dataFl = 0;
                }
            }
            else
            {
                *(((uint8_t*)(&parametrs) + offset) + sizeCMD - iterationWaytCMD - 1) = data;//записываю в структуру данные
            }
        }
        else if(cmd == GET_SIZE_USE)
        {
            if(iterationWaytCMD == (sizeCMD - 1))
            {
                spi_write(parametrs.sizeCS);
                spi_write(parametrs.sizeSPI);
                spi_write(parametrs.sizeInterfase);
                spi_write(parametrs.sizeFunction);
            }
        }
        else//все команды кроме исключающих
            *(((uint8_t*)(&parametrs) + offset) + sizeCMD - iterationWaytCMD - 1) = data;//записываю в структуру данные
        //..

        if(iterationWaytCMD == 0)
        {
            state = WaytCmd;
            setChangeCMD(cmd);
        }
        break;
    case WaytDataRead://жду пока все байты отправяться
        //OK
        iterationWaytCMD--;
        if(iterationWaytCMD != 0)//если счетчик не достиг 0 то еще не все прочитанно
        {
            break;
        }
    default:
        //отчистка spi и возвращение в исходное положение
        resetMachineState();
        break;
    }
}

inline void cs_interupt()
{
    //по приходу команд на чтение мне нужно назад чтото отправить и нужно учесть сли я хочу отправляю 4 байта а ждут только 2
    //поятому я по прерыванию CS ресетаю состояние
    resetMachineState();

    if(cmd == FEE_WRITE)//если закончил пеисать фо флешку то проверяю контрольную сумму
    {
        updateCrcMemory();
    }
}
