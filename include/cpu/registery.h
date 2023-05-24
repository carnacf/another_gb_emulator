#pragma once

#include "..\utils\global.h"

namespace cpu
{
class Registers {
public:
  enum Names {
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

  Registers operator++(int)
  {
    Registers old = *this;
    incrementPC();
    return old;
  }

  Registers& operator++()
  {
    incrementPC();
    return *this;
  }

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

  void incrementPC()
  {
    pc++;
  }

private:
  uint8_t registers[8];
  uint16_t sp;
  uint16_t pc;
};
}
