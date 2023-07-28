#pragma once

#include "utils/global.h"

#include "logger.h"
#include "registery.h"

#include <string>
#include <optional>

class Memory;

namespace cpu
{
enum class Interrupt
{
    VBlank,
    LCD_STAT,
    Timer,
    Serial,
    Joypad
};

class Registers;

class Processor
{
public:
    using Instruction = int (Processor::*)();

    Processor() = delete;
    Processor(Registers& regist, Memory& mem);

    void runNextInstruction(bool trace);

    void handleInterrupt(Interrupt interruptType);
    std::optional<Interrupt> pendingInterrupt() const;
    void updateClocks(int ticks);
private:
    void fillInstructionSet();
    void fillCbInstructionSet();
    
    std::optional<int> getTIMANbCycles() const;
    int unhandled();

    uint8_t getImmediate8();
    uint16_t getImmediate16();

    int nop();
    int cb();

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
    int ld_SP_nn();
    int ld_nn_SP();
    int ld_SP_HL();
    int ld_HL_SP_r8();

    void push(uint16_t val);
    uint16_t pop();
    template<Registers::Paired NAME>
    int push_rr();
    template<Registers::Paired NAME>
    int pop_rr();

    void updateFlags(uint16_t res, bool n, bool h, bool c)
    {
        m_registers.setFlag(Registers::Flag::Z, res == 0);
        m_registers.setFlag(Registers::Flag::N, n);
        m_registers.setFlag(Registers::Flag::H, h);
        m_registers.setFlag(Registers::Flag::C, c);
    }
    // 8-bit arithmetic and logical instructions
    void updateFlagsWithCarry8bit(int carryBits, uint8_t res, bool carryFlag, bool n)
    {
        bool h = (carryBits & 0x10) == 0x10;
        bool c = carryFlag && ((carryBits & 0x100) == 0x100);
        
        updateFlags(res, n, h, c);
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

    void cp(int a, int b);
    template<Registers::Names NAME>
    int cp_r();
    int cp_HL();
    int cp_n();

    template<Registers::Names NAME>
    int inc_r();
    int inc_HL();

    template<Registers::Names NAME>
    int dec_r();
    int dec_HL();

    template<Registers::Names NAME>
    int and_r();
    int and_HL();
    int and_n();

    template<Registers::Names NAME>
    int or_r();
    int or_HL();
    int or_n();

    template<Registers::Names NAME>
    int xor_r();
    int xor_HL();
    int xor_n();

    int scf();
    int ccf();
    int cpl();
    int daa();

    // 16-bit arithmetic
    void updateFlagsWithCarry16bit(int carryBits, bool carryFlag, bool n)
    {
        bool h = (carryBits & 0x1000) == 0x1000;
        bool c = carryFlag && ((carryBits & 0x10000) == 0x10000);

        m_registers.setFlag(Registers::Flag::N, n);
        m_registers.setFlag(Registers::Flag::H, h);
        m_registers.setFlag(Registers::Flag::C, c);
    }

    template<Registers::Paired NAME>
    int add_HL_rr();
    int add_HL_SP();
    int add_SP_n();
    template<Registers::Paired NAME>
    int inc_rr();
    int inc_SP();
    template<Registers::Paired NAME>
    int dec_rr();
    int dec_SP();

    // Rotates and shifts
    int rlca();
    template<Registers::Names NAME>
    int rlc_r();
    int rlc_HL();

    int rla();
    template<Registers::Names NAME>
    int rl_r();
    int rl_HL();

    int rrca();
    template<Registers::Names NAME>
    int rrc_r();
    int rrc_HL();

    int rra();
    template<Registers::Names NAME>
    int rr_r();
    int rr_HL();

    template<Registers::Names NAME>
    int sla_r();
    int sla_HL();

    template<Registers::Names NAME>
    int sra_r();
    int sra_HL();

    template<Registers::Names NAME>
    int srl_r();
    int srl_HL();

    template<Registers::Names NAME>
    int swap_r();
    int swap_HL();

    // Single - bit Operation instructions
    void bit_n(uint8_t n, uint16_t r);
    template<uint8_t n, Registers::Names NAME>
    int bit_n_r();
    template<uint8_t n>
    int bit_n_HL();

    template<uint8_t n, Registers::Names NAME>
    int res_n_r();
    template<uint8_t n>
    int res_n_HL();

    template<uint8_t n, Registers::Names NAME>
    int set_n_r();
    template<uint8_t n>
    int set_n_HL();

    // Control flow instructions
    int jp_nn();
    int jp_HL();
    template<Registers::Flag f>
    int jp_cc_nn();
    template<Registers::Flag f>
    int jp_ncc_nn();

    int jr_e();
    template<Registers::Flag f>
    int jr_cc_n();
    template<Registers::Flag f>
    int jr_ncc_n();

    int call_nn();
    template<Registers::Flag f>
    int call_cc_nn();
    template<Registers::Flag f>
    int call_ncc_nn();

    int ret();
    template<Registers::Flag f>
    int ret_cc();
    template<Registers::Flag f>
    int ret_ncc();
    int reti();

    template<uint8_t n>
    int rst();

    int di();
    int ei();
    int halt();
    int stop();

private:
    Logger m_tracer;
    Registers& m_registers;
    Memory& m_memory;

    Instruction m_instructionSet[256];
    Instruction m_cbInstructionSet[256];

    int m_divCycleCounter = 0;
    int m_timaCycleCounter = 0;

    bool m_IME = false;
    bool m_isHalt = false;
    bool m_isStopped = false;
};
}

#include "processor-impl.hpp"
