#pragma once
#include "instruction_utils.h"
#include "logger.h"

namespace cpu
{
    template<uint8_t opcode>
    void Logger::ld_r_r_8(uint8_t opCode)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        printf("0x%2X : LD %s, %s;\n", opCode, 
            Registers::register8ToStr(operands.first).c_str(),
            Registers::register8ToStr(operands.second).c_str());
    }

    template<uint8_t opcode>
    void Logger::ld_r_n_8(uint8_t opCode)
    {
        constexpr Registers::Names opA = opAFromOpCode<opcode>();
        printf("0x%2X : LD %s, n;\n", opCode, Registers::register8ToStr(opA).c_str());
    }

    template<uint8_t opcode>
    void Logger::ld_r_HL(uint8_t opCode)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        printf("0x%2X : LD %s, (HL);\n", opCode, Registers::register8ToStr(operands.first).c_str());
    }

    template<uint8_t opcode>
    void Logger::ld_HL_r(uint8_t opCode)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        printf("0x%2X : LD (HL), %s;\n", opCode, Registers::register8ToStr(operands.first).c_str());
    }

    template<Registers::Paired NAME>
    void Logger::ld_A_r16(uint8_t opCode)
    {
        printf("0x%2X : LD A, (%s);\n", opCode, Registers::register16ToStr(NAME).c_str());
    }

    template<Registers::Paired NAME>
    void Logger::ld_r16_A(uint8_t opCode)
    {
        printf("0x%2X : LD (%s), A;\n", opCode, Registers::register16ToStr(NAME).c_str());
    }

    template<Registers::Paired NAME>
    void Logger::ld_rr_nn(uint8_t opCode)
    {
        printf("0x%2X : LD %s, nn;\n", opCode, Registers::register16ToStr(NAME).c_str());
    }

    template<Registers::Paired NAME>
    void Logger::push(uint8_t opCode)
    {
        printf("0x%2X : PUSH %s;\n", opCode, Registers::register16ToStr(NAME).c_str());
    }

    template<Registers::Paired NAME>
    void Logger::pop(uint8_t opCode)
    {
        printf("0x%2X : POP %s;\n", opCode, Registers::register16ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::add_r(uint8_t opCode)
    {
        printf("0x%2X : ADD %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::adc_r(uint8_t opCode)
    {
        printf("0x%2X : ADC %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::sub_r(uint8_t opCode)
    {
        printf("0x%2X : SUB %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::sbc_r(uint8_t opCode)
    {
        printf("0x%2X : SBC %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::cp_r(uint8_t opCode)
    {
        printf("0x%2X : CP %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::inc_r(uint8_t opCode)
    {
        printf("0x%2X : INC %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::dec_r(uint8_t opCode)
    {
        printf("0x%2X : DEC %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::and_r(uint8_t opCode)
    {
        printf("0x%2X : AND %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::or_r(uint8_t opCode)
    {
        printf("0x%2X : OR %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::xor_r(uint8_t opCode)
    {
        printf("0x%2X : XOR %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Paired NAME>
    void Logger::add_HL_rr(uint8_t opCode)
    {
        printf("0x%2X : ADD HL %s;\n", opCode, m_registers.register16ToStr(NAME).c_str());
    }

    template<Registers::Paired NAME>
    void Logger::inc_rr(uint8_t opCode)
    {
        printf("0x%2X : INC %s;\n", opCode, m_registers.register16ToStr(NAME).c_str());
    }

    template<Registers::Paired NAME>
    void Logger::dec_rr(uint8_t opCode)
    {
        printf("0x%2X : DEC %s;\n", opCode, m_registers.register16ToStr(NAME).c_str());
    }

    template<uint8_t n>
    void Logger::rst(uint8_t opCode)
    {
        printf("0x%2X : RST %u;\n", opCode, n);
    }

    template<Registers::Names NAME>
    void Logger::rlc_r(uint8_t opCode)
    {
        printf("0x%2X : RLC %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::rrc_r(uint8_t opCode)
    {
        printf("0x%2X : RRC %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::rl_r(uint8_t opCode)
    {
        printf("0x%2X : RL %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::rr_r(uint8_t opCode)
    {
        printf("0x%2X : RR %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::sla_r(uint8_t opCode)
    {
        printf("0x%2X : SLA %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::sra_r(uint8_t opCode)
    {
        printf("0x%2X : SRA %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void Logger::swap_r(uint8_t opCode)
    {
        printf("0x%2X : SWAP %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }

    template<Registers::Names NAME>
    void cpu::Logger::srl_r(uint8_t opCode)
    {
        printf("0x%2X : SRL %s;\n", opCode, m_registers.register8ToStr(NAME).c_str());
    }
   
    template<uint8_t n, Registers::Names NAME>
    void Logger::bit_n_r(uint8_t opCode)
    {
        printf("0x%2X : BIT %u, %s;\n", opCode, n, m_registers.register8ToStr(NAME).c_str());
    }
    
    template<uint8_t n>
    void Logger::bit_n_HL(uint8_t opCode)
    {
        printf("0x%2X : BIT %u, HL;\n", opCode, n);
    }

    template<uint8_t n, Registers::Names NAME>
    void Logger::res_n_r(uint8_t opCode)
    {
        printf("0x%2X : RES %u, %s;\n", opCode, n, m_registers.register8ToStr(NAME).c_str());
    }

    template<uint8_t n>
    void Logger::res_n_HL(uint8_t opCode)
    {
        printf("0x%2X : RES %u, HL;\n", opCode, n);
    }

    template<uint8_t n, Registers::Names NAME>
    void Logger::set_n_r(uint8_t opCode)
    {
        printf("0x%2X : SET %u, %s;\n", opCode, n, m_registers.register8ToStr(NAME).c_str());
    }

    template<uint8_t n>
    void Logger::set_n_HL(uint8_t opCode)
    {
        printf("0x%2X : SET %u, HL;\n", opCode, n);
    }
}
