#pragma once
#include "instruction_utils.h"
#include "logger.h"

namespace cpu
{
    template<uint8_t opcode>
    std::string Logger::ld_r_r_8(uint8_t opCode)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opcode) + " : LD " + Registers::register8ToStr(operands.first) + ", " +
            Registers::register8ToStr(operands.second) + "; \n";
    }

    template<uint8_t opcode>
    std::string Logger::ld_r_n_8(uint8_t opCode)
    {
        constexpr Registers::Names opA = opAFromOpCode<opcode>();

        return std::to_string(opcode) + " : LD " + Registers::register8ToStr(opA) + ", n; \n";
    }

    template<uint8_t opcode>
    std::string Logger::ld_HL_n_8(uint8_t opCode)
    {
        return std::to_string(opcode) + " : LD (HL), n; \n";
    }

    template<uint8_t opcode>
    std::string Logger::ld_r_HL(uint8_t opCode)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opcode) + " : LD " + Registers::register8ToStr(operands.first) + ", (HL); \n";
    }

    template<uint8_t opcode>
    std::string Logger::ld_HL_r(uint8_t opCode)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opcode) + " : LD (HL), " + Registers::register8ToStr(operands.first) + "; \n";
    }

    template<Registers::Paired NAME>
    std::string Logger::ld_A_r16(uint8_t opCode)
    {
        return std::to_string(opCode) + " : LD A, (" + Registers::register16ToStr(NAME) + "); \n";
    }

    template<Registers::Paired NAME>
    std::string Logger::ld_r16_A(uint8_t opCode)
    {
        return std::to_string(opCode) + " : LD (" + Registers::register16ToStr(NAME) + "), A; \n";
    }

    template<Registers::Paired NAME>
    std::string Logger::ld_rr_nn(uint8_t opCode)
    {
        return std::to_string(opCode) + " : LD rr, " + m_registers.register16ToStr(NAME) + ";\n";
    }

    template<Registers::Paired NAME>
    std::string Logger::push(uint8_t opCode)
    {
        return std::to_string(opCode) + " : PUSH " + m_registers.register16ToStr(NAME) + ";\n";
    }

    template<Registers::Paired NAME>
    std::string Logger::pop(uint8_t opCode)
    {
        return std::to_string(opCode) + " : POP " + m_registers.register16ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::add_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADD " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::adc_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADC " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::sub_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : SUB " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::sbc_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : SBC " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::cp_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : CP " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::inc_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : INC " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::dec_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : DEC " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::and_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : AND " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::or_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : OR " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::xor_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : XOR " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Paired NAME>
    std::string Logger::add_HL_rr(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADD HL " + m_registers.register16ToStr(NAME) + ";\n";
    }

    template<Registers::Paired NAME>
    std::string Logger::inc_rr(uint8_t opCode)
    {
        return std::to_string(opCode) + " : INC " + m_registers.register16ToStr(NAME) + ";\n";
    }

    template<Registers::Paired NAME>
    std::string Logger::dec_rr(uint8_t opCode)
    {
        return std::to_string(opCode) + " : DEC " + m_registers.register16ToStr(NAME) + ";\n";
    }

    template<uint8_t n>
    std::string Logger::rst(uint8_t opCode)
    {
        return std::to_string(opCode) + " : RST " + std::to_string(n) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::rlc_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : RLC " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::rrc_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : RRC " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::rl_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : RL " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::rr_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : RR " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::sla_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : SLA " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::sra_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : SRA " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string Logger::swap_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : SWAP " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    std::string cpu::Logger::srl_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : SRL " + m_registers.register8ToStr(NAME) + ";\n";
    }
    template<uint8_t n, Registers::Names NAME>
    std::string Logger::bit_n_r(uint8_t opCode)
    {
        return std::to_string(opCode) + " : BIT " + std::to_string(n)+ ", " + m_registers.register8ToStr(NAME) + ";\n";
    }
    template<uint8_t n>
    std::string Logger::bit_n_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : BIT " + std::to_string(n) + ", HL;\n";;
    }
}
