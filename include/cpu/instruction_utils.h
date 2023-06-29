#pragma once
#include "registery.h"

namespace cpu
{
    template<uint8_t opcode>
    constexpr Registers::Names opAFromOpCode()
    {
        constexpr uint8_t opAMask = 0b0011'1000;
        constexpr uint8_t opA = (opAMask & opcode) >> 3;

        return Registers::opFieldToName<opA>();
    }

    template<uint8_t opcode>
    constexpr Registers::Names opBFromOpCode()
    {
        constexpr uint8_t opBMask = 0b0000'0111;
        constexpr uint8_t opB = opBMask & opcode;

        return Registers::opFieldToName<opB>();
    }

    template<uint8_t opcode>
    constexpr std::pair<Registers::Names, Registers::Names> extractLDRROperands()
    {
        constexpr Registers::Names rA = opAFromOpCode<opcode>();
        constexpr Registers::Names rB = opBFromOpCode<opcode>();

        return { rA, rB };
    }
}