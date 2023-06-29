#include <iostream>
#include <fstream>

#include "utils/utils.h"

#include "cpu/executor.h"
#include "cpu/registery.h"
#include "memory/memory.h"

int main(int argc, char* argv[])
{
    auto fileName = argv[0];

    Memory memory;
    if (!memory.loadROM(fileName))
    {
        return 0;
    }

    cpu::Registers registery;
    cpu::Executor instructions(registery, memory);

    while (1)
    {
        uint8_t opcode = memory.read8(registery.getPC());
        instructions.execute<true>(opcode);
    }

    return 1;
}