#ifndef SHEDULER_H
#define SHEDULER_H

#include <stdbool.h>
#include <stdint.h>
#include "CMD_const.h"


/******************************************************************************************
 * За основу взят планировщик задач с сайта ChipEnable.ru                                 *
 * http://chipenable.ru/index.php/programming-avr/item/110-planirovschik.html             *
 *                                                                                        *
 * Доработал Шибанов Владимир aka KontAr                                                  *
 * Дата: 26.03.2014                                                                       *
 *                                                                                        *
 * Изменения:                                                                             *
 * - добавлен однократный вызов задачи                                                    *
 * - добавлено удаление задачи по имени                                                   *
 * - при повторном добавлении задачи обновляются ее переменные                            *
 * - добавлен указатель на "хвост" списка                                                 *
 * - функции РТОС скорректированы с учетом "хвоста"                                       *
 ******************************************************************************************/

#define MAX_TASKS      10               // Количество задач


/******************************************************************************************
 * Прототипы фукнций
 */
void RTOS_Init (void);
void RTOS_SetTask (pFunction taskFunc, uint32_t taskDelay, uint32_t taskPeriod, char *argument, uint32_t sizeArgument);
void RTOS_DeleteTask (pFunction taskFunc);
void RTOS_DispatchTask (void);


#endif
