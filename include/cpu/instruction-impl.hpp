#pragma once

namespace cpu
{
    template<uint8_t opcode>
    int Instructions::ld_r_r_8()
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();
        uint8_t A = m_registers.read8<operands.first>();
        m_registers.write8<operands.second>(A);

        return 1;
    }

    template<uint8_t opcode>
    std::string Instructions::ld_r_r_8_dis(uint8_t opCode)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opcode) + " : LD " + Registers::register8ToStr(operands.first) + ", " +
            Registers::register8ToStr(operands.second) + "; \n";
    }

    template<uint8_t opcode>
    int Instructions::ld_r_n_8()
    {
        uint8_t immediate = m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();

        constexpr Registers::Names opA = opAFromOpCode<opcode>();
        m_registers.write8<opA>(immediate);

        return 2;
    }

    template<uint8_t opcode>
    std::string Instructions::ld_r_n_8_dis(uint8_t opCode)
    {
        constexpr Registers::Names opA = opAFromOpCode<opcode>();

        return std::to_string(opcode) + " : LD " + Registers::register8ToStr(opA) + ", n; \n";
    }

    template<uint8_t opcode>
    int Instructions::ld_HL_n_8()
    {
        uint16_t addr = m_registers.read16<Registers::HL>();
        uint8_t immediate = m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();

        m_memory.write8(addr, immediate);

        return 3;
    }

    template<uint8_t opcode>
    std::string Instructions::ld_HL_n_8_dis(uint8_t opCode)
    {
        return std::to_string(opcode) + " : LD (HL), n; \n";
    }

    template<uint8_t opcode>
    inline int Instructions::ld_r_HL()
    {
        constexpr Registers::Names opA = opAFromOpCode<opcode>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);
        m_registers.write8<opA>(val);

        return 2;
    }

    template<uint8_t opcode>
    inline std::string Instructions::ld_r_HL_dis(uint8_t opCode)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opcode) + " : LD " + Registers::register8ToStr(operands.first) + ", (HL); \n";
    }

    template<uint8_t opcode>
    int Instructions::ld_HL_r()
    {
        constexpr Registers::Names opB = opBFromOpCode<opcode>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_registers.read8<opB>();
        m_memory.write8(hl, val);

        return 2;
    }

    template<uint8_t opcode>
    std::string Instructions::ld_HL_r_dis(uint8_t opCode)
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();

        return std::to_string(opcode) + " : LD (HL), " + Registers::register8ToStr(operands.first) + "; \n";
    }

    template<Registers::Paired NAME>
    int Instructions::ld_A_r16()
    {
        uint16_t addr = m_registers.read16<NAME>();
        uint8_t val = m_memory.read8(addr);

        m_registers.write8<Registers::A>(val);

        return 2;
    }

    template<Registers::Paired NAME>
    std::string Instructions::ld_A_r16_dis(uint8_t opCode)
    {
        return std::to_string(opCode) + " : LD A, (" + Registers::register16ToStr(NAME) + "); \n";
    }

    template<Registers::Paired NAME>
    int Instructions::ld_r16_A()
    {
        uint16_t addr = m_registers.read16<NAME>();
        uint8_t val = m_registers.read8<Registers::A> ();
        m_memory.write8(addr, val);

        return 2;
    }

    template<Registers::Paired NAME>
    std::string Instructions::ld_r16_A_dis(uint8_t opCode)
    {
        return std::to_string(opCode) + " : LD (" + Registers::register16ToStr(NAME) + "), A; \n";
    }

    template<Registers::Paired NAME>
    int Instructions::ld_rr_nn()
    {
        uint16_t val = m_memory.read16(m_registers.getPC());
        m_registers.incrementPC();
        m_registers.incrementPC();

        m_registers.write16<NAME>(val);

        return 3;
    }
    template<Registers::Paired NAME>
    std::string Instructions::ld_rr_nn_dis(uint8_t opCode)
    {
        return std::to_string(opCode) + " : LD rr, " + m_registers.register16ToStr(NAME) + ";\n";
    }

    template<Registers::Paired NAME>
    int Instructions::push()
    {
        uint16_t val = m_registers.read16<NAME>();
        uint16_t sp = m_registers.getSP();
        sp--;
        sp--;

        m_registers.setSP(sp);
        m_memory.write16(m_registers.getSP(), val);

        return 4;
    }
    template<Registers::Paired NAME>
    std::string Instructions::push_dis(uint8_t opCode)
    {
        return std::to_string(opCode) + " : PUSH " + m_registers.register16ToStr(NAME) + ";\n";
    }

    template<Registers::Paired NAME>
    int Instructions::pop()
    {
        uint16_t sp = m_registers.getSP();
        uint16_t val = m_memory.read16(sp);

        sp++;
        sp++;
        m_registers.setSP(sp);

        m_registers.write16<NAME>(val);
        return 3;                                                                                                                                                                                            
    }
    template<Registers::Paired NAME>
    std::string Instructions::pop_dis(uint8_t opCode)
    {
        return std::to_string(opCode) + " : POP " + m_registers.register16ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    int Instructions::add_r()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();
        int b = (int8_t) m_registers.read8<NAME>();
        add(a, b);

        return 1;
    }
    template<Registers::Names NAME>
    std::string Instructions::add_r_dis(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADD " + m_registers.register8ToStr(NAME) + ";\n";
    }

    template<Registers::Names NAME>
    int Instructions::adc_r()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();
        int b = (int8_t) m_registers.read8<NAME>();
        adc(a, b);

        return 1;
    }
    template<Registers::Names NAME>
    std::string Instructions::adc_r_dis(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADC " + m_registers.register8ToStr(NAME) + ";\n";
    }
}
