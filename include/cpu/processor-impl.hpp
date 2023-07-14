#pragma once

#include "instruction_utils.h"

namespace cpu
{
    template<uint8_t opcode>
    int Processor::ld_r_r_8()
    {
        constexpr std::pair<Registers::Names, Registers::Names> operands = extractLDRROperands<opcode>();
        uint8_t A = m_registers.read8<operands.first>();
        m_registers.write8<operands.second>(A);

        return 1;
    }

    template<uint8_t opcode>
    int Processor::ld_r_n_8()
    {
        uint8_t immediate = getImmediate8();

        constexpr Registers::Names opA = opAFromOpCode<opcode>();
        m_registers.write8<opA>(immediate);

        return 2;
    }

    template<uint8_t opcode>
    int Processor::ld_HL_n_8()
    {
        uint16_t addr = m_registers.read16<Registers::HL>();
        uint8_t immediate = getImmediate8();

        m_memory.write8(addr, immediate);

        return 3;
    }

    template<uint8_t opcode>
    inline int Processor::ld_r_HL()
    {
        constexpr Registers::Names opA = opAFromOpCode<opcode>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);
        m_registers.write8<opA>(val);

        return 2;
    }

    template<uint8_t opcode>
    int Processor::ld_HL_r()
    {
        constexpr Registers::Names opB = opBFromOpCode<opcode>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_registers.read8<opB>();
        m_memory.write8(hl, val);

        return 2;
    }

    template<Registers::Paired NAME>
    int Processor::ld_A_r16()
    {
        uint16_t addr = m_registers.read16<NAME>();
        uint8_t val = m_memory.read8(addr);

        m_registers.write8<Registers::A>(val);

        return 2;
    }

    template<Registers::Paired NAME>
    int Processor::ld_r16_A()
    {
        uint16_t addr = m_registers.read16<NAME>();
        uint8_t val = m_registers.read8<Registers::A> ();
        m_memory.write8(addr, val);

        return 2;
    }

    template<Registers::Paired NAME>
    int Processor::ld_rr_nn()
    {
        uint16_t val = getImmediate16();

        m_registers.write16<NAME>(val);

        return 3;
    }

    template<Registers::Paired NAME>
    int Processor::push_rr()
    {
        uint16_t val = m_registers.read16<NAME>();
        push(val);

        return 4;
    }

    template<Registers::Paired NAME>
    int Processor::pop_rr()
    {
        uint16_t val = pop();
        m_registers.write16<NAME>(val);
        
        return 3;                                                                                                                                                                                            
    }

    template<Registers::Names NAME>
    int Processor::add_r()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();
        int b = (int8_t) m_registers.read8<NAME>();
        add(a, b);

        return 1;
    }

    template<Registers::Names NAME>
    int Processor::adc_r()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();
        int b = (int8_t) m_registers.read8<NAME>();
        adc(a, b);

        return 1;
    }
    
    template<Registers::Names NAME>
    int Processor::sub_r()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();
        int b = (int8_t)m_registers.read8<NAME>();
        sub(a, b);

        return 1;
    }

    template<Registers::Names NAME>
    int Processor::sbc_r()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();
        int b = (int8_t)m_registers.read8<NAME>();
        sbc(a, b);

        return 1;
    }

    template<Registers::Names NAME>
    int Processor::cp_r()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();
        int b = (int8_t)m_registers.read8<NAME>();
        cp(a, b);

        return 1;
    }

    template<Registers::Names NAME>
    int Processor::inc_r()
    {
        int b = (int8_t)m_registers.read8<NAME>();
        int8_t r = b + 1;
        m_registers.write8<NAME>((uint8_t)r);

        int carryBits = b ^ 1 ^ r;
        updateFlagsWithCarry8bit(carryBits, r, false, false);

        return 1;
    }

    template<Registers::Names NAME>
    int Processor::dec_r()
    {
        int b = (int8_t)m_registers.read8<NAME>();
        int8_t r = b - 1;
        m_registers.write8<NAME>((uint8_t)r);

        int carryBits = b ^ 1 ^ r;
        updateFlagsWithCarry8bit(carryBits, r, false, true);

        return 1;
    }

    template<Registers::Names NAME>
    int Processor::and_r()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        uint8_t b = m_registers.read8<NAME>();
        uint8_t r = a & b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, true, false);

        return 1;
    }

    template<Registers::Names NAME>
    int Processor::or_r()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        uint8_t b = m_registers.read8<NAME>();
        uint8_t r = a | b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, false, false);

        return 1;
    }

    template<Registers::Names NAME>
    int Processor::xor_r()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        uint8_t b = m_registers.read8<NAME>();
        uint8_t r = a ^ b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, false, false);

        return 1;
    }

    template<Registers::Paired NAME>
    int Processor::add_HL_rr()
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
    int Processor::inc_rr()
    {
        uint16_t rr = m_registers.read16<NAME>();
        rr++;
        m_registers.write16<NAME>(rr);

        return 2;
    }

    template<Registers::Paired NAME>
    int Processor::dec_rr()
    {
        uint16_t rr = m_registers.read16<NAME>();
        rr--;
        m_registers.write16<NAME>(rr);

        return 2;
    }

    template<Registers::Flag f>
    int Processor::jp_cc_nn()
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
    int Processor::jp_ncc_nn()
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
    int Processor::jr_cc_n()
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
    int Processor::jr_ncc_n()
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
    int Processor::call_cc_nn()
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
    int Processor::call_ncc_nn()
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

    template<Registers::Flag f>
    int Processor::ret_cc()
    {
        if (m_registers.isSetFlag(f))
        {
            uint16_t addr = pop();
            m_registers.setPC(addr);

            return 5;
        }
        else
        {
            return 2;
        }
    }

    template<Registers::Flag f>
    int Processor::ret_ncc()
    {
        if (!m_registers.isSetFlag(f))
        {
            uint16_t addr = pop();
            m_registers.setPC(addr);

            return 5;
        }
        else
        {
            return 2;
        }
    }

    template<uint8_t n>
    int Processor::rst()
    {
        uint16_t pc = m_registers.getPC();
        push(pc);

        uint16_t addr = utils::to16(0x00, n);
        m_registers.setPC(addr);
        return 4;
    }

    template<Registers::Names NAME>
    int Processor::rlc_r()
    {
        uint8_t r = m_registers.read8<NAME>();
        bool c = (r & 0x80) == 0x80;

        r <<= 1;
        if (c)
        {
            r |= 1;
        }

        updateFlags(r, false, false, c);

        m_registers.write8<NAME>(r);

        return 2;
    }

    template<Registers::Names NAME>
    int Processor::rrc_r()
    {
        uint8_t r = m_registers.read8<NAME>();
        bool c = (r & 0x01) == 0x01;

        r >>= 1;
        if (c)
        {
            r |= 0x80;
        }

        updateFlags(r, false, false, c);

        m_registers.write8<NAME>(r);

        return 2;
    }

    template<Registers::Names NAME>
    int Processor::rl_r()
    {
        uint8_t r = m_registers.read8<NAME>();
        bool c = (r & 0x80) == 0x80;
        uint8_t oldCarry = m_registers.isSetFlag(Registers::Flag::C) ? 1 : 0;
        r <<= 1;
        r |= oldCarry;

        updateFlags(r, false, false, c);

        m_registers.write8<NAME>(r);

        return 2;
    }

    template<Registers::Names NAME>
    int Processor::rr_r()
    {
        uint8_t r = m_registers.read8<NAME>();
        bool c = (r & 1) == 1;
        uint8_t oldCarry = m_registers.isSetFlag(Registers::Flag::C) ? 0X80 : 0;
        r >>= 1;
        r |= oldCarry;

        updateFlags(r, false, false, c);

        m_registers.write8<NAME>(r);

        return 2;
    }

    template<Registers::Names NAME>
    int Processor::sla_r()
    {
        uint8_t r = m_registers.read8<NAME>();
        bool c = (r & 0x80) == 0x80;
        r <<= 1;

        updateFlags(r, false, false, c);
        m_registers.write8<NAME>(r);

        return 2;
    }

    template<Registers::Names NAME>
    int Processor::sra_r()
    {
        uint8_t r = m_registers.read8<NAME>();
        bool c = (r & 1) == 1;
        if ((r & 0x80) != 0)
        {
            r >>= 1;
            r |= 0x80;
        }
        else
        {
            r >>= 1;
        }
        
        updateFlags(r, false, false, c);
        m_registers.write8<NAME>(r);

        return 2;
    }

    template<Registers::Names NAME>
    int Processor::srl_r()
    {
        uint8_t r = m_registers.read8<NAME>();
        bool c = (r & 1) == 1;
        r >>= 1;

        updateFlags(r, false, false, c);
        m_registers.write8<NAME>(r);

        return 2;
    }

    template<Registers::Names NAME>
    int cpu::Processor::swap_r()
    {
        uint8_t r = m_registers.read8<NAME>();
        r = (r << 4) | (r >> 4);

        updateFlags(r, false, false, false);
        m_registers.write8<NAME>(r);

        return 2;
    }

    template<uint8_t n, Registers::Names NAME>
    int Processor::bit_n_r()
    {
        static_assert(n < 8);

        uint8_t r = m_registers.read8<NAME>();
        bit_n(n ,r);

        return 2;
    }

    template<uint8_t n>
    int Processor::bit_n_HL()
    {
        static_assert(n < 8);
        
        uint16_t hl = m_registers.read16<Registers::HL>();
        bit_n(n, hl);

        return 3;
    }

    template<uint8_t n, Registers::Names NAME>
    int Processor::res_n_r()
    {
        static_assert(n < 8);

        uint8_t r = m_registers.read8<NAME>();
        r = r & ~(1 << n);

        m_registers.write8<NAME>(r);

        return 2;
    }

    template<uint8_t n>
    int Processor::res_n_HL()
    {
        static_assert(n < 8);

        uint16_t hl = m_registers.read16<Registers::HL>();
        hl = hl & ~(1 << n);

        m_registers.write16<Registers::HL>(hl);

        return 4;
    }

    template<uint8_t n, Registers::Names NAME>
    int Processor::set_n_r()
    {
        static_assert(n < 8);
        
        uint8_t r = m_registers.read8<NAME>();
        r = r | (1 << n);
        
        m_registers.write8<NAME>(r);

        return 2;
    }

    template<uint8_t n>
    int Processor::set_n_HL()
    {
        static_assert(n < 8);

        uint16_t hl = m_registers.read16<Registers::HL>();
        hl = hl | (1 << n);

        m_registers.write16<Registers::HL>(hl);

        return 4;
    }
}
