#include "instruction.h"

#include "registery.h"
#include "memory.h"

namespace cpu
{
    Instructions::Instructions(Registers& regist, Memory& mem): m_registers(regist), m_memory(mem)
    {
        fillInstructionSet();
        fillCbInstructionSet();

        fillInstructionSetDisassembly();
        fillCbInstructionSetDisassembly();
    }
    
    void Instructions::fillInstructionSet()
    {
        std::fill_n(m_instructionSet, 255, &Instructions::unhandled);
        m_instructionSet[0x03] = &Instructions::ld_BC_A;
    }

    void Instructions::fillCbInstructionSet()
    {
        std::fill_n(m_cbInstructionSet, 255, &Instructions::unhandled);
    }

    void Instructions::fillInstructionSetDisassembly()
    {
        std::fill_n(m_instructionSetDisassembly, 255, &Instructions::unhandledDisassembly);
        m_instructionSetDisassembly[0x03] = &Instructions::ld_BC_A_dis;
    }

    void Instructions::fillCbInstructionSetDisassembly()
    {
        std::fill_n(m_cbInstructionSetDisassembly, 255, &Instructions::unhandledDisassembly);
    }
    
    template<bool disassemble>
    void Instructions::execute(uint8_t opCode)
    {
        m_instructionSet[opCode]();
        if constexpr (disassemble)
        {
            std::cout << m_instructionSetDisassembly[opCode]();
        }
    }

    int Instructions::unhandled(uint16_t, uint16_t) 
    {
        return 1;
    }

    std::string Instructions::unhandledDisassembly(uint8_t opCode, uint16_t, uint16_t)
    {
        return std::to_string(opCode) +  " : Not handled yet;\n";
    }

    int Instructions::ld_BC_A(uint16_t, uint16_t)
    {
        m_registers++;

        uint8_t A = m_registers.read8<Registers::A>();
        m_registers.write8<Registers::BC>(A);

        return 8;
    }

    std::string Instructions::ld_BC_A_dis(uint8_t opCode, uint16_t, uint16_t)
    {
        return std::to_string(opCode) +  " : LD (BC), A;\n";
    }
}