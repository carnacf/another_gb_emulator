#pragma once 

#include "utils/global.h"
#include "utils/utils.h"

#include "mmio.h"

#include <fstream>
#include <algorithm>

class Cartridge;
namespace cpu
{
class Registers;
}
class Rom;

class Memory
{
public:
    Memory(const Cartridge& cartridge, cpu::Registers& registers, 
        const char* bootROMPath);
    ~Memory();

    FORCEINLINE void write8(uint16_t index, uint8_t val);
    FORCEINLINE uint8_t read8(uint16_t index);

    FORCEINLINE void write16(uint16_t index, uint16_t val);
    FORCEINLINE uint16_t read16(uint16_t index);

    bool loadROM(const char* filename);

    void disableBootRom()
    {
        m_bootROMEnabled = false;
    }


    void incrementDIV();

private:
    friend MMIO;

    bool loadBootROM(const char* filename);

    MMIO m_mmio;
    std::unique_ptr<Rom> m_romBank;
    uint8_t m_memoryMap[0x10000];
    uint8_t* m_bootROM = nullptr;
    bool m_bootROMEnabled = true;
};