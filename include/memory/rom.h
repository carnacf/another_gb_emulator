#pragma once 

#include <cstdint>

class Rom
{
public:
	virtual ~Rom() = 0;
	virtual uint8_t read(uint16_t addr) = 0;
	virtual void write(uint16_t addr, uint8_t val) = 0;
};