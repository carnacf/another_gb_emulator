#pragma once

#include "utils/global.h"
#include "utils/utils.h"

#include <cstdint>
#include <cassert>
#include <string>

namespace cpu
{

class Registers 
{
public:
    enum class Names 
    {
        A = 0,
        F = 1,
        B = 2,
        C = 3,
        D = 4,
        E = 5,
        H = 6,
        L = 7
    };
    static constexpr Names A = Names::A;
    static constexpr Names F = Names::F;
    static constexpr Names B = Names::B;
    static constexpr Names C = Names::C;
    static constexpr Names D = Names::D;
    static constexpr Names E = Names::E;
    static constexpr Names H = Names::H;
    static constexpr Names L = Names::L;

    enum class Paired
    {
        AF = 0,
        BC = 2,
        DE = 4,
        HL = 6
    };
    static constexpr Paired AF = Paired::AF;
    static constexpr Paired BC = Paired::BC;
    static constexpr Paired DE = Paired::DE;
    static constexpr Paired HL = Paired::HL;

    enum class Flag : std::uint8_t
    {
        carry = 0b0001'0000,
        half_carry = 0b0010'0000,
        substract = 0b0100'0000,
        zero = 0b1000'0000
    };

    void enableFlag(Flag f, bool enabled)
    {
        if (!enabled)
        {
            write8<F>(read8<F>() & ~(uint8_t)f);
        }
        else
        {
            write8<F>(read8<F>() | (uint8_t)f);
        }
    }

    template<Paired NAME>
    void write16(uint16_t val)
    {
        m_registers[(int)NAME] = utils::high(val);
        m_registers[(int)NAME + 1] = utils::low(val);
    }

    template<Names NAME>
    void write8(uint8_t val1)
    {
        m_registers[(int)NAME] = val1;
    }
  
    template<Paired NAME>
    uint16_t read16()
    {
        return utils::to16(m_registers[(int)NAME], m_registers[(int)NAME + 1]);
    }

    template<Names NAME>
    uint8_t read8()
    {
        return m_registers[(int)NAME];
    }

    uint16_t getPC() const
    {
        return m_pc;
    }
    void incrementPC()
    {
        m_pc++;
    }
    
    uint16_t getSP() const
    {
        return m_sp;
    }
    void setSP(uint16_t sp)
    {
        m_sp = sp;
    }

    template<uint8_t v>
    static constexpr Names opFieldToName()
    {
        if constexpr (v == 0b111)
        {
            return Names::A;
        }
        else if constexpr (v == 0b000)
        {
            return Names::B;
        }
        else if constexpr (v == 0b001)
        {
            return Names::C;
        }
        else if constexpr (v == 0b010)
        {
            return Names::D;
        }
        else if constexpr (v == 0b011)
        {
            return Names::E;
        }
        else if constexpr (v == 0b100)
        {
            return Names::H;
        }
        else if constexpr (v == 0b101)
        {
            return Names::L;
        }

        return Names::A;
    }

    static std::string register8ToStr(Names reg)
    {
        switch (reg)
        {
        case Names::F:
            return "F";
        case Names::A:
            return "A";
        case Names::C:
            return "C";
        case Names::B:
            return "B";
        case Names::E:
            return "E";
        case Names::D:
            return "D";
        case Names::L:
            return "L";
        case Names::H:
            return "H";
        default:
            break;
        }

        return "";
    }

    static std::string register16ToStr(Paired reg)
    {
        switch (reg)
        {
        case Paired::AF:
            return "AF";
        case Paired::BC:
            return "BC";
        case Paired::DE:
            return "DE";
        case Paired::HL:
            return "HL";
        default:
            break;
        }
        return "";
    }
private:
    uint8_t m_registers[8] = {};
    uint16_t m_sp = 0;
    uint16_t m_pc = 0; // TODO: add reset method
};
}
