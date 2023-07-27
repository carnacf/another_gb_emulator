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
	using mapped_ioW = void (MMIO::*)(uint16_t, uint8_t);
	using mapped_ioR = uint8_t (MMIO::*)(uint16_t) const;
	
	MMIO(cpu::Registers& registers, Memory& memory, video::Screen& screen);

	uint8_t read(uint16_t addr) const;
	void write(uint16_t addr, uint8_t val);

	void incrementTIMA(uint16_t addr, uint8_t val);
	void disableBootROM(uint16_t addr, uint8_t val);

private:
	uint8_t readAddress(uint16_t addr) const;

	// Read-only address
	void empty(uint16_t, uint8_t);

	void resetDIV(uint16_t addr, uint8_t val);
	void lcdControl(uint16_t addr, uint8_t val);
	void scy(uint16_t addr, uint8_t val);
	void scx(uint16_t addr, uint8_t val);
	void lyc(uint16_t addr, uint8_t val);
	void wy(uint16_t addr, uint8_t val);
	void wx(uint16_t addr, uint8_t val);
	void dma(uint16_t addr, uint8_t val);

	void updateBGPalette(uint16_t addr, uint8_t val);
	uint8_t readBGPalette(uint16_t addr) const;
	uint8_t ly(uint16_t addr) const;

	void writeValue(uint16_t addr, uint8_t value);

private:
	Memory& m_memory;
	video::Screen& m_screen;

	mapped_ioW m_mappedIOsW[128];
	mapped_ioR m_mappedIOsR[128];

	cpu::Registers& m_registers;
};