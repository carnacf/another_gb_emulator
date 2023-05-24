#pragma once 

#include "../utils/global.h"
#include "../utils/utils.h"

class Memory
{
public:
    FORCEINLINE void write8(uint16_t index, uint8_t val)
    {
        memoryMap[index] = val;
    };

    FORCEINLINE uint8_t read8(uint16_t index)
    {
        return memoryMap[index];
    }

    FORCEINLINE void write16(uint16_t index, uint16_t val)
    {
        memoryMap[index] = low(val);
        memoryMap[index + 1] = high(val); 
    }

    FORCEINLINE uint16_t read16(uint16_t index)
    {
        return (uint16_t) ((((uint16_t) memoryMap[index + 1]) << 8) & memoryMap[index]);
    }

private:
    uint8_t memoryMap[0x10000];
};