#pragma once

#include <cstdint>

namespace cpu
{
class Registers;
}
class Memory;
namespace video
{
class Screen;
}

class MMIO
{
public:
	using mapped_io = void (MMIO::*)(uint16_t, uint8_t);
	
	MMIO(cpu::Registers& registers, Memory& memory, video::Screen& screen);

	uint8_t read(uint16_t addr) const;
	void write(uint16_t addr, uint8_t val);

	void empty(uint16_t, uint8_t);

	void resetDIV(uint16_t addr, uint8_t val);
	void lcdControl(uint16_t addr, uint8_t val);
	void scy(uint16_t addr, uint8_t val);
	void scx(uint16_t addr, uint8_t val);
	void wy(uint16_t addr, uint8_t val);
	void wx(uint16_t addr, uint8_t val);
	void incrementTIMA(uint16_t addr, uint8_t val);
	void disableBootROM(uint16_t addr, uint8_t val);

	void writeValue(uint16_t addr, uint8_t value);

private:
	Memory& m_memory;
	video::Screen& m_screen;

	mapped_io m_mappedIOs[128];

	cpu::Registers& m_registers;
};