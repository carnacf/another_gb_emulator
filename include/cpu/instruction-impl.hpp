#pragma once

namespace cpu
{
    template<uint8_t opcode>
    inline int Instructions::ld_r_r_8(uint16_t, uint16_t)
    {
        m_registers.incrementPC();

        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();
        uint8_t A = m_registers.read8<operands.first>();
        m_registers.write8<operands.second>(A);

        return 1;
    }

    template<uint8_t opcode>
    inline std::string Instructions::ld_r_r_8_dis(uint8_t, uint16_t, uint16_t)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opCode) + " : LD" + Registers::register8ToStr(operands.first) + ", " +
            Registers::register8ToStr(operands.second) + "; \n";
    }

    template<uint8_t opcode>
    int Instructions::ld_r_n_8(uint16_t, uint16_t)
    {
        m_registers.incrementPC();

        uint8_t immediate = m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();

        constexpr Registers::Names opA = opAFromOpCode<opcode>();
        m_registers.write8<opA>(immediate);

        return 2;
    }

    template<uint8_t opcode>
    std::string Instructions::ld_r_n_8_dis(uint8_t, uint16_t, uint16_t)
    {
        constexpr Registers::Names opA = opAFromOpCode<opcode>();

        return std::to_string(opCode) + " : LD" + Registers::register8ToStr(opA) + ", n; \n";
    }

    template<uint8_t opcode>
    inline int Instructions::ld_r_HL(uint16_t, uint16_t)
    {
        m_register.incrementPC();

        constexpr Registers::Names opA = opAFromOpCode<opcode>();
        uint16_t hl = m_registers.read16<HL>();
        uint8_t val = m_memory.read8(hl);
        m_registers.write8<opA>(val);

        return 2;
    }

    template<uint8_t opcode>
    inline std::string Instructions::ld_r_HL_dis(uint8_t, uint16_t, uint16_t)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opCode) + " : LD" + Registers::register8ToStr(operands.first) + ", (HL); \n";
    }

    template<uint8_t opcode>
    int Instructions::ld_HL_r(uint16_t, uint16_t)
    {
        m_register.incrementPC();

        constexpr Registers::Names opB = opBFromOpCode<opcode>();
        uint16_t hl = m_registers.read16<HL>();
        uint8_t val = m_registers.read8<opB>(val);
        m_memory.write8(hl, val);

        return 2;
    }

    template<uint8_t opcode>
    std::string Instructions::ld_HL_r_dis(uint8_t, uint16_t, uint16_t)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opCode) + " : LD (HL), " + Registers::register8ToStr(operands.first) + "; \n";
    }
}
