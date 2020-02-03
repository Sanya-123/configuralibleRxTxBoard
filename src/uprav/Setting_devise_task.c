#include "Setting_devise.h"
#include "xprintf.h"
#include <stdbool.h>
#include <string.h>

//для инициализации
#ifdef USE_RTOS
    #include "FreeRTOS.h"
    #include "task.h"
    //#include "timers.h"
    //#include "semphr.h"
#else
    #include "sheduler.h"
#endif

int _initCS(uint8_t pinPort)
{
    if(parametrs.sizeCS < SIZE_CS)
    {
        init_GPIO_(pinPort, 1, 0x01);
        masCs[parametrs.sizeCS].numPin = pinPort & 0x0F;
        masCs[parametrs.sizeCS].port = getPort(pinPort >> 4);
        parametrs.sizeCS++;
        return 0;
    }
    return -1;
}

int _initSPI(uint8_t pinPortCLK, uint8_t pinPortMOSI, uint8_t pinPortMSIO)
{
    if(parametrs.sizeSPI < SIZE_SPI)
    {
        masSpi[parametrs.sizeSPI].numPin_CLK = pinPortCLK & 0x0F;
        masSpi[parametrs.sizeSPI].numPin_MOSI = (pinPortMOSI) & 0x0F;
        masSpi[parametrs.sizeSPI].numPin_MISO = (pinPortMSIO) & 0x0F;

        masSpi[parametrs.sizeSPI].portPin_CLK = getPort((pinPortCLK >> 4) & 0x0F);
        masSpi[parametrs.sizeSPI].portPin_MOSI = getPort((pinPortMOSI >> 4) & 0x0F);
        masSpi[parametrs.sizeSPI].portPin_MISO = getPort((pinPortMSIO >> 4) & 0x0F);

        init_SPI_GPIO(masSpi[parametrs.sizeSPI]);
        parametrs.sizeSPI++;
        return 0;
    }
    return -1;
}

int _initInterface(UsePerepheria per, uint8_t spi, uint8_t cs, char *name)
{
    if(parametrs.sizeInterfase < SIZE_INTERFACE)
    {

        masInterface[parametrs.sizeInterfase].perepheria1 = per;
        if(masInterface[parametrs.sizeInterfase].perepheria1 == SPI_int)
        {
            uint8_t numSpi = spi;
            if(numSpi < parametrs.sizeSPI)//если наддый spi существует
            {
                masInterface[parametrs.sizeInterfase].spi = &masSpi[numSpi];
                masInterface[parametrs.sizeInterfase].cs = NULL;
                uint8_t numCs = cs;
                if(numCs < parametrs.sizeCS)
                {
                    masInterface[parametrs.sizeInterfase].cs = &masCs[numCs];
                }
                strncpy(masInterface[parametrs.sizeInterfase].name, name, MAX_SIZE_NAME);
                parametrs.sizeInterfase++;
                return 0;
            }
        }
    }
    return -1;
}

void initSetting()
{
    //init parametrs
    parametrsChange.theriseChange = 0;


    parametrsChange.spanFreqMin = false;
    parametrsChange.spanFreqMax = false;
    parametrsChange.spanFreqStep = false;
    parametrsChange.spanFreqIF = false;
    parametrsChange.powInCalibr = false;
    parametrsChange.powOutMin = false;
    parametrsChange.powOutMax = false;
    parametrsChange.powOutCorrection = false;
    parametrsChange.powReal = false;
    parametrsChange.vgaCorrection = false;
    parametrsChange.flashMK = false;
    parametrsChange.boadrID = false;
    parametrsChange.key_RxTx = false;
    parametrsChange.key_bypassIF = false;
    parametrsChange.flag_loadSyntRx = false;
    parametrsChange.flag_loadSyntRx_2 = false;
    parametrsChange.flag_loadSyntTx = false;
    parametrsChange.flag_hwUARTcontrolEN = false;
    parametrsChange.earseFlash = false;
    parametrsChange.flashMK = false;
    parametrsChange.flashEarsePage = false;



    parametrsChange.sizeCS = false;
    parametrsChange.sizeSPI = false;
    parametrsChange.sizeInterfase = false;

    parametrsChange.initGPIO = false;
    parametrsChange.readWriteGPIO = false;
    parametrsChange.initSPI = false;
    parametrsChange.initCS = false;

    parametrsChange.writeSpi = false;
    parametrsChange.writeInterface = false;

    parametrsChange.removeCs = false;
    parametrsChange.removeSpi = false;
    parametrsChange.removeInterface = false;

    parametrsChange.writeFunction = false;
    parametrsChange.initFunction = false;


    parametrs.sizeCS = 0;
    parametrs.sizeSPI = 0;
    parametrs.sizeInterfase = 0;
    parametrs.sizeFunction = 0;
    parametrs.initGPIO = 0;
    parametrs.readWriteGPIO = 0;
    parametrs.initSPI = 0;
    parametrs.initCS = 0;


    functionBoard.readWriteInerface = boardReadWriteInterface;
    functionBoard.readWriteSPI = boardReadWriteSPI;
    functionBoard.initSpi = _initSPI;
    functionBoard.initCS = _initCS;
    functionBoard.initInterface = _initInterface;
}

void ubpateFlash()
{

}

void updateCrcMemory()
{

}

#define PARAMETRS_SETS(x)                   do{parametrsChange.x = false; parametrsChange.theriseChange--;}while(0)
#define PARAMETRS_STATION_SETS(x)           do{if(parametrsChange.x) {parametrsStation.x = parametrs.x;PARAMETRS_SETS(x);ubpateFlash();}  }while(0)

void taskUprav()
{
    if(parametrsChange.theriseChange)
    {
        uint8_t oldSize = parametrsChange.theriseChange;
        if(parametrsChange.initGPIO)
        {
            init_GPIO_(parametrs.initGPIO & 0xFF, (parametrs.initGPIO >> 8) & 0xFF, (parametrs.initGPIO >> 16) & 0xFF);
            PARAMETRS_SETS(initGPIO);
        }
        if(parametrsChange.readWriteGPIO)
        {
            GPIO_TypeDef *port = getPort((parametrs.readWriteGPIO >> 4) & 0x0F);
            if(port)
                port->BSRR |= (parametrs.readWriteGPIO >> 8) == 1 ? (1 << (parametrs.readWriteGPIO & 0x0F)) : (1 << (16 + (parametrs.readWriteGPIO & 0x0F)));
            PARAMETRS_SETS(readWriteGPIO);
        }
        if(parametrsChange.initCS)
        {
            _initCS(parametrs.initCS);
            PARAMETRS_SETS(initCS);
        }
        if(parametrsChange.initSPI)
        {
            _initSPI(parametrs.initSPI, parametrs.initSPI >> 8, parametrs.initSPI >> 16);
            PARAMETRS_SETS(initSPI);
        }
        if(parametrsChange.initInterface)
        {
            uint8_t numSpi = (parametrs.initInterface.initInterface >> 8) & 0xFF;
            uint8_t numCs = (parametrs.initInterface.initInterface >> 16) & 0xFF;
            _initInterface((UsePerepheria)(parametrs.initInterface.initInterface & 0xFF), numSpi, numCs, parametrs.initInterface.name);
            PARAMETRS_SETS(initInterface);
        }
        if(parametrsChange.writeSpi)
        {
            boardReadWriteSPI(parametrs.writeSpi.spi, parametrs.writeSpi.cs, parametrs.writeSpi.data, sizeof(uint32_t)*8);
            PARAMETRS_SETS(writeSpi);
        }
        if(parametrsChange.writeInterface)
        {
            boardReadWriteInterface(parametrs.writeInterface.interface, parametrs.writeInterface.data, sizeof(uint32_t)*8);
            PARAMETRS_SETS(writeInterface);
        }
        if(parametrsChange.removeCs)
        {
            if(parametrs.removeCs < parametrs.sizeCS)
            {
                parametrs.sizeCS--;
                masCs[parametrs.removeCs] = masCs[parametrs.sizeCS];
            }
            PARAMETRS_SETS(removeCs);
        }
        if(parametrsChange.removeSpi)
        {
            if(parametrs.removeSpi < parametrs.sizeSPI)
            {
                parametrs.sizeSPI--;
                masCs[parametrs.removeSpi] = masCs[parametrs.sizeSPI];
            }
            PARAMETRS_SETS(removeSpi);
        }
        if(parametrsChange.removeInterface)
        {
            if(parametrs.removeInterface < parametrs.sizeInterfase)
            {
                parametrs.sizeInterfase--;
                masCs[parametrs.removeInterface] = masCs[parametrs.sizeInterfase];
            }
            PARAMETRS_SETS(removeInterface);
        }
        if(parametrsChange.initFunction)
        {
            if(parametrs.sizeFunction < SIZE_FUNCTION)
            {
                uint32_t addres = *((__IO uint32_t*)(parametrs.initFunction.addres + 4));
                masFunction[parametrs.sizeFunction].function = (pFunction)addres;
                strncpy(masFunction[parametrs.sizeFunction].name, parametrs.initFunction.name, MAX_SIZE_NAME);
                parametrs.sizeFunction++;
            }
            PARAMETRS_SETS(initFunction);
        }
        if(parametrsChange.doFunction)
        {
            if(parametrs.doFunction.function < parametrs.sizeFunction)
            {
                int ret = masFunction[parametrs.doFunction.function].function(&functionBoard, parametrs.doFunction.arguments, parametrs.doFunction.sizeArgument);
                (void)ret;
            }
            PARAMETRS_SETS(doFunction);
        }
        if(parametrsChange.removeFunction)
        {
            if(parametrs.removeFunction < parametrs.sizeFunction)
            {
                parametrs.sizeFunction--;
                RTOS_DeleteTask(masFunction[parametrs.removeFunction].function);
                masFunction[parametrs.removeFunction] = masFunction[parametrs.sizeFunction];
            }
            PARAMETRS_SETS(removeFunction);
        }
        if(parametrsChange.functionTimer)
        {
            if(parametrs.functionTimer.function < parametrs.sizeFunction)
            {
                RTOS_SetTask(masFunction[parametrs.functionTimer.function].function, parametrs.functionTimer.taskDelay,  \
                        parametrs.functionTimer.taskPeriod, parametrs.functionTimer.arguments, \
                        parametrs.functionTimer.sizeArgument);
            }
            PARAMETRS_SETS(functionTimer);
        }
        if(parametrsChange.stopTimerFunction)
        {
            if(parametrs.stopTimerFunction < parametrs.sizeFunction)
            {
                RTOS_DeleteTask(masFunction[parametrs.stopTimerFunction].function);
            }
            PARAMETRS_SETS(stopTimerFunction);
        }


        if(parametrsChange.boadrID)//в этом случае первые 2 байта id остальные версия прошивки и версия железа
        {
            parametrs.boadrID = (parametrs.boadrID & 0x0000FFFF) | (VERSION_FRIM << 16);//т.е. старшие 2 байта я не мойгу перетереть они костанты
        }
        PARAMETRS_STATION_SETS(boadrID);

        if(parametrsChange.flashEarsePage)
        {
            flashErasePage(ADDRES_PAGE_PARAMETRS_4K + parametrs.flashEarsePage*ONE_PAGE_SIZE);
            PARAMETRS_SETS(flashEarsePage);
        }
        if(parametrsChange.earseFlash)
        {
            for(int i = 0; i < 32; i++)
                flashErasePage(ADDRES_PAGE_PARAMETRS_4K + i*ONE_PAGE_SIZE);
            PARAMETRS_SETS(earseFlash);
        }
        if(oldSize == parametrsChange.theriseChange)//если пролшел весь цикл но значение не изменилось
            parametrsChange.theriseChange = 0;//то отчищаю значенияю
        //это возможно если в 1 регистр 2 запишут данные
    }


}
