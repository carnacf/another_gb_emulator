#include "memory.h"

#include "cartridge.h"
#include "registery.h"
#include "memory.h"

Memory::Memory(const Cartridge& cartridge, cpu::Registers& registers, const char* bootROMPath):
    m_mmio(registers, *this), 
    m_romBank(Cartridge::buildRomFromCartridge(cartridge))
{
    loadBootROM(bootROMPath);
}

Memory::~Memory()
{
    delete[] m_bootROM;
}

void Memory::write8(uint16_t addr, uint8_t val)
{
    if (addr < 0x8000)
    {
        m_memoryMap[addr] = val;
        //return m_romBank->read(addr);
    }
    else if (addr < 0xA000)
    {
        // Video Ram
        m_memoryMap[addr] = val;
    }
    else if (addr < 0xE000)
    {
        m_memoryMap[addr] = val;
        //return m_romBank->read(addr);
    }
    else if (addr < 0xFEA0)
    {
        // Object attribute memory
        m_memoryMap[addr] = val;
    }
    else if (addr < 0xFF80 && addr > 0xFEFF)
    {
        // MMIO
        m_mmio.write(addr, val);
    }
    else if (addr < 0xFFFF)
    {
        m_memoryMap[addr] = val;
        //return m_romBank->read(addr);
    }

    m_memoryMap[addr] = val;
}

uint8_t Memory::read8(uint16_t addr)
{
    if (addr < 0x8000)
    {
        if (m_bootROMEnabled && addr <= 0xFF)
        {
            return m_bootROM[addr];
        }

        return m_memoryMap[addr];
        //return m_romBank->read(addr);
    }
    else if (addr < 0xA000)
    {
        // Video Ram
        return 0;
    }
    else if (addr < 0xE000)
    {
        return m_memoryMap[addr];
        //return m_romBank->read(addr);
    }
    else if (addr < 0xFEA0)
    {
        // Object attribute memory
        return 0;
    }
    else if (addr < 0xFF80 && addr > 0xFEFF)
    {
        // MMIO
        m_mmio.read(addr);
    }
    else if (addr < 0xFFFF)
    {
        return m_memoryMap[addr];
        //return m_romBank->read(addr);
    }

    return m_memoryMap[addr];
}

void Memory::write16(uint16_t addr, uint16_t val)
{
    write8(addr, utils::low(val));
    write8(addr + 1, utils::high(val));
}

uint16_t Memory::read16(uint16_t addr)
{
    return utils::to16(read8(addr + 1), read8(addr));
}

bool Memory::loadROM(const char* filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        return false;
    }

    size_t size = std::min((size_t)file.tellg(), (size_t)0x7FFF);
    file.seekg(0, std::ios::beg);

    bool readSuccess = file.read(reinterpret_cast<char*>(m_memoryMap), size).good();
    file.close();

    return readSuccess;
}

void Memory::incrementDIV()
{
    m_memoryMap[0xFF04] += 1;
}

bool Memory::loadBootROM(const char* filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        return false;
    }

    size_t size = (size_t) file.tellg();
    if (m_bootROM != nullptr)
    {
        delete[] m_bootROM;
    }
    m_bootROM = new uint8_t[size];

    file.seekg(0, std::ios::beg);

    bool readSuccess = file.read(reinterpret_cast<char*>(m_bootROM), size).good();
    file.close();

    return readSuccess;
}
