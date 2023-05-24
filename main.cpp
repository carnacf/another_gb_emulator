#include <iostream>
#include <fstream>

#include "utils/utils.h"

#include "cpu/instruction.h"
#include "cpu/registery.h"
#include "memory/memory.h"

int main(int argc, char* argv[])
{
    auto fileName = argv[0];

    cpu::Registers registery;
    Memory memory;
    cpu::Instructions instructions(registery, memory);

    std::ifstream file(fileName, std::ios::binary);
    if(!file.is_open())
    {
        return 0;
    }
    
    file.close();
    return 1;
}