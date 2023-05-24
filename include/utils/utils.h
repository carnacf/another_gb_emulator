#pragma once

#include "global.h"

FORCEINLINE uint8_t high(uint16_t val)
{
    return (uint8_t) (val & 0xff);
}

FORCEINLINE uint8_t low(uint16_t val)
{
    return (uint8_t) (val >> 8);
}