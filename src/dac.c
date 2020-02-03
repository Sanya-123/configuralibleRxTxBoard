#include "dac.h"

void initDAC()
{
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    PIN_CONFIGURATION(DAC_CANAL1_OUT);

    DAC->CR |= DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1;//CHange External line 9
    DAC->CR |= DAC_CR_EN1;//Enable DAC chanal 2
}

bool DACRighr(uint16_t value)
{
    return value == DAC->DOR1;
}

void setVoltageDAC(uint16_t value)
{
    DAC->DHR12R1 = value & 0x0FFF;//Устанавливаю биты

#if USE_PROV_AFTER == 1 //Если хотим чтобы точно установились
    while (DACRighr(value))//проверюу устаеовились ли
    {
        DAC->DHR12R1 = value;
    }
#endif
}

uint16_t getDACValue()
{
    return (uint16_t)(DAC->DOR1);
}




