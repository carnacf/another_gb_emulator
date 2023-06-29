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
    using Instruction = int (Instructions::*)();
    using Disassembly = std::string (Instructions::*)(uint8_t);

    Instructions() = delete;
    Instructions(Registers& regist, Memory& mem);

    template<bool disassemble>
    int execute(uint8_t opCode)
    {
        m_registers.incrementPC();
        int numberOfCycles = (this->*m_instructionSet[opCode])();
        if constexpr (disassemble)
        {
            std::cout << (this->*m_instructionSetDisassembly[opCode])(opCode);
        }

        return numberOfCycles;
    }

private:
    void fillInstructionSet();
    void fillCbInstructionSet();

    void fillInstructionSetDisassembly();
    void fillCbInstructionSetDisassembly();
    
    int unhandled();
    std::string unhandledDisassembly(uint8_t opcode);

    // 8bit Loads
    template<uint8_t opcode>
    int ld_r_r_8();
    template<uint8_t opcode>
    std::string ld_r_r_8_dis(uint8_t opCode);

    template<uint8_t opcode>
    int ld_r_n_8();
    template<uint8_t opcode>
    std::string ld_r_n_8_dis(uint8_t opCode);

    template<uint8_t opcode>
    int ld_HL_n_8();
    template<uint8_t opcode>
    std::string ld_HL_n_8_dis(uint8_t opCode);

    template<uint8_t opcode>
    int ld_r_HL();
    template<uint8_t opcode>
    std::string ld_r_HL_dis(uint8_t opCode);

    template<uint8_t opcode>
    int ld_HL_r();
    template<uint8_t opcode>
    std::string ld_HL_r_dis(uint8_t opCode);

    template<Registers::Paired NAME>
    int ld_A_r16();
    template<Registers::Paired NAME>
    std::string ld_A_r16_dis(uint8_t opCode);

    template<Registers::Paired NAME>
    int ld_r16_A();
    template<Registers::Paired NAME>
    std::string ld_r16_A_dis(uint8_t opCode);

    int ld_A_nn();
    std::string ld_A_nn_dis(uint8_t opCode);

    int ld_nn_A();
    std::string ld_nn_A_dis(uint8_t opCode);

    int ldh_A_aC();
    std::string ldh_A_aC_dis(uint8_t opCode);

    int ldh_aC_A();
    std::string ldh_aC_A_dis(uint8_t opCode);

    int ldh_A_an();
    std::string ldh_A_an_dis(uint8_t opCode);

    int ldh_an_A();
    std::string ldh_an_A_dis(uint8_t opCode);

    int ld_HLd_A();
    std::string ld_HLd_A_dis(uint8_t opCode);

    int ld_A_HLd();
    std::string ld_A_HLd_dis(uint8_t opCode);

    int ld_HLi_A();
    std::string ld_HLi_A_dis(uint8_t opCode);

    int ld_A_HLi();
    std::string ld_A_HLi_dis(uint8_t opCode);

    // 16bits loads
    template<Registers::Paired NAME>
    int ld_rr_nn();
    template<Registers::Paired NAME>
    std::string ld_rr_nn_dis(uint8_t opCode);
    
    int ld_SP_rr();
    std::string ld_SP_rr_dis(uint8_t opCode);

    int ld_nn_SP();
    std::string ld_nn_SP_dis(uint8_t opCode);

    int ld_SP_HL();
    std::string ld_SP_HL_dis(uint8_t opCode);

    int ld_HL_SP_r8();
    std::string ld_HL_SP_r8_dis(uint8_t opCode);

    template<Registers::Paired NAME>
    int push();
    template<Registers::Paired NAME>
    std::string push_dis(uint8_t opCode);

    template<Registers::Paired NAME>
    int pop();
    template<Registers::Paired NAME>
    std::string pop_dis(uint8_t opCode);

    // 8-bit arithmetic and logical instructions
    void updateFlagsAdd(int carryBits, uint8_t res)
    {
        m_registers.resetFlags();

        bool hc = (carryBits & 0x10) == 0x10;
        bool c = (carryBits & 0x100) == 0x100;
        m_registers.setFlag(Registers::Flag::H, hc);
        m_registers.setFlag(Registers::Flag::C, c);
        m_registers.setFlag(Registers::Flag::Z, res == 0);
    }

    void add(int a, int b);
    template<Registers::Names NAME>
    int add_r();
    template<Registers::Names NAME>
    std::string add_r_dis(uint8_t opCode);
    
    int add_HL();
    std::string add_HL_dis(uint8_t opCode);

    int add_n();
    std::string add_n_dis(uint8_t opCode);

    void adc(int a, int b);
    template<Registers::Names NAME>
    int adc_r();
    template<Registers::Names NAME>
    std::string adc_r_dis(uint8_t opCode);

    int adc_HL();
    std::string adc_HL_dis(uint8_t opCode);

    int adc_n();
    std::string adc_n_dis(uint8_t opCode);
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
