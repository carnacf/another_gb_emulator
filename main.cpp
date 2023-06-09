#include <iostream>
#include <fstream>

#include "utils/utils.h"

#include "cpu/instruction.h"
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
    cpu::Instructions instructions(registery, memory);
    

    return 1;
}