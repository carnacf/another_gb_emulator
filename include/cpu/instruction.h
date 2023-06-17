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

    template<bool disassemble>
    int execute(uint8_t opCode)
    {
        int numberOfCycles = (this->*m_instructionSet[opCode])(0, 0);
        if constexpr (disassemble)
        {
            std::cout << (this->*m_instructionSetDisassembly[opCode])(opCode, 0, 0);
        }

        return numberOfCycles;
    }

private:
    void fillInstructionSet();
    void fillCbInstructionSet();

    void fillInstructionSetDisassembly();
    void fillCbInstructionSetDisassembly();
    
    int unhandled(uint16_t, uint16_t);
    std::string unhandledDisassembly(uint8_t opcode, uint16_t, uint16_t);

    template<uint8_t opcode>
    int ld_r_r_8(uint16_t opA, uint16_t opB);
    template<uint8_t opcode>
    std::string ld_r_r_8_dis(uint8_t opCode, uint16_t opA, uint16_t opB);

    template<uint8_t opcode>
    int ld_r_n_8(uint16_t opA, uint16_t opB);
    template<uint8_t opcode>
    std::string ld_r_n_8_dis(uint8_t opCode, uint16_t opA, uint16_t opB);

    template<uint8_t opcode>
    int ld_HL_n_8(uint16_t opA, uint16_t opB);
    template<uint8_t opcode>
    std::string ld_HL_n_8_dis(uint8_t opCode, uint16_t opA, uint16_t opB);

    template<uint8_t opcode>
    int ld_r_HL(uint16_t opA, uint16_t opB);
    template<uint8_t opcode>
    std::string ld_r_HL_dis(uint8_t opCode, uint16_t opA, uint16_t opB);

    template<uint8_t opcode>
    int ld_HL_r(uint16_t opA, uint16_t opB);
    template<uint8_t opcode>
    std::string ld_HL_r_dis(uint8_t opCode, uint16_t opA, uint16_t opB);

    template<Registers::Paired NAME>
    int ld_A_r16(uint16_t opA, uint16_t opB);
    template<Registers::Paired NAME>
    std::string ld_A_r16_dis(uint8_t opCode, uint16_t opA, uint16_t opB);

    template<Registers::Paired NAME>
    int ld_r16_A(uint16_t opA, uint16_t opB);
    template<Registers::Paired NAME>
    std::string ld_r16_A_dis(uint8_t opCode, uint16_t opA, uint16_t opB);

    int ld_A_nn(uint16_t opA, uint16_t opB);
    std::string ld_A_nn_dis(uint8_t, uint16_t, uint16_t);

    int ld_nn_A(uint16_t opA, uint16_t opB);
    std::string ld_nn_A_dis(uint8_t, uint16_t, uint16_t);

private:
    Registers& m_registers;
    Memory& m_memory;

    Instruction m_instructionSet[255];
    Instruction m_cbInstructionSet[255];

    Disassembly m_instructionSetDisassembly[255];
    Disassembly m_cbInstructionSetDisassembly[255];
};
}

#include "instruction-impl.hpp"
