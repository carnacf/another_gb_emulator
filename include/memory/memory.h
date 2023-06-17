#pragma once 

#include "utils/global.h"
#include "utils/utils.h"

#include <fstream>
#include <algorithm>

class Memory
{
public:
    FORCEINLINE void write8(uint16_t index, uint8_t val)
    {
        m_memoryMap[index] = val;
    };

    FORCEINLINE uint8_t read8(uint16_t index)
    {
        return m_memoryMap[index];
    }

    FORCEINLINE void write16(uint16_t index, uint16_t val)
    {
        m_memoryMap[index] = utils::low(val);
        m_memoryMap[index + 1] = utils::high(val); 
    }

    FORCEINLINE uint16_t read16(uint16_t index)
    {
        return utils::to16(m_memoryMap[index + 1], m_memoryMap[index]);
    }

    bool loadROM(const char* filename)
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            return false;
        }

        size_t size = std::min((size_t) file.tellg(), (size_t) 0x7FFF);
        file.seekg(0, std::ios::beg);

        bool readSuccess = file.read(reinterpret_cast<char*>(m_memoryMap), size).good();
        file.close();

        return readSuccess;
    }

private:

    uint8_t m_memoryMap[0x10000];
};