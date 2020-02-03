#ifndef SBUFFER_H
#define SBUFFER_H

#include <stdint.h>

#define SIZE_BUFFER     256 //Размер буффера

typedef struct
{
    uint16_t sizeBuffer;
    uint16_t pointBegin;
    uint16_t pointEnd;
    uint8_t data[SIZE_BUFFER];
}Buffer_t;//Структура кольцевого буффера

void initBuffer(Buffer_t *buffer);

uint16_t takeDataBuffer(Buffer_t *buffer, uint8_t *data, uint16_t dataLen);
uint16_t readDataBuffer(Buffer_t *buffer, uint8_t *data, uint16_t dataLen);
uint16_t getSizeData(Buffer_t *buffer);

void bufferSetData(Buffer_t *buffer, uint8_t data);
void bufferSetDataMas(Buffer_t *buffer, uint8_t *data, uint16_t size);





#endif // SBUFFER_H
