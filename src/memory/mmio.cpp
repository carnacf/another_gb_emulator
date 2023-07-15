#include "mmio.h"

#include "memory.h"

#include <algorithm>

MMIO::MMIO(cpu::Registers& registers, Memory& memory) :
	m_memory(memory),
	m_registers(registers)
{
	std::fill_n(std::begin(m_mappedIOs), 71, &MMIO::empty);
	// Joypad Input
	m_mappedIOs[0] = &MMIO::writeValue;
	m_mappedIOs[0x04] = &MMIO::resetDIV;
	m_mappedIOs[0x05] = &MMIO::incrementTIMA;
	
	m_mappedIOs[0x50] = &MMIO::disableBootROM;
}

uint8_t MMIO::read(uint16_t addr) const
{
	return m_memory.m_memoryMap[addr];
}

void MMIO::write(uint16_t addr, uint8_t val)
{
	(this->*m_mappedIOs[addr |= 0x0000])(addr, val);
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

void MMIO::disableBootROM(uint16_t addr, uint8_t val)
{
	if (val != 0)
	{
		m_memory.disableBootRom();
	}
	m_memory.m_memoryMap[addr] = val;
}
