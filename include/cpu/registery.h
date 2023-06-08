#pragma once

#include "utils\global.h"

#include <cstdint>
#include <cassert>
#include <string>

namespace cpu
{
class Registers 
{
public:
    enum Names 
    {
        AF = 0,
        F = 0,
        A = 1,
        BC = 2,
        C = 2,
        B = 3,
        DE = 4,
        E = 4,
        D = 5,
        HL = 6,
        L = 6,
        H = 7
    };

    template<Names NAME>
    void write16(uint16_t val)
    {
        write16<NAME>(low(val), high(val));
    }

    template<Names NAME>
    void write16(uint8_t val1, uint8_t val2)
    {
        registers[NAME] = val1;
        registers[NAME + 1] = val2;
    }

    template<Names NAME>
    void write8(uint8_t val1)
    {
        registers[NAME] = val1;
    }
  
    template<Names NAME>
    uint16_t read16()
    {
        static_assert(NAME == AF || NAME == BC || NAME == DE || NAME == HL);
        return (uint16_t) ((((uint16_t) registers[NAME]) << 8) & registers[NAME + 1]);
    }

    template<Names NAME>
    uint8_t read8()
    {
        static_assert(NAME >= F && NAME <= H);
        return registers[NAME];
    }

    uint16_t getPC() const
    {
        return m_pc;
    }

    void incrementPC()
    {
        m_pc++;
    }

    template<uint8_t v>
    static constexpr Names valueToName()
    {
        if constexpr (v == 0b111)
        {
            return A;
        }
        else if constexpr (v == 0b000)
        {
            return B;
        }
        else if constexpr (v == 0b001)
        {
            return C;
        }
        else if constexpr (v == 0b010)
        {
            return D;
        }
        else if constexpr (v == 0b011)
        {
            return E;
        }
        else if constexpr (v == 0b100)
        {
            return H;
        }
        else if constexpr (v == 0b101)
        {
            return L;
        }

        assert(false);
        return A;
        
    }

    static std::string register8ToStr(Names reg)
    {
        switch (reg)
        {
        case F:
            return "F";
        case A:
            return "A";
        case C:
            return "C";
        case B:
            return "B";
        case E:
            return "E";
        case D:
            return "D";
        case L:
            return "L";
        case H:
            return "H";
        default:
            break;
        }

        return "";
    }

    static std::string register16ToStr(Names reg)
    {
        switch (reg)
        {
        case AF:
            return "AF";
        case BC:
            return "BC";
        case DE:
            return "DE";
        case HL:
            return "HL";
        default:
            break;
        }
        return "";
    }
private:
  uint8_t m_registers[8];
  uint16_t m_sp;
  uint16_t m_pc = 0;
};
}
