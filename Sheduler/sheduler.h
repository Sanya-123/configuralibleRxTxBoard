#ifndef SHEDULER_H
#define SHEDULER_H

#include <stdbool.h>
#include <stdint.h>
#include "CMD_const.h"


/******************************************************************************************
 * �� ������ ���� ����������� ����� � ����� ChipEnable.ru                                 *
 * http://chipenable.ru/index.php/programming-avr/item/110-planirovschik.html             *
 *                                                                                        *
 * ��������� ������� �������� aka KontAr                                                  *
 * ����: 26.03.2014                                                                       *
 *                                                                                        *
 * ���������:                                                                             *
 * - �������� ����������� ����� ������                                                    *
 * - ��������� �������� ������ �� �����                                                   *
 * - ��� ��������� ���������� ������ ����������� �� ����������                            *
 * - �������� ��������� �� "�����" ������                                                 *
 * - ������� ���� ��������������� � ������ "������"                                       *
 ******************************************************************************************/

#define MAX_TASKS      10               // ���������� �����


/******************************************************************************************
 * ��������� �������
 */
void RTOS_Init (void);
void RTOS_SetTask (pFunction taskFunc, uint32_t taskDelay, uint32_t taskPeriod, char *argument, uint32_t sizeArgument);
void RTOS_DeleteTask (pFunction taskFunc);
void RTOS_DispatchTask (void);


#endif
