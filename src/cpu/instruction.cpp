#include "instruction.h"

#include "registery.h"
#include "memory.h"

#include <utility>

namespace cpu
{
    namespace
    {
        template<uint8_t opcode>
        constexpr std::pair<Registers::Names, Registers::Names> extractLDRROperands()
        {
            constexpr uint8_t opAMask = 0b0011'1000;
            constexpr uint8_t opA = (opAMask & opcode) >> 3;
            constexpr Registers::Names rA = Registers::valueToName<opA>();

            constexpr uint8_t opBMask = 0b0000'0111;
            constexpr uint8_t opB = opBMask & opcode;
            constexpr Registers::Names rB = Registers::valueToName<opB>();

            return {rA, rB};
        }
    }

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

        m_instructionSet[0x40] = &Instructions::ld_r_r_8<0x40>;
        m_instructionSet[0x41] = &Instructions::ld_r_r_8<0x41>;
        m_instructionSet[0x42] = &Instructions::ld_r_r_8<0x42>;
        m_instructionSet[0x43] = &Instructions::ld_r_r_8<0x43>;
        m_instructionSet[0x44] = &Instructions::ld_r_r_8<0x44>;
        m_instructionSet[0x45] = &Instructions::ld_r_r_8<0x45>;

        m_instructionSet[0x47] = &Instructions::ld_r_r_8<0x47>;
        m_instructionSet[0x48] = &Instructions::ld_r_r_8<0x48>;
        m_instructionSet[0x49] = &Instructions::ld_r_r_8<0x49>;
        m_instructionSet[0x4A] = &Instructions::ld_r_r_8<0x4A>;
        m_instructionSet[0x4B] = &Instructions::ld_r_r_8<0x4B>;
        m_instructionSet[0x4C] = &Instructions::ld_r_r_8<0x4C>;
        m_instructionSet[0x4D] = &Instructions::ld_r_r_8<0x4D>;

        m_instructionSet[0x4F] = &Instructions::ld_r_r_8<0x4F>;
        m_instructionSet[0x50] = &Instructions::ld_r_r_8<0x50>;
        m_instructionSet[0x51] = &Instructions::ld_r_r_8<0x51>;
        m_instructionSet[0x52] = &Instructions::ld_r_r_8<0x52>;
        m_instructionSet[0x53] = &Instructions::ld_r_r_8<0x53>;
        m_instructionSet[0x54] = &Instructions::ld_r_r_8<0x54>;
        m_instructionSet[0x55] = &Instructions::ld_r_r_8<0x55>;

        m_instructionSet[0x57] = &Instructions::ld_r_r_8<0x57>;
        m_instructionSet[0x58] = &Instructions::ld_r_r_8<0x58>;
        m_instructionSet[0x59] = &Instructions::ld_r_r_8<0x59>;
        m_instructionSet[0x5A] = &Instructions::ld_r_r_8<0x5A>;
        m_instructionSet[0x5B] = &Instructions::ld_r_r_8<0x5B>;
        m_instructionSet[0x5C] = &Instructions::ld_r_r_8<0x5C>;
        m_instructionSet[0x5D] = &Instructions::ld_r_r_8<0x5D>;

        m_instructionSet[0x5F] = &Instructions::ld_r_r_8<0x5F>;
        m_instructionSet[0x60] = &Instructions::ld_r_r_8<0x60>;
        m_instructionSet[0x61] = &Instructions::ld_r_r_8<0x61>;
        m_instructionSet[0x62] = &Instructions::ld_r_r_8<0x62>;
        m_instructionSet[0x63] = &Instructions::ld_r_r_8<0x63>;
        m_instructionSet[0x64] = &Instructions::ld_r_r_8<0x64>;
        m_instructionSet[0x65] = &Instructions::ld_r_r_8<0x65>;

        m_instructionSet[0x67] = &Instructions::ld_r_r_8<0x67>;
        m_instructionSet[0x68] = &Instructions::ld_r_r_8<0x68>;
        m_instructionSet[0x69] = &Instructions::ld_r_r_8<0x69>;
        m_instructionSet[0x6A] = &Instructions::ld_r_r_8<0x6A>;
        m_instructionSet[0x6B] = &Instructions::ld_r_r_8<0x6B>;
        m_instructionSet[0x6C] = &Instructions::ld_r_r_8<0x6C>;
        m_instructionSet[0x6D] = &Instructions::ld_r_r_8<0x6D>;

        m_instructionSet[0x6F] = &Instructions::ld_r_r_8<0x6F>;
        m_instructionSet[0x78] = &Instructions::ld_r_r_8<0x78>;
        m_instructionSet[0x79] = &Instructions::ld_r_r_8<0x79>;
        m_instructionSet[0x7A] = &Instructions::ld_r_r_8<0x7A>;
        m_instructionSet[0x7B] = &Instructions::ld_r_r_8<0x7B>;
        m_instructionSet[0x7C] = &Instructions::ld_r_r_8<0x7C>;
        m_instructionSet[0x7D] = &Instructions::ld_r_r_8<0x7D>;
        m_instructionSet[0x7F] = &Instructions::ld_r_r_8<0x7F>;
    }

    void Instructions::fillCbInstructionSet()
    {
        std::fill_n(m_cbInstructionSet, 255, &Instructions::unhandled);
    }

    void Instructions::fillInstructionSetDisassembly()
    {
        std::fill_n(m_instructionSetDisassembly, 255, &Instructions::unhandledDisassembly);
        m_instructionSetDisassembly[0x03] = &Instructions::ld_BC_A_dis;

        m_instructionSetDisassembly[0x40] = &Instructions::ld_r_r_8_dis<0x40>;
        m_instructionSetDisassembly[0x41] = &Instructions::ld_r_r_8_dis<0x41>;
        m_instructionSetDisassembly[0x42] = &Instructions::ld_r_r_8_dis<0x42>;
        m_instructionSetDisassembly[0x43] = &Instructions::ld_r_r_8_dis<0x43>;
        m_instructionSetDisassembly[0x44] = &Instructions::ld_r_r_8_dis<0x44>;
        m_instructionSetDisassembly[0x45] = &Instructions::ld_r_r_8_dis<0x45>;

        m_instructionSetDisassembly[0x47] = &Instructions::ld_r_r_8_dis<0x47>;
        m_instructionSetDisassembly[0x48] = &Instructions::ld_r_r_8_dis<0x48>;
        m_instructionSetDisassembly[0x49] = &Instructions::ld_r_r_8_dis<0x49>;
        m_instructionSetDisassembly[0x4A] = &Instructions::ld_r_r_8_dis<0x4A>;
        m_instructionSetDisassembly[0x4B] = &Instructions::ld_r_r_8_dis<0x4B>;
        m_instructionSetDisassembly[0x4C] = &Instructions::ld_r_r_8_dis<0x4C>;
        m_instructionSetDisassembly[0x4D] = &Instructions::ld_r_r_8_dis<0x4D>;

        m_instructionSetDisassembly[0x4F] = &Instructions::ld_r_r_8_dis<0x4F>;
        m_instructionSetDisassembly[0x50] = &Instructions::ld_r_r_8_dis<0x50>;
        m_instructionSetDisassembly[0x51] = &Instructions::ld_r_r_8_dis<0x51>;
        m_instructionSetDisassembly[0x52] = &Instructions::ld_r_r_8_dis<0x52>;
        m_instructionSetDisassembly[0x53] = &Instructions::ld_r_r_8_dis<0x53>;
        m_instructionSetDisassembly[0x54] = &Instructions::ld_r_r_8_dis<0x54>;
        m_instructionSetDisassembly[0x55] = &Instructions::ld_r_r_8_dis<0x55>;

        m_instructionSetDisassembly[0x57] = &Instructions::ld_r_r_8_dis<0x57>;
        m_instructionSetDisassembly[0x58] = &Instructions::ld_r_r_8_dis<0x58>;
        m_instructionSetDisassembly[0x59] = &Instructions::ld_r_r_8_dis<0x59>;
        m_instructionSetDisassembly[0x5A] = &Instructions::ld_r_r_8_dis<0x5A>;
        m_instructionSetDisassembly[0x5B] = &Instructions::ld_r_r_8_dis<0x5B>;
        m_instructionSetDisassembly[0x5C] = &Instructions::ld_r_r_8_dis<0x5C>;
        m_instructionSetDisassembly[0x5D] = &Instructions::ld_r_r_8_dis<0x5D>;

        m_instructionSetDisassembly[0x5F] = &Instructions::ld_r_r_8_dis<0x5F>;
        m_instructionSetDisassembly[0x60] = &Instructions::ld_r_r_8_dis<0x60>;
        m_instructionSetDisassembly[0x61] = &Instructions::ld_r_r_8_dis<0x61>;
        m_instructionSetDisassembly[0x62] = &Instructions::ld_r_r_8_dis<0x62>;
        m_instructionSetDisassembly[0x63] = &Instructions::ld_r_r_8_dis<0x63>;
        m_instructionSetDisassembly[0x64] = &Instructions::ld_r_r_8_dis<0x64>;
        m_instructionSetDisassembly[0x65] = &Instructions::ld_r_r_8_dis<0x65>;

        m_instructionSetDisassembly[0x67] = &Instructions::ld_r_r_8_dis<0x67>;
        m_instructionSetDisassembly[0x68] = &Instructions::ld_r_r_8_dis<0x68>;
        m_instructionSetDisassembly[0x69] = &Instructions::ld_r_r_8_dis<0x69>;
        m_instructionSetDisassembly[0x6A] = &Instructions::ld_r_r_8_dis<0x6A>;
        m_instructionSetDisassembly[0x6B] = &Instructions::ld_r_r_8_dis<0x6B>;
        m_instructionSetDisassembly[0x6C] = &Instructions::ld_r_r_8_dis<0x6C>;
        m_instructionSetDisassembly[0x6D] = &Instructions::ld_r_r_8_dis<0x6D>;

        m_instructionSetDisassembly[0x6F] = &Instructions::ld_r_r_8_dis<0x6F>;
        m_instructionSetDisassembly[0x78] = &Instructions::ld_r_r_8_dis<0x78>;
        m_instructionSetDisassembly[0x79] = &Instructions::ld_r_r_8_dis<0x79>;
        m_instructionSetDisassembly[0x7A] = &Instructions::ld_r_r_8_dis<0x7A>;
        m_instructionSetDisassembly[0x7B] = &Instructions::ld_r_r_8_dis<0x7B>;
        m_instructionSetDisassembly[0x7C] = &Instructions::ld_r_r_8_dis<0x7C>;
        m_instructionSetDisassembly[0x7D] = &Instructions::ld_r_r_8_dis<0x7D>;
        m_instructionSetDisassembly[0x7F] = &Instructions::ld_r_r_8_dis<0x7F>;
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
        m_registers.incrementPC();

        uint8_t A = m_registers.read8<Registers::A>();
        m_registers.write8<Registers::BC>(A);

        return 2;
    }

    std::string Instructions::ld_BC_A_dis(uint8_t opCode, uint16_t, uint16_t)
    {
        return std::to_string(opCode) +  " : LD (BC), A;\n";
    }
    
}