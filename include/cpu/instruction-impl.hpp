#pragma once

#include "utils/global.h"

#include "registery.h"

#include <string>

class Memory;

namespace cpu
{
    template<uint8_t opcode>
    inline int Instructions::ld_r_r_8(uint16_t opA, uint16_t opB)
    {
        m_registers.incrementPC();

        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();
        uint8_t A = m_registers.read8<operands.first>();
        m_registers.write8<operands.second>(A);

        return 1;
    }

    template<uint8_t opcode>
    inline std::string Instructions::ld_r_r_8_dis(uint8_t opCode, uint16_t opA, uint16_t opB)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opCode) + " : LD" + Registers::register8ToStr(operands.first) + ", " +
            Registers::register8ToStr(operands.second) + "; \n";
    }
}
