#pragma once
#include "instruction_utils.h"

namespace cpu
{
    template<uint8_t opcode>
    int Executor::ld_r_r_8()
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();
        uint8_t A = m_registers.read8<operands.first>();
        m_registers.write8<operands.second>(A);

        return 1;
    }

    template<uint8_t opcode>
    int Executor::ld_r_n_8()
    {
        uint8_t immediate = getImmediate8();

        constexpr Registers::Names opA = opAFromOpCode<opcode>();
        m_registers.write8<opA>(immediate);

        return 2;
    }

    template<uint8_t opcode>
    int Executor::ld_HL_n_8()
    {
        uint16_t addr = m_registers.read16<Registers::HL>();
        uint8_t immediate = getImmediate8();

        m_memory.write8(addr, immediate);

        return 3;
    }

    template<uint8_t opcode>
    inline int Executor::ld_r_HL()
    {
        constexpr Registers::Names opA = opAFromOpCode<opcode>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);
        m_registers.write8<opA>(val);

        return 2;
    }

    template<uint8_t opcode>
    int Executor::ld_HL_r()
    {
        constexpr Registers::Names opB = opBFromOpCode<opcode>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_registers.read8<opB>();
        m_memory.write8(hl, val);

        return 2;
    }

    template<Registers::Paired NAME>
    int Executor::ld_A_r16()
    {
        uint16_t addr = m_registers.read16<NAME>();
        uint8_t val = m_memory.read8(addr);

        m_registers.write8<Registers::A>(val);

        return 2;
    }

    template<Registers::Paired NAME>
    int Executor::ld_r16_A()
    {
        uint16_t addr = m_registers.read16<NAME>();
        uint8_t val = m_registers.read8<Registers::A> ();
        m_memory.write8(addr, val);

        return 2;
    }

    template<Registers::Paired NAME>
    int Executor::ld_rr_nn()
    {
        uint16_t val = getImmediate16();

        m_registers.write16<NAME>(val);

        return 3;
    }

    template<Registers::Paired NAME>
    int Executor::push_rr()
    {
        uint16_t val = m_registers.read16<NAME>();
        push(val);

        return 4;
    }

    template<Registers::Paired NAME>
    int Executor::pop_rr()
    {
        uint16_t val = pop();
        m_registers.write16<NAME>(val);
        
        return 3;                                                                                                                                                                                            
    }

    template<Registers::Names NAME>
    int Executor::add_r()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();
        int b = (int8_t) m_registers.read8<NAME>();
        add(a, b);

        return 1;
    }

    template<Registers::Names NAME>
    int Executor::adc_r()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();
        int b = (int8_t) m_registers.read8<NAME>();
        adc(a, b);

        return 1;
    }
    
    template<Registers::Names NAME>
    int Executor::sub_r()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();
        int b = (int8_t)m_registers.read8<NAME>();
        sub(a, b);

        return 1;
    }

    template<Registers::Names NAME>
    int Executor::sbc_r()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();
        int b = (int8_t)m_registers.read8<NAME>();
        sbc(a, b);

        return 1;
    }

    template<Registers::Names NAME>
    int Executor::cp_r()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();
        int b = (int8_t)m_registers.read8<NAME>();
        cp(a, b);

        return 1;
    }

    template<Registers::Names NAME>
    int Executor::inc_r()
    {
        int b = (int8_t)m_registers.read8<NAME>();
        int8_t r = b + 1;
        m_registers.write8<NAME>((uint8_t)r);

        int carryBits = b ^ 1 ^ r;
        updateFlagsWithCarry8bit(carryBits, r, false, false);

        return 1;
    }

    template<Registers::Names NAME>
    int Executor::dec_r()
    {
        int b = (int8_t)m_registers.read8<NAME>();
        int8_t r = b - 1;
        m_registers.write8<NAME>((uint8_t)r);

        int carryBits = b ^ 1 ^ r;
        updateFlagsWithCarry8bit(carryBits, r, false, true);

        return 1;
    }

    template<Registers::Names NAME>
    int Executor::and_r()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        uint8_t b = m_registers.read8<NAME>();
        uint8_t r = a & b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, true, false);

        return 1;
    }

    template<Registers::Names NAME>
    int Executor::or_r()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        uint8_t b = m_registers.read8<NAME>();
        uint8_t r = a | b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, false, false);

        return 1;
    }

    template<Registers::Names NAME>
    int Executor::xor_r()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        uint8_t b = m_registers.read8<NAME>();
        uint8_t r = a ^ b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, false, false);

        return 1;
    }

    template<Registers::Paired NAME>
    int Executor::add_HL_rr()
    {
        int hl = m_registers.read16<Registers::HL>();
        int rr = m_registers.read16<NAME>();
        int res = hl + rr;

        int carryBits = hl ^ rr ^ res;
        updateFlagsWithCarry16bit(carryBits, true, false);

        m_registers.write16<Registers::HL>((uint16_t) res);

        return 2;
    }

    template<Registers::Paired NAME>
    int Executor::inc_rr()
    {
        uint16_t rr = m_registers.read16<NAME>();
        rr++;
        m_registers.write16<NAME>(rr);

        return 2;
    }

    template<Registers::Paired NAME>
    int Executor::dec_rr()
    {
        uint16_t rr = m_registers.read16<NAME>();
        rr--;
        m_registers.write16<NAME>(rr);

        return 2;
    }

    template<Registers::Flag f>
    int Executor::jp_cc_nn()
    {
        uint16_t immediate = getImmediate16();
        if (m_registers.isSetFlag(f))
        {
            m_registers.setPC(immediate);
            return 4;
        }
        else
        {
            return 3;
        }
    }

    template<Registers::Flag f>
    int Executor::jp_ncc_nn()
    {
        uint16_t immediate = getImmediate16();
        if (!m_registers.isSetFlag(f))
        {
            m_registers.setPC(immediate);
            return 4;
        }
        else
        {
            return 3;
        }
    }

    template<Registers::Flag f>
    int Executor::jr_cc_n()
    {
        int8_t immediate = (int8_t) getImmediate8();
        if (m_registers.isSetFlag(f))
        {
            uint16_t pc = m_registers.getPC();
            m_registers.setPC(pc + immediate);
            return 3;
        }
        else
        {
            return 2;
        }
    }

    template<Registers::Flag f>
    int Executor::jr_ncc_n()
    {
        int8_t immediate = (int8_t)getImmediate8();
        if (!m_registers.isSetFlag(f))
        {
            uint16_t pc = m_registers.getPC();
            m_registers.setPC(pc + immediate);
            return 3;
        }
        else
        {
            return 2;
        }
    }

    template<Registers::Flag f>
    int Executor::call_cc_nn()
    {
        uint16_t addr = getImmediate16();
        if (m_registers.isSetFlag(f))
        {
            uint16_t pc = m_registers.getPC();
            push(pc);

            m_registers.setPC(addr);
            return 6;
        }
        else
        {
            return 3;
        }
    }

    template<Registers::Flag f>
    int Executor::call_ncc_nn()
    {
        uint16_t addr = getImmediate16();
        if (!m_registers.isSetFlag(f))
        {
            uint16_t pc = m_registers.getPC();
            push(pc);

            m_registers.setPC(addr);
            return 6;
        }
        else
        {
            return 3;
        }
    }
}
