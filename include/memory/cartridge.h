#pragma once

#include "rom.h"

#include <string>
#include <memory>

class Cartridge
{
public:
	enum class Type
	{
		ROM_ONLY,
		MBC1,
		MBC2,
		MBC3,
		MBC5
	};

	Cartridge(const char* filePath);
	uint8_t nbRomBank() const
	{
		return m_nbRomBank;
	}

	uint8_t nbRamBank()
	{
		return m_nbRamBank;
	}

	static std::unique_ptr<Rom> buildRomFromCartridge(const Cartridge& cartridge);
private:

	void loadCartidge(const char* filePath);

	std::string m_title;
	Type m_cartridgeType = Type::ROM_ONLY;

	uint8_t m_nbRomBank;
	uint8_t m_nbRamBank;
};