#include "timer_PWM.h"

void initTimerPWM(Timers tim)
{
    if(tim.timer == TIM1)//тактирование таймера
    {
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    }
    else if(tim.timer == TIM2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    }
    else if(tim.timer == TIM3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    }
    else if(tim.timer == TIM4)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    }

    tim.timer->CCER |= 1 << 4*(tim.chanal - 1);//разрешаю таймеру использовать кокойто катал
    //биты использования канала кратны 4(0,4,8,12);

    //prescaler
    //	tim.timer->PSC = calcTime(tim.presc);
    tim.timer->PSC = tim.presc-1;
    tim.timer->ARR = 240-1;//нужна брать число больше 100

    //есть 2 режима PWM OCxM = 110 или 111
    switch(tim.chanal)
    {
    case 1:
    {
        tim.timer->CCR1 = (tim.qProcent*(tim.timer->ARR)/100);
        tim.timer->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
        break;
    }
    case 2:
    {
        tim.timer->CCR2 = (tim.qProcent*(tim.timer->ARR)/100);
        tim.timer->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
        break;
    }
    case 3:
    {
        tim.timer->CCR3 = (tim.qProcent*(tim.timer->ARR)/100);
        tim.timer->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;
        break;
    }
    case 4:
    {
        tim.timer->CCR4 = (tim.qProcent*(tim.timer->ARR)/100);
        tim.timer->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;
        break;
    }
    }

    tim.timer->CR1 |= TIM_CR1_CEN;//enable timer
}

void setQprocent(Timers tim, uint8_t pro)
{
    if(pro > 100)
        pro = 100;
    tim.qProcent = pro;
    switch(tim.chanal)
    {
    case 1:
    {
        tim.timer->CCR1 = (tim.qProcent*(tim.timer->ARR+1)/100);
        break;
    }
    case 2:
    {
        tim.timer->CCR2 = (tim.qProcent*(tim.timer->ARR+1)/100);
        break;
    }
    case 3:
    {
        tim.timer->CCR3 = (tim.qProcent*(tim.timer->ARR+1)/100);
        break;
    }
    case 4:
    {
        tim.timer->CCR4 = (tim.qProcent*(tim.timer->ARR+1)/100);
        break;
    }
    default:
        break;
    }
}

uint32_t calcTime(uint32_t valeSec)//калькулятор переводящий из количества тиков в секунду в число которое нужно записть в регистр PWR
{
    //	return (2*SystemCoreClock/valeSec+1)/2 - 1;
    return (2*SystemCoreClock/valeSec+1)/2 - 1;
}

