#ifndef DAC1_H
#define DAC1_H

#include "gpio.h"
#include "defines_gpio.h"
#include <stdbool.h>

#define USE_PROV_AFTER  0//если проверять когда устанавливает значения установилось ли(1) проверять в отдельной функции (0)
/* Example with 0
 * do{
 * setVoltageDAC(x);}
 * while(DACRighr(x));
 * --------------
 * Example with 1
 * setVoltageDAC(x);
*/

void initDAC(void);
void setVoltageDAC(uint16_t value);

bool DACRighr(uint16_t value);
uint16_t getDACValue();


#endif
