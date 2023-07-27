#include "cartridge.h"

#include <fstream>
#include <sstream>

Cartridge::Cartridge(const char* filePath)
{
	loadCartidge(filePath);
}

std::unique_ptr<Rom> Cartridge::buildRomFromCartridge(const Cartridge& cartridge)
{
	// TODO
	return std::unique_ptr<Rom>();
}

void Cartridge::loadCartidge(const char* filePath)
{
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		return;
	}

	file.seekg(0x134, std::ios::beg);
	char buffer[15];
	bool success = file.read(&buffer[0], 15).good();
	if (!success)
	{
		return;
	}
	m_title = buffer;

	uint8_t type = 0;
	file.seekg(0x147, std::ios::beg);
	success = file.read(reinterpret_cast<char*>(&type), 1).good();
	if (!success)
	{
		return;
	}
	switch (type)
	{
	case 0x00:
		m_cartridgeType = Type::ROM_ONLY;
		break;
	case 0x01:
	case 0x02:
	case 0x03:
		m_cartridgeType = Type::MBC1;
		break;
	case 0x05:
	case 0x06:
		m_cartridgeType = Type::MBC2;
		break;
	case 0x0F:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
		m_cartridgeType = Type::MBC3;
		break;
	case 0x19:
	case 0x1A:
	case 0x1B:
	case 0x1C:
	case 0x1D:
	case 0x1E:
		m_cartridgeType = Type::MBC5;
		break;
	default:
		m_cartridgeType = Type::ROM_ONLY;
		break;
	}

	uint8_t romBank = 0;
	file.seekg(0x148, std::ios::beg);
	success = file.read(reinterpret_cast<char*>(&romBank), 1).good();
	if (!success)
	{
		return;
	}
	
	m_nbRomBank = (1 << romBank) / 16;

	uint8_t ramBank = 0;
	file.seekg(0x149, std::ios::beg);
	success = file.read(reinterpret_cast<char*>(&ramBank), 1).good();
	if (!success)
	{
		return;
	}

	switch (ramBank)
	{
	case 0:
	case 1:
		m_nbRamBank = 0;
		break;
	case 2:
		m_nbRamBank = 1;
		break;
	case 3:
		m_nbRamBank = 4;
		break;
	case 4:
		m_nbRamBank = 16;
		break;
	case 5:
		m_nbRamBank = 8;
		break;
	default:
		m_nbRamBank = 0;
		break;
	}
}
