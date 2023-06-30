#pragma once
#include "instruction_utils.h"

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
}
