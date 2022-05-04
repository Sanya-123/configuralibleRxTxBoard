#ifndef _GPIO_MACROS_F1_H
#define _GPIO_MACROS_F1_H

#include "stm32f1xx.h"
//#include "stdbool.h"

/*
 Данный набор макросов упрощает работу с GPIO портами микроконтроллеров STM32F4xx
 Протестировано на STM32F100 STM32F103

 За основу взято:
 http://we.easyelectronics.ru/STM32/udobnaya-nastroyka-gpio-portov.html#comment114408
 http://ziblog.ru/2012/09/09/linii-vvoda-vyivoda-stm32-chast-3.html
 http://we.easyelectronics.ru/Soft/preprocessor-c.html
 https://www.opennet.ru/docs/RUS/cpp/

 Определение GPIO должно выглядить так:
 #define PIN_NAME PORT, PIN, MODE, SPEED, DEFAULT_STATE
 PORT - A, B, C, D и тд
 PIN - номер бита в порте
 MODE - режим выхода
 SPEED - скорость (актуально только для выхода)
 DEFAULT_STATE - значение поумолчанию для настроенных на выход пинов

 Пример: #define PIN_LED C, 9, MODE_OUTPUT_PUSH_PULL, SPEED_2MHZ,   0
 Пример: #define BUTTON1 C, 8, MODE_INPUT_PULL_UP,    SPEED_2MHZ,   0

 Доступные операции:
 PIN_CONFIGURATION(PIN_NAME) - настройка gpio
 PIN_ON(PIN_NAME)            - включить (установка лог. 1)
 PIN_OFF(PIN_NAME)           - выключить (установка лог. 0)
 PIN_TOGGLE(PIN_NAME)        - переключить состояние
 PIN_ISSET                   - состояние порта, используется так: if(PIN_ISSET(PIN_NAME))
*/

//--------------------------------------------------------------------------------------------------
// Доступные настройки GPIO

// Режимы
#define MODE_OUTPUT_PUSH_PULL               MODE_OUTPUT_PUSH_PULL
#define MODE_OUTPUT_OPEN_DRAIN              MODE_OUTPUT_OPEN_DRAIN
#define MODE_AF_PUSH_PULL                   MODE_AF_PUSH_PULL
#define MODE_AF_OPEN_DRAIN                  MODE_AF_OPEN_DRAIN
#define MODE_INPUT_FLOATING                 MODE_INPUT_FLOATING
#define MODE_INPUT_PULL_UP                  MODE_INPUT_PULL_UP
#define MODE_INPUT_PULL_DOWN                MODE_INPUT_PULL_DOWN
#define MODE_ANALOG                         MODE_ANALOG

// Скорости
#define SPEED_2MHZ                          2UL
#define SPEED_10MHZ                         1UL
#define SPEED_50MHZ                         3UL

//--------------------------------------------------------------------------------------------------
// Установка параметров GPIO
/*
 Традиционное обнуление-присвоение:
 { GPIO##PORT->CRL &= ~(15UL << 0); GPIO##PORT->CRL |= ((MODE) << 0); }
 эквивалентно записи
 { GPIO##PORT->CRL = ( GPIO##PORT->CRL & (~(15UL << 0)) ) | ((MODE) << 0); }
  но без промежуточных состояний порта, что гораздо безопаснее
*/
#define _GPIO_PIN0_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRL = ( GPIO##PORT->CRL & (~(15UL << 0)) ) | ((MODE) << 0);

#define _GPIO_PIN1_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRL = ( GPIO##PORT->CRL & (~(15UL << 4)) ) | ((MODE) << 4);

#define _GPIO_PIN2_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRL = ( GPIO##PORT->CRL & (~(15UL << 8)) ) | ((MODE) << 8);

#define _GPIO_PIN3_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRL = ( GPIO##PORT->CRL & (~(15UL << 12)) ) | ((MODE) << 12);

#define _GPIO_PIN4_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRL = ( GPIO##PORT->CRL & (~(15UL << 16)) ) | ((MODE) << 16);

#define _GPIO_PIN5_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRL = ( GPIO##PORT->CRL & (~(15UL << 20)) ) | ((MODE) << 20);

#define _GPIO_PIN6_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRL = ( GPIO##PORT->CRL & (~(15UL << 24)) ) | ((MODE) << 24);

#define _GPIO_PIN7_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRL = ( GPIO##PORT->CRL & (~(15UL << 28)) ) | ((MODE) << 28);

#define _GPIO_PIN8_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRH = ( GPIO##PORT->CRH & (~(15UL << 0)) ) | ((MODE) << 0);

#define _GPIO_PIN9_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRH = ( GPIO##PORT->CRH & (~(15UL << 4)) ) | ((MODE) << 4);

#define _GPIO_PIN10_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRH = ( GPIO##PORT->CRH & (~(15UL << 8)) ) | ((MODE) << 8);

#define _GPIO_PIN11_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRH = ( GPIO##PORT->CRH & (~(15UL << 12)) ) | ((MODE) << 12);

#define _GPIO_PIN12_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRH = ( GPIO##PORT->CRH & (~(15UL << 16)) ) | ((MODE) << 16);

#define _GPIO_PIN13_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRH = ( GPIO##PORT->CRH & (~(15UL << 20)) ) | ((MODE) << 20);

#define _GPIO_PIN14_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRH = ( GPIO##PORT->CRH & (~(15UL << 24)) ) | ((MODE) << 24);

#define _GPIO_PIN15_CONFIGURATION(PORT, MODE) \
    GPIO##PORT->CRH = ( GPIO##PORT->CRH & (~(15UL << 28)) ) | ((MODE) << 28);

//--------------------------------------------------------------------------------------------------
// Настройка режима GPIO
/*
 Традиционное обнуление-присвоение:
 { GPIO##PORT->ODR &= ~(1UL << PIN); GPIO##PORT->ODR |= ((DEFAULT_STATE & 1UL) << PIN); }
 эквивалентно записи
 { GPIO##PORT->ODR = ( GPIO##PORT->ODR & (~(1UL << PIN)) ) | ((DEFAULT_STATE & 1UL) << PIN); }
  но без промежуточных состояний порта, что гораздо безопаснее
*/
#define _SET_MODE_OUTPUT_PUSH_PULL(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    _GPIO_PIN##PIN##_CONFIGURATION(PORT, 0UL | SPEED) \
    GPIO##PORT->ODR = ( GPIO##PORT->ODR & (~(1UL << PIN)) ) | ((DEFAULT_STATE & 1UL) << PIN);

#define _SET_MODE_OUTPUT_OPEN_DRAIN(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    _GPIO_PIN##PIN##_CONFIGURATION(PORT, 4UL | SPEED) \
    GPIO##PORT->ODR = ( GPIO##PORT->ODR & (~(1UL << PIN)) ) | ((DEFAULT_STATE & 1UL) << PIN);

#define _SET_MODE_AF_PUSH_PULL(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    _GPIO_PIN##PIN##_CONFIGURATION(PORT, 8UL | SPEED)

#define _SET_MODE_AF_OPEN_DRAIN(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    _GPIO_PIN##PIN##_CONFIGURATION(PORT, 12UL | SPEED)

#define _SET_MODE_INPUT_FLOATING(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    _GPIO_PIN##PIN##_CONFIGURATION(PORT, 4UL)

#define _SET_MODE_INPUT_PULL_UP(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    _GPIO_PIN##PIN##_CONFIGURATION(PORT, 8UL)  \
        GPIO##PORT->BSRR |= (1UL << PIN);

#define _SET_MODE_INPUT_PULL_DOWN(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    _GPIO_PIN##PIN##_CONFIGURATION(PORT, 8UL); \
        GPIO##PORT->BSRR |= (1UL << PIN) << 16;

#define _SET_MODE_ANALOG(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    _GPIO_PIN##PIN##_CONFIGURATION(PORT, 0UL)

//--------------------------------------------------------------------------------------------------
// Реализация макросов

#define _PIN_CONFIGURATION(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    do { _SET_##MODE(PORT, PIN, MODE, SPEED, DEFAULT_STATE) } while (0)

#define _PIN_ON(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    do { GPIO##PORT->BSRR = (1UL << PIN); } while (0)

#define _PIN_OFF(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    do { GPIO##PORT->BRR = (1UL << PIN); } while (0)

#define _PIN_TOGGLE(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    do { GPIO##PORT->ODR ^= (1UL << PIN); } while (0)

#define _PIN_ISSET(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    ( GPIO##PORT->IDR & (1UL << PIN) )


//MY
#define _PIN_ISSET_SEND(PORT, PIN, MODE, SPEED, DEFAULT_STATE) \
    (( GPIO##PORT->ODR & (1UL << PIN) ) >> PIN)

//--------------------------------------------------------------------------------------------------
// Переопределение макросов

#define PIN_CONFIGURATION(PIN_DESCRIPTION)  _PIN_CONFIGURATION(PIN_DESCRIPTION)
#define PIN_ON(PIN_DESCRIPTION)             _PIN_ON(PIN_DESCRIPTION)
#define PIN_OFF(PIN_DESCRIPTION)            _PIN_OFF(PIN_DESCRIPTION)
#define PIN_TOGGLE(PIN_DESCRIPTION)         _PIN_TOGGLE(PIN_DESCRIPTION)
#define PIN_ISSET(PIN_DESCRIPTION)          _PIN_ISSET(PIN_DESCRIPTION)

//MY
#define PIN_ISSET_SEND(PIN_DESCRIPTION)		_PIN_ISSET_SEND(PIN_DESCRIPTION)

#endif // _GPIO_MACROS_F1_H
