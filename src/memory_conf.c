#include "memory_conf.h"


void flashUnlock()//открытие памяти для записи
{
    FLASH->KEYR |= FLASH_KEY1;
    FLASH->KEYR |= FLASH_KEY2;
}

void flashLock()//после записи нужно закрыть
{
    FLASH->CR |= FLASH_CR_LOCK;
}

//Функция возврщает true когда можно стирать или писать память.
uint8_t flashReady(void)
{
    return !(FLASH->SR & FLASH_SR_BSY);
}

//Функция стирает ВСЕ страницы. При её вызове прошивка самоуничтожается
void flashEraseAllPages(void)
{
    while(FLASH->CR & FLASH_CR_LOCK)//проверяю открыта ли память
    {
        flashUnlock();
    }

    FLASH->CR |= FLASH_CR_MER; //Устанавливаем бит стирания ВСЕХ страниц
    FLASH->CR |= FLASH_CR_STRT; //Начать стирание
    while(!flashReady());// Ожидание готовности.. Хотя оно уже наверное ни к чему здесь...
    FLASH->CR &= FLASH_CR_MER;
    flashLock();
}
//Функция стирает одну страницу. В качестве адреса можно использовать любой
//принадлежащий диапазону адресов той странице которую нужно очистить.
void flashErasePage(uint32_t address)
{
    if(ADRES_USE(address))//если память не переполнена
    {
        while(FLASH->CR & FLASH_CR_LOCK)//проверяю открыта ли память
        {
            flashUnlock();
        }

        FLASH->CR |= FLASH_CR_PER; //Устанавливаем бит стирания одной страницы
        FLASH->AR = address; // Задаем её адрес
        FLASH->CR |= FLASH_CR_STRT; // Запускаем стирание
//	while(!flashReady());  //Ждем пока страница сотрется.
        FLASH->SR = FLASH_SR_EOP;
        FLASH->CR&= ~FLASH_CR_PER; //Сбрасываем бит обратно
        flashLock();
    }
}

void writeFlash(uint32_t data, uint32_t pageAddress)
{
    if(ADRES_USE(pageAddress))//если память не переполнена
    {
        while(FLASH->CR & FLASH_CR_LOCK)//проверяю открыта ли память
        {
            flashUnlock();
        }

        FLASH->CR |= FLASH_CR_PG; //Разрешаем программирование флеша

        if((data & 0xFFFF) != 0xFFFF)//если первая половина и так еденички то ее не записываю
        {
            //записываю первую половину
            while(!flashReady());
            *((__IO uint16_t *)pageAddress) = (uint16_t)(data & 0xFFFF);
            FLASH->SR = FLASH_SR_EOP;
        }

        if((data >> 16) != 0xFFFF)//если вторая половина и так еденички то ее не записываю
        {
            pageAddress +=2;
            //записываю вторую половину
            while(!flashReady());
            (*(__IO uint16_t*) (pageAddress)) = (uint16_t)(data >> 16);
            FLASH->SR = FLASH_SR_EOP;//конец операции
        }

        FLASH->CR &= ~(FLASH_CR_PG); //Запрещаем программирование флеша
//        flashLock();//закрываю память
    }
}

void writeFlashData8(uint32_t pageAddress, uint8_t *data, uint32_t dataLen)
{
    uint32_t i;
    for(i = 0; i < dataLen/4; i++)//собираю из 8 битных 32 битные пакеты и записываю
    {
        uint32_t data32 = (data[i*4] << 0) | (data[i*4 + 1] << 8) | (data[i*4 + 2] << 16) | (data[i*4 + 3] << 24);
        writeFlash(data32, pageAddress + i*4);
    }

    //оставшиеся собираю в один пакет и записываю
    if(dataLen%4 != 0)//FIXME
    {
        uint32_t data32 = 0xFFFFFFFF;
        for(int j = 0; j < dataLen%4; j++)
        {
            data32 &= data[i + j] << (j*8);
        }
        writeFlash(data32, pageAddress + i*4);
    }
}


void writeFlashData32(uint32_t pageAddress, uint32_t *data, uint32_t dataLen)
{
    for(uint32_t i = 0; i < dataLen; i++)
    {
        writeFlash(data[i], pageAddress + i*4);
    }
}

uint32_t readFlash(uint32_t pageAddress)
{
//    uint32_t data = 0;
//    if(ADRES_USE(pageAddress))
//    {
//        data = *((__IO uint32_t *)pageAddress);//считываю
//    }

//    return data;
    return *((__IO uint32_t *)pageAddress);
}




void readFlashData32(uint32_t pageAddress, uint32_t *data, uint32_t dataLen)
{
    for(uint32_t i = 0; i < dataLen; i++)
        data[i] = readFlash(pageAddress + i*4);
}
