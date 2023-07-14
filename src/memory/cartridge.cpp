#include "cartridge.h"

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
	// TODO
}
