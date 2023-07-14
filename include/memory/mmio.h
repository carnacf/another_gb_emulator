#pragma once

#include <cstdint>

namespace cpu
{
class Registers;
}
class Memory;

class MMIO
{
public:
	using mapped_io = void (MMIO::*)(uint16_t, uint8_t);
	
	MMIO(cpu::Registers& registers, Memory& memory);

	uint8_t read(uint16_t addr) const;
	void write(uint16_t addr, uint8_t val);

	void empty(uint16_t, uint8_t);

	void disableBootROM(uint16_t addr, uint8_t val);
	void writeValue(uint16_t addr, uint8_t value);

private:
	Memory& m_memory;
	mapped_io m_mappedIOs[128];
	uint8_t m_adressSpace[128];

	cpu::Registers& m_registers;
};