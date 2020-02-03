#include "systimer.h"
#include "core_cm3.h"

#ifndef USE_RTOS

void configureSystimre()
{
	SysTick_Config(SystemCoreClock / 1000); /* SysTick закончит счет каждые 1 мс */
	//NVIC_EnableIRQ(SysTick_IRQn);
	__enable_irq();
}

#endif

//void startSystimer()
//{
//	NVIC_EnableIRQ(SysTick_IRQn);
//}

//void stopSystimer()
//{
//	NVIC_DisableIRQ(SysTick_IRQn);
//}
