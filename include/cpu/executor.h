#pragma once

#include "utils/global.h"

#include "tracer.h"
#include "registery.h"

#include <string>

class Memory;

namespace cpu
{
class Registers;

class Executor
{
public:
    using Instruction = int (Executor::*)();

    Executor() = delete;
    Executor(Registers& regist, Memory& mem);

    template<bool disassemble>
    int execute(uint8_t opCode)
    {
        m_registers.incrementPC();
        int numberOfCycles = (this->*m_instructionSet[opCode])();
        if constexpr (disassemble)
        {
            std::cout << m_tracer(opCode);
        }

        return numberOfCycles;
    }

private:
    void fillInstructionSet();
    void fillCbInstructionSet();
    
    int unhandled();

    // 8bit Loads
    template<uint8_t opcode>
    int ld_r_r_8();
    template<uint8_t opcode>
    int ld_r_n_8();
    template<uint8_t opcode>
    int ld_HL_n_8();
    template<uint8_t opcode>
    int ld_r_HL();
    template<uint8_t opcode>
    int ld_HL_r();
    template<Registers::Paired NAME>
    int ld_A_r16();
    template<Registers::Paired NAME>
    int ld_r16_A();
    int ld_A_nn();
    int ld_nn_A();
    int ldh_A_aC();
    int ldh_aC_A();
    int ldh_A_an();
    int ldh_an_A();
    int ld_HLd_A();
    int ld_A_HLd();
    int ld_HLi_A();
    int ld_A_HLi();

    // 16bits loads
    template<Registers::Paired NAME>
    int ld_rr_nn();
    int ld_SP_rr();
    int ld_nn_SP();
    int ld_SP_HL();
    int ld_HL_SP_r8();

    template<Registers::Paired NAME>
    int push();
    template<Registers::Paired NAME>
    int pop();

    // 8-bit arithmetic and logical instructions
    void updateFlags(int carryBits, uint8_t res, bool isSub = false)
    {
        bool hc = (carryBits & 0x10) == 0x10;
        bool c = (carryBits & 0x100) == 0x100;
        m_registers.setFlag(Registers::Flag::H, hc);
        m_registers.setFlag(Registers::Flag::C, c);
        m_registers.setFlag(Registers::Flag::Z, res == 0);
        m_registers.setFlag(Registers::Flag::N, isSub);
    }

    void add(int a, int b);
    template<Registers::Names NAME>
    int add_r();
    int add_HL();
    int add_n();

    void adc(int a, int b);
    template<Registers::Names NAME>
    int adc_r();
    int adc_HL();
    int adc_n();

    void sub(int a, int b);
    template<Registers::Names NAME>
    int sub_r();
    int sub_HL();
    int sub_n();

    void sbc(int a, int b);
    template<Registers::Names NAME>
    int sbc_r();
    int sbc_HL();
    int sbc_n();
private:
    Tracer m_tracer;
    Registers& m_registers;
    Memory& m_memory;

    Instruction m_instructionSet[255];
    Instruction m_cbInstructionSet[255];
};
}

#include "executor-impl.hpp"
