#ifndef MEMORU_CONF_FLASH_H
#define MEMORU_CONF_FLASH_H

#include "gpio.h"


#define FLASH_SIZE      (1024* (*((uint16_t*)FLASHSIZE_BASE)))
#define ONE_PAGE_SIZE   1024
#define PAGE_MAX_VALUE	/*(FLASH_SIZE/ONE_PAGE_SIZE)*/128
//#define FLASH_SIZE      (ONE_PAGE_SIZE*PAGE_MAX_VALUE)
#define MIN_ADRESS      0x08000000
#define MAX_ADRESS      (MIN_ADRESS + FLASH_SIZE)

#define ADRES_USE(x)	((x < MAX_ADRESS) && (x >= MIN_ADRESS))	//принадлежит ли адрес данному интервалу


void flashErasePage(uint32_t address);
void flashEraseAllPages(void);

void writeFlash(uint32_t data, uint32_t pageAddress);
void writeFlashData8(uint32_t pageAddress, uint8_t *data, uint32_t dataLen);
void writeFlashData32(uint32_t pageAddress, uint32_t *data, uint32_t dataLen);
uint32_t readFlash(uint32_t pageAddress);
void readFlashData32(uint32_t pageAddress, uint32_t *data, uint32_t dataLen);




#endif

