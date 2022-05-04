#ifndef SETTING_DEVICE_H
#define SETTING_DEVICE_H

//модуль setting уровень взаимодейцствия с пользователем
// protocol физическая связь им быстрая обработка
// task обработка команда

#include "Buffer.h"
#include "uart1.h"
#include "soft_spi.h"
#include "spi.h"
#include "dac.h"
#include "memory_conf.h"

#include "Board.h"


//wake CMD for configurate
#define CDM_HRF_SET         20
#define CDM_RFSA_SET        21
#define CDM_ADF_LOW_SET     22  //ADF1
#define CDM_ADF_144_SET     23
#define CDM_LMX_HIGHT_SET   24


//respone
#define RESPONE_AFTER_COMMAND
#define RESPONE_ABOUT_ERROR     199

////адреса для протокола общения
//typedef enum {
//    ADF1        = 50,     //Sentizator adf low
//    ADF2        = 49,     //Sentizator adf 144
//    LMX         = 48,     //Sentizator lmz high
//    RFSA        = 47,     //Attenuator rfsa Rx
//    HRF         = 46,     //Attenuator hrf Tx
//    Key_Tx_F    = 45,     //Key for Tx input filtr
//    Key_Rx_F    = 44,     //Key for Rx output filtr
//    Key_SH_EN   = 43,     //Key for shlef enable
//    Key_Rx_BP_1 = 42,     //Key for enable RX_BYPASS_1
//    Key_Rx_BP_2 = 41,     //Key for enable RX_BYPASS_2
//    //for read and write dara RFSA
//    Write_Data_RFSA     = 21,
//    Read_Data_RFSA      = 20,
//    ADF1_Set_F  = 40,     //Enouther command for set Frequency ADF low
//    ADF2_Set_F  = 39,     //Enouther command for set Frequency ADF 144
//    ADF1_Init   = 38,     //init ADF1
//    ADF2_Init   = 37,     //init ADF2
//    LMX_Set_F   = 36,     //Enouther command for set Frequency LMX
//    FREE        = 77,     //Free nothin to do
//    Ask_Version = 70,     //Ask version frim
//}AddresDevice;

//Buffer_t bufferReadUART;
//Buffer_t bufferComand;

void spi_interupt(uint8_t data);
void cs_interupt();

void initSetting();

void wrireDatInterface(uint8_t data);

void taskUprav();
void updateCrcMemory();




#endif // SETTING_DEVICE_H
