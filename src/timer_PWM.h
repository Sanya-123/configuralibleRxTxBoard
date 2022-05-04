#ifndef TIMER_PWM_H
#define TIMER_PWM_H

#include "stm32f1xx.h"

typedef struct
{
	TIM_TypeDef *timer;
	uint8_t chanal;
	uint32_t presc;//100000/presc
	uint8_t qProcent;
}Timers;

void initTimerPWM(Timers tim);

void setQprocent(Timers tim, uint8_t pro);

uint32_t calcTime(uint32_t valeSec);



#endif
