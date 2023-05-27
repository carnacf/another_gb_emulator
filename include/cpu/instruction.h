#pragma once

#include "utils/global.h"

#include "registery.h"

#include <string>

class Memory;

namespace cpu
{
class Registers;

struct InstructionData
{
    std::string opcode;
    std::string instruction;
};

class Instructions
{
public:
    using Instruction = int (Instructions::*)(uint16_t, uint16_t);
    using Disassembly = std::string (Instructions::*)(uint8_t, uint16_t, uint16_t);

    Instructions() = delete;
    Instructions(Registers& regist, Memory& mem);

    template<bool disassemble = false>
    void execute(uint8_t opCode);

private:
    void fillInstructionSet();
    void fillCbInstructionSet();

    void fillInstructionSetDisassembly();
    void fillCbInstructionSetDisassembly();
    
    int unhandled(uint16_t, uint16_t);
    std::string unhandledDisassembly(uint8_t opcode, uint16_t, uint16_t);

    int ld_BC_A(uint16_t opA, uint16_t opB);
    std::string ld_BC_A_dis(uint8_t opCode, uint16_t opA, uint16_t opB);

    template<uint8_t opcode>
    int ld_r_r_8(uint16_t opA, uint16_t opB)
    {
        m_registers.incrementPC();

        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();
        uint8_t A = m_registers.read8<operands.first>();
        m_registers.write8<operands.second>(A);

        return 1;
    }

    template<uint8_t opcode>
    std::string ld_r_r_8_dis(uint8_t opCode, uint16_t opA, uint16_t opB)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opCode) + " : LD" + Registers::register8ToStr(operands.first) + ", " +
            Registers::register8ToStr(operands.second) + "; \n";
    }

private:
    Registers& m_registers;
    Memory& m_memory;

    Instruction m_instructionSet[255];
    Instruction m_cbInstructionSet[255];

    Disassembly m_instructionSetDisassembly[255];
    Disassembly m_cbInstructionSetDisassembly[255];
};
}
