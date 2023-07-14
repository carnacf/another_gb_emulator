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
	
	m_mappedIOs[0x50] = &MMIO::disableBootROM;
}

uint8_t MMIO::read(uint16_t addr) const
{
	return m_adressSpace[addr | 0x0000];
}

void MMIO::write(uint16_t addr, uint8_t val)
{
	addr |= 0x0000;
	(this->*m_mappedIOs[addr])(addr, val);
}

void MMIO::empty(uint16_t, uint8_t)
{
}

void MMIO::writeValue(uint16_t addr, uint8_t value)
{
	m_adressSpace[addr] = value;
}

void MMIO::disableBootROM(uint16_t addr, uint8_t val)
{
	if (val != 0)
	{
		m_memory.disableBootRom();
	}
	m_adressSpace[addr] = val;
}
