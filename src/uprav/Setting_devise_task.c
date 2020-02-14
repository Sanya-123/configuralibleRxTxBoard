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

void initSetting()
{
    //init parametrs
    parametrsChange.theriseChange = 0;

//    parametrsChange.spanFreqMin = false;
//    parametrsChange.spanFreqMax = false;
//    parametrsChange.spanFreqStep = false;
//    parametrsChange.spanFreqIF = false;
//    parametrsChange.powInCalibr = false;
//    parametrsChange.powOutMin = false;
//    parametrsChange.powOutMax = false;
//    parametrsChange.powOutCorrection = false;
//    parametrsChange.powReal = false;
//    parametrsChange.vgaCorrection = false;
//    parametrsChange.key_RxTx = false;
//    parametrsChange.key_bypassIF = false;
//    parametrsChange.flag_loadSyntRx = false;
//    parametrsChange.flag_loadSyntRx_2 = false;
//    parametrsChange.flag_loadSyntTx = false;
//    parametrsChange.flag_hwUARTcontrolEN = false;
    parametrsChange.boadrID = false;
    parametrsChange.earseFlash = false;
    parametrsChange.flashMK = false;
    parametrsChange.flashEarsePage = false;



    parametrsChange.sizeCS = false;
    parametrsChange.sizeSPI = false;
    parametrsChange.sizeInterfase = false;

    parametrsChange.initGPIO = false;
    parametrsChange.readWriteGPIO = false;
    parametrsChange.initSPI = false;
    parametrsChange.initInterface = false;
    parametrsChange.initCS = false;

    parametrsChange.writeSpi = false;
    parametrsChange.writeInterface = false;

    parametrsChange.removeCs = false;
    parametrsChange.removeSpi = false;
    parametrsChange.removeInterface = false;

    parametrsChange.writeFunction = false;
    parametrsChange.initFunction = false;
    parametrsChange.doFunction = false;
    parametrsChange.removeFunction = false;
    parametrsChange.functionTimer = false;
    parametrsChange.stopTimerFunction = false;

    parametrsChange.saveConfig = false;


    parametrs.sizeCS = 0;
    parametrs.sizeSPI = 0;
    parametrs.sizeInterfase = 0;
    parametrs.sizeFunction = 0;
    parametrs.initGPIO = 0;
    parametrs.readWriteGPIO = 0;
    parametrs.initSPI = 0;
    parametrs.initCS = 0;
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
            if(/*_initCS*/functionBoard.initCS(parametrs.initCS) == 0)
                parametrs.sizeCS++;
            PARAMETRS_SETS(initCS);
        }
        if(parametrsChange.initSPI)
        {
            if(/*_initSPI*/functionBoard.initSpi(parametrs.initSPI, parametrs.initSPI >> 8, parametrs.initSPI >> 16) == 0)
                parametrs.sizeSPI++;
            PARAMETRS_SETS(initSPI);
        }
        if(parametrsChange.initInterface)
        {
//            uint8_t numSpi = (parametrs.initInterface.initInterface >> 8) & 0xFF;
//            uint8_t numCs = (parametrs.initInterface.initInterface >> 16) & 0xFF;
            if(/*_initInterface*/functionBoard.initInterface((UsePerepheria)(parametrs.initInterface.interface), \
                                                             parametrs.initInterface.numSPI, parametrs.initInterface.numCS, parametrs.initInterface.name) == 0)
                parametrs.sizeInterfase++;
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
                configBoard.sizeCS--;
                configBoard.masCs[parametrs.removeCs] = configBoard.masCs[parametrs.sizeCS];
            }
            PARAMETRS_SETS(removeCs);
        }
        if(parametrsChange.removeSpi)
        {
            if(parametrs.removeSpi < parametrs.sizeSPI)
            {
                parametrs.sizeSPI--;
                configBoard.sizeSPI--;
                configBoard.masSpi[parametrs.removeSpi] = configBoard.masSpi[parametrs.sizeSPI];
            }
            PARAMETRS_SETS(removeSpi);
        }
        if(parametrsChange.removeInterface)
        {
            if(parametrs.removeInterface < parametrs.sizeInterfase)
            {
                parametrs.sizeInterfase--;
                configBoard.sizeInterfase--;
                configBoard.masInterface[parametrs.removeInterface] = configBoard.masInterface[parametrs.sizeInterfase];
            }
            PARAMETRS_SETS(removeInterface);
        }
        if(parametrsChange.initFunction)
        {
            if(_init_function(parametrs.initFunction.addres, parametrs.initFunction.name) == 0)
                parametrs.sizeFunction++;
//            if(parametrs.sizeFunction < SIZE_FUNCTION)
//            {
//                uint32_t addres = *((__IO uint32_t*)(parametrs.initFunction.addres + 4));
//                configBoard.masFunction[parametrs.sizeFunction].function = (pFunction)addres;
//                strncpy(configBoard.masFunction[parametrs.sizeFunction].name, parametrs.initFunction.name, MAX_SIZE_NAME);
//                parametrs.sizeFunction++;
//                configBoard.sizeFunction++;
//            }
            PARAMETRS_SETS(initFunction);
        }
        if(parametrsChange.doFunction)
        {
            if(parametrs.doFunction.function < parametrs.sizeFunction)
            {
                int ret = configBoard.masFunction[parametrs.doFunction.function].function(&functionBoard, parametrs.doFunction.arguments, parametrs.doFunction.sizeArgument);
                (void)ret;
            }
            PARAMETRS_SETS(doFunction);
        }
        if(parametrsChange.removeFunction)
        {
            if(parametrs.removeFunction < parametrs.sizeFunction)
            {
                functionTimerStop(parametrs.removeFunction);
                parametrs.sizeFunction--;
                configBoard.sizeFunction--;
                configBoard.masFunction[parametrs.removeFunction] = configBoard.masFunction[parametrs.sizeFunction];
                configBoard.masAddresFunction[parametrs.removeFunction] = configBoard.masAddresFunction[parametrs.sizeFunction];
            }
            PARAMETRS_SETS(removeFunction);
        }
        if(parametrsChange.functionTimer)
        {
            functionTimeStart(parametrs.functionTimer.function, parametrs.functionTimer.taskDelay,  \
                              parametrs.functionTimer.taskPeriod, parametrs.functionTimer.arguments, \
                              parametrs.functionTimer.sizeArgument);
            PARAMETRS_SETS(functionTimer);
        }
        if(parametrsChange.stopTimerFunction)
        {
            functionTimerStop(parametrs.stopTimerFunction);
            PARAMETRS_SETS(stopTimerFunction);
        }
        if(parametrsChange.saveConfig)
        {
            saveConfig();
            PARAMETRS_SETS(saveConfig);
        }
        if(parametrsChange.getInfoCS)
        {
            if(parametrs.getInfoCS < parametrs.sizeCS)
                parametrs.initCS = configBoard.masCs[parametrs.getInfoCS].numPin | (getPinPort(configBoard.masCs[parametrs.getInfoCS].port) << 4);
            PARAMETRS_SETS(getInfoCS);
        }
        if(parametrsChange.getInfoSPI)
        {
            if(parametrs.getInfoSPI < parametrs.sizeSPI)
            {
                //записываю данные в регистер
                parametrs.initSPI = (configBoard.masSpi[parametrs.getInfoSPI].numPin_MISO | (getPinPort(configBoard.masSpi[parametrs.getInfoSPI].portPin_MISO) << 4)) & 0xFF;
                //перенашу их на 8 битов
                parametrs.initSPI = parametrs.initSPI << 8;
                //дозаписываю в начало
                parametrs.initSPI |= (configBoard.masSpi[parametrs.getInfoSPI].numPin_MOSI | (getPinPort(configBoard.masSpi[parametrs.getInfoSPI].portPin_MOSI) << 4)) & 0xFF;
                //перенашу их на 8 битов
                parametrs.initSPI = parametrs.initSPI << 8;
                //дозаписываю в начало
                parametrs.initSPI |= (configBoard.masSpi[parametrs.getInfoSPI].numPin_CLK | (getPinPort(configBoard.masSpi[parametrs.getInfoSPI].portPin_CLK) << 4)) & 0xFF;
            }
            PARAMETRS_SETS(getInfoSPI);
        }
        if(parametrsChange.getInfoInterface)
        {
            if(parametrs.getInfoInterface < parametrs.sizeInterfase)
            {
                parametrs.initInterface.interface = (uint8_t)configBoard.masInterface[parametrs.getInfoInterface].perepheria1;
                for(uint8_t i = 0; i < parametrs.sizeSPI; i++)
                {
                    if(configBoard.masInterface[parametrs.getInfoInterface].spi == (&configBoard.masSpi[i]))
                    {
                        parametrs.initInterface.numSPI = i;
                        break;
                    }
                }
                for(uint8_t i = 0; i < parametrs.sizeCS; i++)
                {
                    if(configBoard.masInterface[parametrs.getInfoInterface].cs == (&configBoard.masCs[i]))
                    {
                        parametrs.initInterface.numCS = i;
                        break;
                    }
                }
                strncpy(parametrs.initInterface.name, configBoard.masInterface[parametrs.getInfoInterface].name, MAX_SIZE_NAME);
             }
            PARAMETRS_SETS(getInfoInterface);
        }
        if(parametrsChange.getInfoFunction)
        {
            if(parametrs.getInfoFunction < parametrs.sizeFunction)
            {
                parametrs.initFunction.addres = configBoard.masAddresFunction[parametrs.getInfoFunction];
                strncpy(parametrs.initFunction.name, configBoard.masFunction[parametrs.getInfoFunction].name, MAX_SIZE_NAME);
            }
            PARAMETRS_SETS(getInfoFunction);
        }


        if(parametrsChange.boadrID)//в этом случае первые 2 байта id остальные версия прошивки и версия железа
        {
            parametrs.boadrID = (parametrs.boadrID & 0x0000FFFF) | (VERSION_FRIM << 16);//т.е. старшие 2 байта я не мойгу перетереть они костанты
            PARAMETRS_SETS(boadrID);
        }
//        PARAMETRS_STATION_SETS(boadrID);

        if(parametrsChange.flashEarsePage)
        {
            flashErasePage(ADDRES_PAGE_PARAMETRS_STATION + parametrs.flashEarsePage*ONE_PAGE_SIZE);
            PARAMETRS_SETS(flashEarsePage);
        }
        if(parametrsChange.earseFlash)
        {
            for(int i = 0; i < 32; i++)
                flashErasePage(ADDRES_PAGE_PARAMETRS_STATION + i*ONE_PAGE_SIZE);
            PARAMETRS_SETS(earseFlash);
        }
        if(oldSize == parametrsChange.theriseChange)//если пролшел весь цикл но значение не изменилось
            parametrsChange.theriseChange = 0;//то отчищаю значенияю
        //это возможно если в 1 регистр 2 запишут данные
    }
}
