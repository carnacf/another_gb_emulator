#include "mmio.h"

#include "memory.h"
#include "video/screen.h"

#include <algorithm>

MMIO::MMIO(cpu::Registers& registers, Memory& memory, video::Screen& screen) :
	m_memory(memory),
	m_screen(screen),
	m_registers(registers)
{
	std::fill_n(std::begin(m_mappedIOs), 128, &MMIO::writeValue);
	// Joypad Input
	m_mappedIOs[0] = &MMIO::writeValue;
	m_mappedIOs[0x04] = &MMIO::resetDIV;
	m_mappedIOs[0x05] = &MMIO::incrementTIMA;
	
	m_mappedIOs[0x40] = &MMIO::lcdControl;
	m_mappedIOs[0x42] = &MMIO::scy;
	m_mappedIOs[0x43] = &MMIO::scx;
	m_mappedIOs[0x4A] = &MMIO::wy;
	m_mappedIOs[0x4B] = &MMIO::wx;
	m_mappedIOs[0x50] = &MMIO::disableBootROM;
}

uint8_t MMIO::read(uint16_t addr) const
{
	return m_memory.m_memoryMap[addr];
}

void MMIO::write(uint16_t addr, uint8_t val)
{
	auto actualAddr = addr & 0x00FF;
	(this->*m_mappedIOs[actualAddr])(addr, val);
}

void MMIO::empty(uint16_t, uint8_t)
{
}

void MMIO::writeValue(uint16_t addr, uint8_t value)
{
	m_memory.m_memoryMap[addr] = value;
}

void MMIO::resetDIV(uint16_t addr, uint8_t val)
{
	m_memory.m_memoryMap[addr] = 0;
}

void MMIO::incrementTIMA(uint16_t addr, uint8_t val)
{
	if (m_memory.m_memoryMap[addr] == 0xFF)
	{
		m_memory.m_memoryMap[addr] = m_memory.m_memoryMap[0xFF06];
		// Timer interrupt
		m_memory.m_memoryMap[0xFF0F] = m_memory.m_memoryMap[0xFF0F] | 0x04;
	}
	else
	{
		m_memory.m_memoryMap[addr] = m_memory.m_memoryMap[addr] + 1;
	}
}

void MMIO::lcdControl(uint16_t addr, uint8_t val)
{
	m_memory.m_memoryMap[addr] = val;

	m_screen.enableLCD((val & 0x80) == 0x80);
	
	uint16_t tileMapAddr = ((val & 0x40) == 0x40) ? 0x9C00 : 0x9800;
	m_screen.setWindowTileMapAddr(tileMapAddr);

	m_screen.enableWindow((val & 0x20) == 0x20);

	uint16_t tileDataArea = ((val & 0x10) == 0x10) ? 0x8800 : 0x8800;
	m_screen.setTileDataArea(tileMapAddr);

	uint16_t bgTileMapArea = ((val & 0x08) == 0x08) ? 0x9C00 : 0x9800;
	m_screen.setBgTileMapAddr(tileMapAddr);

	uint8_t objSize = ((val & 0x04) == 0x04) ? 16 : 8;
	m_screen.setObjSize(objSize);

	m_screen.enableObj((val & 0x02) == 0x02);

	m_screen.enablePriority((val & 0x01) == 0x01);
}

void MMIO::scy(uint16_t addr, uint8_t val)
{
	m_memory.m_memoryMap[addr] = val > 255 ? val % 255 : val;
	m_screen.setSCY(m_memory.m_memoryMap[addr]);
}

void MMIO::scx(uint16_t addr, uint8_t val)
{
	m_memory.m_memoryMap[addr] = val > 255 ? val % 255 : val;
	m_screen.setSCX(m_memory.m_memoryMap[addr]);
}

void MMIO::wy(uint16_t addr, uint8_t val)
{
	m_memory.m_memoryMap[addr] = val > 143 ? val % 143 : val;
	m_screen.setWY(m_memory.m_memoryMap[addr]);
}

void MMIO::wx(uint16_t addr, uint8_t val)
{
	m_memory.m_memoryMap[addr] = val > 166 ? val % 166 : val;
	m_screen.setWX(m_memory.m_memoryMap[addr]);
}

void MMIO::disableBootROM(uint16_t addr, uint8_t val)
{
	if (val != 0)
	{
		m_memory.disableBootRom();
	}
	m_memory.m_memoryMap[addr] = val;
}
