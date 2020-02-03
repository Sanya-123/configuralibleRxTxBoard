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
        (void)USART2->DR;
    }
    else
    {
        (void)USART2->DR;                                 // вычитываем данные и на всякий случай
        USART2->SR &= ~USART_SR_RXNE;               // ещё и сбрасываем флаг прерывания
    }
}

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
    if((cmd >= INIT_PIN) && (cmd <= INIT_SPI_CMD))
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
        return 6;

    if((cmd == WRITE_FUNCTION))
        return SIZE_USE_FLASH_4K;
    if((cmd == DO_FUNCTION))
        return 1 + MAX_ARGUMENT_SIZE + 4;
    if((cmd == FUNCTION_TIMER))
        return 9 + MAX_ARGUMENT_SIZE + 4;
    if((cmd == FUNCTION_TIMER_STOP))
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

int32_t getOfsetData(uint16_t cmd)
{
    int32_t offset = -1;

//    offsetof(EpromStruct, masNPT1[0]);
    switch (cmd) {
    case INIT_PIN:
        offset = offsetof(Parametrs, initGPIO);
        break;
    case WRITE_GPIO:
        offset = offsetof(Parametrs, readWriteGPIO);
        break;
    case READ_GPIO:
        offset = offsetof(Parametrs, readWriteGPIO);
        break;
    case INIT_SPI_CMD:
        offset = offsetof(Parametrs, initSPI);
        break;
    case INIT_CS:
        offset = offsetof(Parametrs, initCS);
        break;
    case INIT_INTERFACE:
        offset = offsetof(Parametrs, initInterface);
        break;

    case WARITE_SPI:
        offset = offsetof(Parametrs, writeSpi);
        break;
    case WARITE_INTERFACE:
        offset = offsetof(Parametrs, writeInterface);
        break;

    case REMOVE_CS:
        offset = offsetof(Parametrs, removeCs);
        break;
    case REMOVE_SPI:
        offset = offsetof(Parametrs, removeSpi);
        break;
    case REMOVE_INTERFAVE:
        offset = offsetof(Parametrs, removeInterface);
        break;

    case GET_SIZE_USE:
        offset = offsetof(Parametrs, sizeSPI);
        break;

    case WRITE_FUNCTION:
        offset = offsetof(Parametrs, writeFunction);
        break;
    case INIT_FUNCTION:
        offset = offsetof(Parametrs, initFunction);
        break;
    case DO_FUNCTION:
        offset = offsetof(Parametrs, doFunction);
        break;
    case REMOVE_FUNCTION:
        offset = offsetof(Parametrs, removeFunction);
        break;
    case FUNCTION_TIMER:
        offset = offsetof(Parametrs, functionTimer);
        break;
    case FUNCTION_TIMER_STOP:
        offset = offsetof(Parametrs, stopTimerFunction);
        break;

    //*****board param*****
    case BOARD_ID:
        offset = offsetof(Parametrs, boadrID);
        break;
    //**********
    //*****flash*****
    case FEE_READ:
        //ффункция чтения
    case FEE_WRITE:
        offset = offsetof(Parametrs, flashMK);
        break;
    case FEE_ERASE_PAGE:
        offset = offsetof(Parametrs, flashEarsePage);
        break;
    case FEE_ERASE_ALL_PAGE:
        offset = offsetof(Parametrs, earseFlash);
        break;
    //**********
    //*****key*****
//******
    default:
        break;
    }

    return offset;
}

void setChangeCMD(uint8_t cmd)
{
    parametrsChange.theriseChange++;
    switch (cmd) {
    case INIT_PIN:
        parametrsChange.initGPIO = true;
        break;
    case WRITE_GPIO:
        parametrsChange.readWriteGPIO = true;
        break;
    case INIT_SPI_CMD:
        parametrsChange.initSPI = true;
        break;
    case INIT_CS:
        parametrsChange.initCS = true;
        break;
    case INIT_INTERFACE:
        parametrsChange.initInterface = true;
        break;

    case WARITE_SPI:
        parametrsChange.writeSpi = true;
        break;
    case WARITE_INTERFACE:
        parametrsChange.writeInterface = true;
        break;

    case REMOVE_CS:
        parametrsChange.removeCs = true;
        break;
    case REMOVE_SPI:
        parametrsChange.removeSpi = true;
        break;
    case REMOVE_INTERFAVE:
        parametrsChange.removeInterface = true;
        break;

    case INIT_FUNCTION:
        parametrsChange.initFunction = true;
        break;
    case DO_FUNCTION:
        parametrsChange.doFunction = true;
        break;
    case REMOVE_FUNCTION:
        parametrsChange.removeFunction = true;
        break;
    case FUNCTION_TIMER:
        parametrsChange.functionTimer = true;
        break;
    case FUNCTION_TIMER_STOP:
        parametrsChange.stopTimerFunction = true;
        break;


    //*****board param*****
    case BOARD_ID:
        parametrsChange.boadrID = true;
        break;
    //**********
    //*****flash*****
//    case FEE_WRITE:
//        parametrsChange.flashMK = true;
//        break;
    case FEE_ERASE_PAGE:
        parametrsChange.flashEarsePage = true;
        break;
    case FEE_ERASE_ALL_PAGE:
        parametrsChange.earseFlash = true;
        break;
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
        if((cmd == FEE_WRITE) || (cmd == FEE_READ))//если команды связаные со флешкой
        {
            if(iterationWaytCMD < (SIZE_USE_FLASH_4K - 2))
            {//NOTE сдесь нужно минимальное количество команд чтобы происходило максимаотно быстро
                if((cmd == FEE_READ))//есть 1 исключение когда нужно читать из флешки
                {
                    spi_write(*((__IO uint8_t *)(ADDRES_PAGE_PARAMETRS_4K + parametrs.flashMK.addres + SIZE_USE_FLASH_4K - iterationWaytCMD - 3)));
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
                        writeFlash(((uint32_t)dataFl) | (0xFFFF << 16), ADDRES_PAGE_PARAMETRS_4K + parametrs.flashMK.addres/2*2 + SIZE_USE_FLASH_4K - iterationWaytCMD - 4);
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
                    writeFlash(((uint32_t)dataFl) | (0xFFFF << 16), parametrs.writeFunction/2*2 + SIZE_USE_FLASH_4K - iterationWaytCMD - 6);
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
                spi_write(parametrs.sizeCS);
                spi_write(parametrs.sizeCS);
            }
        }
        else
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
