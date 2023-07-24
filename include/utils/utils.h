#pragma once

#include "global.h"

#include <cstdint>

namespace utils
{

FORCEINLINE inline uint8_t high(uint16_t val)
{
    return (uint8_t) (val >> 8);
}

FORCEINLINE inline uint8_t low(uint16_t val)
{
    return (uint8_t) (val);
}

FORCEINLINE inline uint16_t to16(uint8_t msb, uint8_t lsb)
{
    return ((uint16_t)(msb) << 8) | (uint16_t)(lsb);
}

FORCEINLINE inline bool testBit(uint8_t val, uint8_t bit)
{
    return (val & (1 << bit)) != 0;
}

FORCEINLINE inline uint8_t setBit(uint8_t val, uint8_t bit)
{
    return val | (1 << bit);
}

FORCEINLINE inline uint8_t resetBit(uint8_t val, uint8_t bit)
{
    return val & ~(1 << bit);
}
}
