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

        return std::to_string(opcode) + " : LD " + Registers::register8ToStr(operands.first) + ", " +
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

        return std::to_string(opcode) + " : LD " + Registers::register8ToStr(opA) + ", n; \n";
    }

    template<uint8_t opcode>
    int Instructions::ld_HL_n_8(uint16_t, uint16_t)
    {
        m_registers.incrementPC();

        uint16_t addr = m_registers.read16<Registers::HL>();
        uint8_t immediate = m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();

        m_memory.write8(addr, immediate);

        return 3;
    }

    template<uint8_t opcode>
    std::string Instructions::ld_HL_n_8_dis(uint8_t, uint16_t, uint16_t)
    {
        return std::to_string(opcode) + " : LD (HL), n; \n";
    }

    template<uint8_t opcode>
    inline int Instructions::ld_r_HL(uint16_t, uint16_t)
    {
        m_registers.incrementPC();

        constexpr Registers::Names opA = opAFromOpCode<opcode>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);
        m_registers.write8<opA>(val);

        return 2;
    }

    template<uint8_t opcode>
    inline std::string Instructions::ld_r_HL_dis(uint8_t, uint16_t, uint16_t)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opcode) + " : LD " + Registers::register8ToStr(operands.first) + ", (HL); \n";
    }

    template<uint8_t opcode>
    int Instructions::ld_HL_r(uint16_t, uint16_t)
    {
        m_registers.incrementPC();

        constexpr Registers::Names opB = opBFromOpCode<opcode>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_registers.read8<opB>();
        m_memory.write8(hl, val);

        return 2;
    }

    template<uint8_t opcode>
    std::string Instructions::ld_HL_r_dis(uint8_t, uint16_t, uint16_t)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opcode) + " : LD (HL), " + Registers::register8ToStr(operands.first) + "; \n";
    }

    template<Registers::Paired NAME>
    int Instructions::ld_A_r16(uint16_t, uint16_t)
    {
        m_registers.incrementPC();
        uint16_t addr = m_registers.read16<NAME>();
        uint8_t val = m_memory.read8(addr);

        m_registers.write8<Registers::A>(val);

        return 2;
    }

    template<Registers::Paired NAME>
    std::string Instructions::ld_A_r16_dis(uint8_t opCode, uint16_t, uint16_t)
    {
        return std::to_string(opCode) + " : LD A, (" + Registers::register16ToStr(NAME) + "); \n";
    }

    template<Registers::Paired NAME>
    int Instructions::ld_r16_A(uint16_t, uint16_t)
    {
        m_registers.incrementPC();

        uint16_t addr = m_registers.read16<NAME>();
        uint8_t val = m_registers.read8<Registers::A> ();
        m_memory.write8(addr, val);

        return 2;
    }

    template<Registers::Paired NAME>
    std::string Instructions::ld_r16_A_dis(uint8_t opCode, uint16_t opA, uint16_t opB)
    {
        return std::to_string(opCode) + " : LD (" + Registers::register16ToStr(NAME) + "), A; \n";
    }
}
