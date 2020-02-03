#include "Buffer.h"

void initBuffer(Buffer_t *buffer)
{
    buffer->sizeBuffer = SIZE_BUFFER;
    buffer->pointBegin = 0;
    buffer->pointEnd = 0;
}

uint16_t getSizeData(Buffer_t *buffer)//получения информации сколько осталось в буффере
{
    if(buffer->pointEnd >= buffer->pointBegin)//Если точка хвоста назодится поже начальной
        return buffer->pointEnd - buffer->pointBegin;
    else//если точка конца пошла на новый круг
        return SIZE_BUFFER + buffer->pointEnd - buffer->pointBegin;
}
//функция выдает количество прочитаных байит
uint16_t readDataBuffer(Buffer_t *buffer, uint8_t *data, uint16_t dataLen)//получения информации без изменеия точки начало
{
    uint16_t pointFromRead = buffer->pointBegin;//точка считывания
    for(int i = 0; i < dataLen; i++)//даю буфферу столько информации сколько он сможет взять
    {
        if(getSizeData(buffer) == 0)//если в буффере меньше информации чем размер получаймого буфера
        {
            return i;
        }
        data[i] = buffer->data[pointFromRead++];//считываю информацию из буффера
        if(pointFromRead == SIZE_BUFFER)//при достижении конца начинабю сначало
            pointFromRead = 0;
    }

    return dataLen;
}
//функция выдает количество прочитаных байит
uint16_t takeDataBuffer(Buffer_t *buffer, uint8_t *data, uint16_t dataLen)//функция чтения с изменением начало
{
    uint16_t dataToRead = readDataBuffer(buffer, data, dataLen);//полкучаю информацию из буффера

    buffer->pointBegin += dataToRead;//меняю начало

    if(buffer->pointBegin >= SIZE_BUFFER)//если размер начало больше размера то значить это следуйщий круг
        buffer->pointBegin -=SIZE_BUFFER;

    return dataToRead;
}

void bufferSetData(Buffer_t *buffer, uint8_t data)
{
    buffer->data[buffer->pointEnd++] = data;
    if(buffer->pointEnd == SIZE_BUFFER)
        buffer->pointEnd = 0;
}

void bufferSetDataMas(Buffer_t *buffer, uint8_t *data, uint16_t size)
{
    for(uint16_t i = 0 ; i < size; i++)
    {
        bufferSetData(buffer, data[i]);
    }
}
