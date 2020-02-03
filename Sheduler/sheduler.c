#include "sheduler.h"
#include "systimer.h"
#include <string.h>

#ifndef USE_RTOS


#define DISABLE_INTERRUPT NVIC_DisableIRQ(SysTick_IRQn);
#define RESTORE_INTERRUPT NVIC_EnableIRQ(SysTick_IRQn);

/******************************************************************************************
 * ��������� ������
 */
typedef struct
{
   pFunction pFunc;               // ��������� �� �������
   uint32_t delay;                          // �������� ����� ������ �������� ������
   uint32_t period;                         // ������ ������� ������
   bool run;                            // ���� ���������� ������ � �������
   char argument[MAX_ARGUMENT_SIZE];    //��������� ��� �������
   uint32_t argumentSize;               //������ ����������
}Task;


/******************************************************************************************
 * ���������� ������
 */
volatile static Task TaskArray[MAX_TASKS];      // ������� �����
volatile static uint8_t arrayTail = 0;                  // "�����" �������


/******************************************************************************************
 * ������������� ����, ����� ���� - 1 ��
 */
inline void RTOS_Init()
{
	arrayTail = 0;                               // "�����" � 0

	configureSystimre();
}

/******************************************************************************************
 * ���������� ������ � ������
 */
void RTOS_SetTask (pFunction taskFunc, uint32_t taskDelay, uint32_t taskPeriod, char *argument, uint32_t sizeArgument)
{
   if(!taskFunc) return;
   for(uint8_t i = 0; i < arrayTail; i++)                     // ����� ������ � ������� ������
   {
      if(TaskArray[i].pFunc == taskFunc)              // ���� �����, �� ��������� ����������
      {
//         DISABLE_INTERRUPT;

         TaskArray[i].delay  = taskDelay;
         TaskArray[i].period = taskPeriod;
         TaskArray[i].run    = false;
         strncpy(TaskArray[i].argument, argument, sizeArgument);
         TaskArray[i].argumentSize = sizeArgument;

//         RESTORE_INTERRUPT;
         return;                                      // �������, �������
      }
   }

   if (arrayTail < MAX_TASKS)                         // ���� ����� ������ � ������ ���
   {                                                  // � ���� �����,�� ���������
      //DISABLE_INTERRUPT;

      TaskArray[arrayTail].pFunc  = taskFunc;
      TaskArray[arrayTail].delay  = taskDelay;
      TaskArray[arrayTail].period = taskPeriod;
      TaskArray[arrayTail].run    = false;
      strncpy(TaskArray[arrayTail].argument, argument, sizeArgument);
      TaskArray[arrayTail].argumentSize = sizeArgument;

      arrayTail++;                                    // ����������� "�����"
      //RESTORE_INTERRUPT;
   }
}

/******************************************************************************************
 * �������� ������ �� ������
 */
void RTOS_DeleteTask (pFunction taskFunc)
{
   for (uint8_t i=0; i<arrayTail; i++)                        // �������� �� ������ �����
   {
      if(TaskArray[i].pFunc == taskFunc)              // ���� ������ � ������ �������
      {

//         DISABLE_INTERRUPT;
         if(i != (arrayTail - 1))                     // ��������� ��������� ������
         {                                            // �� ����� ���������
            TaskArray[i] = TaskArray[arrayTail - 1];
         }
         arrayTail--;                                 // ��������� ��������� "������"
//         RESTORE_INTERRUPT;
         return;
      }
   }
}

/******************************************************************************************
 * ��������� ����, ���������� � main
 */

void RTOS_DispatchTask()
{
//   void (*function) (void);
   for (uint8_t i=0; i<arrayTail; i++)                        // �������� �� ������ �����
   {
      if (TaskArray[i].run == true)                      // ���� ���� �� ���������� �������,
      {                                               // ���������� ������, �.�. ��
//         function = TaskArray[i].pFunc;               // ����� ���������� �����
                                                      // ���������� ������
         if(TaskArray[i].period == 0)
         {                                            // ���� ������ ����� 0
            RTOS_DeleteTask(TaskArray[i].pFunc);      // ������� ������ �� ������,

         }
         else
         {
            TaskArray[i].run = 0;                     // ����� ������� ���� �������
            if(!TaskArray[i].delay)                   // ���� ������ �� �������� ��������
            {                                         // ������ ��
               TaskArray[i].delay = TaskArray[i].period-1;
            }                                         // ������ ��� ���� ����� ������� �����
         }
//         (*function)();                               // ��������� ������
         TaskArray[i].pFunc(&functionBoard, TaskArray[i].argument, TaskArray[i].argumentSize);// ��������� ������
      }
   }
}

/******************************************************************************************
 * ��������� ������ ���� (���������� ����������� �������)
 */


void SysTick_Handler(void)//������ ��� ����������
{
   for (uint8_t i=0; i<arrayTail; i++)                        // �������� �� ������ �����
   {
      if  (TaskArray[i].delay == 0)                   // ���� ����� �� ���������� �������
    	  TaskArray[i].run = true;                      // ������� ���� �������,
      else TaskArray[i].delay--;                      // ����� ��������� �����
   }
}

#endif




