#include "sheduler.h"
#include "systimer.h"
#include <string.h>

#ifndef USE_RTOS


#define DISABLE_INTERRUPT NVIC_DisableIRQ(SysTick_IRQn);
#define RESTORE_INTERRUPT NVIC_EnableIRQ(SysTick_IRQn);

/******************************************************************************************
 * Структура задачи
 */
typedef struct
{
   pFunction pFunc;               // указатель на функцию
   uint32_t delay;                          // задержка перед первым запуском задачи
   uint32_t period;                         // период запуска задачи
   bool run;                            // флаг готовности задачи к запуску
   char argument[MAX_ARGUMENT_SIZE];    //аргументы для функции
   uint32_t argumentSize;               //размер аргументов
}Task;


/******************************************************************************************
 * Переменные модуля
 */
volatile static Task TaskArray[MAX_TASKS];      // очередь задач
volatile static uint8_t arrayTail = 0;                  // "хвост" очереди


/******************************************************************************************
 * Инициализация РТОС, время тика - 1 мс
 */
inline void RTOS_Init()
{
	arrayTail = 0;                               // "хвост" в 0

	configureSystimre();
}

/******************************************************************************************
 * Добавление задачи в список
 */
void RTOS_SetTask (pFunction taskFunc, uint32_t taskDelay, uint32_t taskPeriod, char *argument, uint32_t sizeArgument)
{
   if(!taskFunc) return;
   for(uint8_t i = 0; i < arrayTail; i++)                     // поиск задачи в текущем списке
   {
      if(TaskArray[i].pFunc == taskFunc)              // если нашли, то обновляем переменные
      {
//         DISABLE_INTERRUPT;

         TaskArray[i].delay  = taskDelay;
         TaskArray[i].period = taskPeriod;
         TaskArray[i].run    = false;
         strncpy(TaskArray[i].argument, argument, sizeArgument);
         TaskArray[i].argumentSize = sizeArgument;

//         RESTORE_INTERRUPT;
         return;                                      // обновив, выходим
      }
   }

   if (arrayTail < MAX_TASKS)                         // если такой задачи в списке нет
   {                                                  // и есть место,то добавляем
      //DISABLE_INTERRUPT;

      TaskArray[arrayTail].pFunc  = taskFunc;
      TaskArray[arrayTail].delay  = taskDelay;
      TaskArray[arrayTail].period = taskPeriod;
      TaskArray[arrayTail].run    = false;
      strncpy(TaskArray[arrayTail].argument, argument, sizeArgument);
      TaskArray[arrayTail].argumentSize = sizeArgument;

      arrayTail++;                                    // увеличиваем "хвост"
      //RESTORE_INTERRUPT;
   }
}

/******************************************************************************************
 * Удаление задачи из списка
 */
void RTOS_DeleteTask (pFunction taskFunc)
{
   for (uint8_t i=0; i<arrayTail; i++)                        // проходим по списку задач
   {
      if(TaskArray[i].pFunc == taskFunc)              // если задача в списке найдена
      {

//         DISABLE_INTERRUPT;
         if(i != (arrayTail - 1))                     // переносим последнюю задачу
         {                                            // на место удаляемой
            TaskArray[i] = TaskArray[arrayTail - 1];
         }
         arrayTail--;                                 // уменьшаем указатель "хвоста"
//         RESTORE_INTERRUPT;
         return;
      }
   }
}

/******************************************************************************************
 * Диспетчер РТОС, вызывается в main
 */

void RTOS_DispatchTask()
{
//   void (*function) (void);
   for (uint8_t i=0; i<arrayTail; i++)                        // проходим по списку задач
   {
      if (TaskArray[i].run == true)                      // если флаг на выполнение взведен,
      {                                               // запоминаем задачу, т.к. во
//         function = TaskArray[i].pFunc;               // время выполнения может
                                                      // измениться индекс
         if(TaskArray[i].period == 0)
         {                                            // если период равен 0
            RTOS_DeleteTask(TaskArray[i].pFunc);      // удаляем задачу из списка,

         }
         else
         {
            TaskArray[i].run = 0;                     // иначе снимаем флаг запуска
            if(!TaskArray[i].delay)                   // если задача не изменила задержку
            {                                         // задаем ее
               TaskArray[i].delay = TaskArray[i].period-1;
            }                                         // задача для себя может сделать паузу
         }
//         (*function)();                               // выполняем задачу
         TaskArray[i].pFunc(&functionBoard, TaskArray[i].argument, TaskArray[i].argumentSize);// выполняем задачу
      }
   }
}

/******************************************************************************************
 * Таймерная служба РТОС (прерывание аппаратного таймера)
 */


void SysTick_Handler(void)//таймер для диспетчера
{
   for (uint8_t i=0; i<arrayTail; i++)                        // проходим по списку задач
   {
      if  (TaskArray[i].delay == 0)                   // если время до выполнения истекло
    	  TaskArray[i].run = true;                      // взводим флаг запуска,
      else TaskArray[i].delay--;                      // иначе уменьшаем время
   }
}

#endif




