#pragma once

#include "global.h"

#include <cstdint>

namespace utils
{

FORCEINLINE inline uint8_t high(uint16_t val)
{
    return (uint8_t) (val & 0xff);
}

FORCEINLINE inline uint8_t low(uint16_t val)
{
    return (uint8_t) (val >> 8);
}
}
