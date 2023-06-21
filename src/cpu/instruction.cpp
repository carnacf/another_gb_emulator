#include "instruction.h"
#include "instruction.h"
#include "instruction.h"

#include "registery.h"
#include "memory.h"

#include <utility>

namespace cpu
{
    namespace
    {
        template<uint8_t opcode>
        constexpr Registers::Names opAFromOpCode()
        {
            constexpr uint8_t opAMask = 0b0011'1000;
            constexpr uint8_t opA = (opAMask & opcode) >> 3;

            return Registers::opFieldToName<opA>();
        }

        template<uint8_t opcode>
        constexpr Registers::Names opBFromOpCode()
        {
            constexpr uint8_t opBMask = 0b0000'0111;
            constexpr uint8_t opB = opBMask & opcode;

            return Registers::opFieldToName<opB>();
        }

        template<uint8_t opcode>
        constexpr std::pair<Registers::Names, Registers::Names> extractLDRROperands()
        {
            constexpr Registers::Names rA = opAFromOpCode<opcode>();
            constexpr Registers::Names rB = opBFromOpCode<opcode>();

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
        
        m_instructionSet[0x01] = &Instructions::ld_rr_nn<Registers::BC>;
        m_instructionSet[0x02] = &Instructions::ld_r16_A<Registers::BC>;
        m_instructionSet[0x06] = &Instructions::ld_r_n_8<0x06>;
        m_instructionSet[0x08] = &Instructions::ld_nn_SP;
        m_instructionSet[0x0A] = &Instructions::ld_A_r16<Registers::BC>;
        m_instructionSet[0x0E] = &Instructions::ld_r_n_8<0x0E>;
        m_instructionSet[0x11] = &Instructions::ld_rr_nn<Registers::DE>;
        m_instructionSet[0x12] = &Instructions::ld_r16_A<Registers::DE>;
        m_instructionSet[0x16] = &Instructions::ld_r_n_8<0x16>;
        m_instructionSet[0x1A] = &Instructions::ld_A_r16<Registers::DE>;
        m_instructionSet[0x1E] = &Instructions::ld_r_n_8<0x1E>;
        m_instructionSet[0x21] = &Instructions::ld_rr_nn<Registers::HL>;
        m_instructionSet[0x22] = &Instructions::ld_HLi_A;
        m_instructionSet[0x26] = &Instructions::ld_r_n_8<0x26>;
        m_instructionSet[0x2A] = &Instructions::ld_A_HLi;
        m_instructionSet[0x2E] = &Instructions::ld_r_n_8<0x2E>;
        m_instructionSet[0x31] = &Instructions::ld_SP_rr;
        m_instructionSet[0x32] = &Instructions::ld_HLd_A;
        m_instructionSet[0x36] = &Instructions::ld_HL_n_8<0x36>;
        m_instructionSet[0x3A] = &Instructions::ld_A_HLd;
        m_instructionSet[0x3E] = &Instructions::ld_r_n_8<0x3E>;

        m_instructionSet[0x40] = &Instructions::ld_r_r_8<0x40>;
        m_instructionSet[0x41] = &Instructions::ld_r_r_8<0x41>;
        m_instructionSet[0x42] = &Instructions::ld_r_r_8<0x42>;
        m_instructionSet[0x43] = &Instructions::ld_r_r_8<0x43>;
        m_instructionSet[0x44] = &Instructions::ld_r_r_8<0x44>;
        m_instructionSet[0x45] = &Instructions::ld_r_r_8<0x45>;
        m_instructionSet[0x46] = &Instructions::ld_r_HL<0x46>;
        m_instructionSet[0x47] = &Instructions::ld_r_r_8<0x47>;
        m_instructionSet[0x48] = &Instructions::ld_r_r_8<0x48>;
        m_instructionSet[0x49] = &Instructions::ld_r_r_8<0x49>;
        m_instructionSet[0x4A] = &Instructions::ld_r_r_8<0x4A>;
        m_instructionSet[0x4B] = &Instructions::ld_r_r_8<0x4B>;
        m_instructionSet[0x4C] = &Instructions::ld_r_r_8<0x4C>;
        m_instructionSet[0x4D] = &Instructions::ld_r_r_8<0x4D>;
        m_instructionSet[0x4E] = &Instructions::ld_r_HL<0x4E>;
        m_instructionSet[0x4F] = &Instructions::ld_r_r_8<0x4F>;
        m_instructionSet[0x50] = &Instructions::ld_r_r_8<0x50>;
        m_instructionSet[0x51] = &Instructions::ld_r_r_8<0x51>;
        m_instructionSet[0x52] = &Instructions::ld_r_r_8<0x52>;
        m_instructionSet[0x53] = &Instructions::ld_r_r_8<0x53>;
        m_instructionSet[0x54] = &Instructions::ld_r_r_8<0x54>;
        m_instructionSet[0x55] = &Instructions::ld_r_r_8<0x55>;
        m_instructionSet[0x56] = &Instructions::ld_r_HL<0x56>;
        m_instructionSet[0x57] = &Instructions::ld_r_r_8<0x57>;
        m_instructionSet[0x58] = &Instructions::ld_r_r_8<0x58>;
        m_instructionSet[0x59] = &Instructions::ld_r_r_8<0x59>;
        m_instructionSet[0x5A] = &Instructions::ld_r_r_8<0x5A>;
        m_instructionSet[0x5B] = &Instructions::ld_r_r_8<0x5B>;
        m_instructionSet[0x5C] = &Instructions::ld_r_r_8<0x5C>;
        m_instructionSet[0x5D] = &Instructions::ld_r_r_8<0x5D>;
        m_instructionSet[0x5E] = &Instructions::ld_r_HL<0x5E>;
        m_instructionSet[0x5F] = &Instructions::ld_r_r_8<0x5F>;
        m_instructionSet[0x60] = &Instructions::ld_r_r_8<0x60>;
        m_instructionSet[0x61] = &Instructions::ld_r_r_8<0x61>;
        m_instructionSet[0x62] = &Instructions::ld_r_r_8<0x62>;
        m_instructionSet[0x63] = &Instructions::ld_r_r_8<0x63>;
        m_instructionSet[0x64] = &Instructions::ld_r_r_8<0x64>;
        m_instructionSet[0x65] = &Instructions::ld_r_r_8<0x65>;
        m_instructionSet[0x66] = &Instructions::ld_r_HL<0x66>;
        m_instructionSet[0x67] = &Instructions::ld_r_r_8<0x67>;
        m_instructionSet[0x68] = &Instructions::ld_r_r_8<0x68>;
        m_instructionSet[0x69] = &Instructions::ld_r_r_8<0x69>;
        m_instructionSet[0x6A] = &Instructions::ld_r_r_8<0x6A>;
        m_instructionSet[0x6B] = &Instructions::ld_r_r_8<0x6B>;
        m_instructionSet[0x6C] = &Instructions::ld_r_r_8<0x6C>;
        m_instructionSet[0x6D] = &Instructions::ld_r_r_8<0x6D>;
        m_instructionSet[0x6E] = &Instructions::ld_r_HL<0x6E>;
        m_instructionSet[0x6F] = &Instructions::ld_r_r_8<0x6F>;
        m_instructionSet[0x70] = &Instructions::ld_HL_r<0x70>;
        m_instructionSet[0x71] = &Instructions::ld_HL_r<0x71>;
        m_instructionSet[0x72] = &Instructions::ld_HL_r<0x72>;
        m_instructionSet[0x73] = &Instructions::ld_HL_r<0x73>;
        m_instructionSet[0x74] = &Instructions::ld_HL_r<0x74>;
        m_instructionSet[0x75] = &Instructions::ld_HL_r<0x75>;
        
        m_instructionSet[0x77] = &Instructions::ld_HL_r<0x77>;
        m_instructionSet[0x78] = &Instructions::ld_r_r_8<0x78>;
        m_instructionSet[0x79] = &Instructions::ld_r_r_8<0x79>;
        m_instructionSet[0x7A] = &Instructions::ld_r_r_8<0x7A>;
        m_instructionSet[0x7B] = &Instructions::ld_r_r_8<0x7B>;
        m_instructionSet[0x7C] = &Instructions::ld_r_r_8<0x7C>;
        m_instructionSet[0x7D] = &Instructions::ld_r_r_8<0x7D>;
        m_instructionSet[0x7E] = &Instructions::ld_r_HL<0x7E>;
        m_instructionSet[0x7F] = &Instructions::ld_r_r_8<0x7F>;


        m_instructionSet[0xE0] = &Instructions::ldh_an_A;
        m_instructionSet[0xE2] = &Instructions::ldh_aC_A;
        m_instructionSet[0xEA] = &Instructions::ld_nn_A;
        m_instructionSet[0xF0] = &Instructions::ldh_A_an;
        m_instructionSet[0xF2] = &Instructions::ldh_A_aC;
        m_instructionSet[0xFA] = &Instructions::ld_A_nn;
    }

    void Instructions::fillCbInstructionSet()
    {
        std::fill_n(m_cbInstructionSet, 255, &Instructions::unhandled);
    }

    void Instructions::fillInstructionSetDisassembly()
    {
        std::fill_n(m_instructionSetDisassembly, 255, &Instructions::unhandledDisassembly);
       
        m_instructionSetDisassembly[0x01] = &Instructions::ld_rr_nn_dis<Registers::BC>;
        m_instructionSetDisassembly[0x02] = &Instructions::ld_r16_A_dis<Registers::BC>;
        m_instructionSetDisassembly[0x06] = &Instructions::ld_r_n_8_dis<0x06>;
        m_instructionSetDisassembly[0x08] = &Instructions::ld_nn_SP_dis;
        m_instructionSetDisassembly[0x0A] = &Instructions::ld_A_r16_dis<Registers::BC>;
        m_instructionSetDisassembly[0x0E] = &Instructions::ld_r_n_8_dis<0x0E>;
        m_instructionSetDisassembly[0x11] = &Instructions::ld_rr_nn_dis<Registers::DE>;
        m_instructionSetDisassembly[0x12] = &Instructions::ld_r16_A_dis<Registers::DE>;
        m_instructionSetDisassembly[0x16] = &Instructions::ld_r_n_8_dis<0x16>;
        m_instructionSetDisassembly[0x1A] = &Instructions::ld_A_r16_dis<Registers::DE>;
        m_instructionSetDisassembly[0x1E] = &Instructions::ld_r_n_8_dis<0x1E>;
        m_instructionSetDisassembly[0x21] = &Instructions::ld_rr_nn_dis<Registers::HL>;
        m_instructionSetDisassembly[0x22] = &Instructions::ld_HLi_A_dis;
        m_instructionSetDisassembly[0x26] = &Instructions::ld_r_n_8_dis<0x26>;
        m_instructionSetDisassembly[0x2A] = &Instructions::ld_A_HLi_dis;
        m_instructionSetDisassembly[0x2E] = &Instructions::ld_r_n_8_dis<0x2E>;
        m_instructionSetDisassembly[0x31] = &Instructions::ld_SP_rr_dis;
        m_instructionSetDisassembly[0x32] = &Instructions::ld_HLd_A_dis;
        m_instructionSetDisassembly[0x36] = &Instructions::ld_HL_n_8_dis<0x36>;
        m_instructionSetDisassembly[0x3A] = &Instructions::ld_A_HLd_dis;
        m_instructionSetDisassembly[0x3E] = &Instructions::ld_r_n_8_dis<0x3E>;

        m_instructionSetDisassembly[0x40] = &Instructions::ld_r_r_8_dis<0x40>;
        m_instructionSetDisassembly[0x41] = &Instructions::ld_r_r_8_dis<0x41>;
        m_instructionSetDisassembly[0x42] = &Instructions::ld_r_r_8_dis<0x42>;
        m_instructionSetDisassembly[0x43] = &Instructions::ld_r_r_8_dis<0x43>;
        m_instructionSetDisassembly[0x44] = &Instructions::ld_r_r_8_dis<0x44>;
        m_instructionSetDisassembly[0x45] = &Instructions::ld_r_r_8_dis<0x45>;
        m_instructionSetDisassembly[0x46] = &Instructions::ld_r_HL_dis<0x46>;
        m_instructionSetDisassembly[0x47] = &Instructions::ld_r_r_8_dis<0x47>;
        m_instructionSetDisassembly[0x48] = &Instructions::ld_r_r_8_dis<0x48>;
        m_instructionSetDisassembly[0x49] = &Instructions::ld_r_r_8_dis<0x49>;
        m_instructionSetDisassembly[0x4A] = &Instructions::ld_r_r_8_dis<0x4A>;
        m_instructionSetDisassembly[0x4B] = &Instructions::ld_r_r_8_dis<0x4B>;
        m_instructionSetDisassembly[0x4C] = &Instructions::ld_r_r_8_dis<0x4C>;
        m_instructionSetDisassembly[0x4D] = &Instructions::ld_r_r_8_dis<0x4D>;
        m_instructionSetDisassembly[0x4E] = &Instructions::ld_r_HL_dis<0x4E>;
        m_instructionSetDisassembly[0x4F] = &Instructions::ld_r_r_8_dis<0x4F>;
        m_instructionSetDisassembly[0x50] = &Instructions::ld_r_r_8_dis<0x50>;
        m_instructionSetDisassembly[0x51] = &Instructions::ld_r_r_8_dis<0x51>;
        m_instructionSetDisassembly[0x52] = &Instructions::ld_r_r_8_dis<0x52>;
        m_instructionSetDisassembly[0x53] = &Instructions::ld_r_r_8_dis<0x53>;
        m_instructionSetDisassembly[0x54] = &Instructions::ld_r_r_8_dis<0x54>;
        m_instructionSetDisassembly[0x55] = &Instructions::ld_r_r_8_dis<0x55>;
        m_instructionSetDisassembly[0x56] = &Instructions::ld_r_HL_dis<0x56>;
        m_instructionSetDisassembly[0x57] = &Instructions::ld_r_r_8_dis<0x57>;
        m_instructionSetDisassembly[0x58] = &Instructions::ld_r_r_8_dis<0x58>;
        m_instructionSetDisassembly[0x59] = &Instructions::ld_r_r_8_dis<0x59>;
        m_instructionSetDisassembly[0x5A] = &Instructions::ld_r_r_8_dis<0x5A>;
        m_instructionSetDisassembly[0x5B] = &Instructions::ld_r_r_8_dis<0x5B>;
        m_instructionSetDisassembly[0x5C] = &Instructions::ld_r_r_8_dis<0x5C>;
        m_instructionSetDisassembly[0x5D] = &Instructions::ld_r_r_8_dis<0x5D>;
        m_instructionSetDisassembly[0x5E] = &Instructions::ld_r_HL_dis<0x5E>;
        m_instructionSetDisassembly[0x5F] = &Instructions::ld_r_r_8_dis<0x5F>;
        m_instructionSetDisassembly[0x60] = &Instructions::ld_r_r_8_dis<0x60>;
        m_instructionSetDisassembly[0x61] = &Instructions::ld_r_r_8_dis<0x61>;
        m_instructionSetDisassembly[0x62] = &Instructions::ld_r_r_8_dis<0x62>;
        m_instructionSetDisassembly[0x63] = &Instructions::ld_r_r_8_dis<0x63>;
        m_instructionSetDisassembly[0x64] = &Instructions::ld_r_r_8_dis<0x64>;
        m_instructionSetDisassembly[0x65] = &Instructions::ld_r_r_8_dis<0x65>;
        m_instructionSetDisassembly[0x66] = &Instructions::ld_r_HL_dis<0x66>;
        m_instructionSetDisassembly[0x67] = &Instructions::ld_r_r_8_dis<0x67>;
        m_instructionSetDisassembly[0x68] = &Instructions::ld_r_r_8_dis<0x68>;
        m_instructionSetDisassembly[0x69] = &Instructions::ld_r_r_8_dis<0x69>;
        m_instructionSetDisassembly[0x6A] = &Instructions::ld_r_r_8_dis<0x6A>;
        m_instructionSetDisassembly[0x6B] = &Instructions::ld_r_r_8_dis<0x6B>;
        m_instructionSetDisassembly[0x6C] = &Instructions::ld_r_r_8_dis<0x6C>;
        m_instructionSetDisassembly[0x6D] = &Instructions::ld_r_r_8_dis<0x6D>;
        m_instructionSetDisassembly[0x6E] = &Instructions::ld_r_HL_dis<0x6E>;
        m_instructionSetDisassembly[0x6F] = &Instructions::ld_r_r_8_dis<0x6F>;
        m_instructionSetDisassembly[0x70] = &Instructions::ld_HL_r_dis<0x70>;
        m_instructionSetDisassembly[0x71] = &Instructions::ld_HL_r_dis<0x71>;
        m_instructionSetDisassembly[0x72] = &Instructions::ld_HL_r_dis<0x72>;
        m_instructionSetDisassembly[0x73] = &Instructions::ld_HL_r_dis<0x73>;
        m_instructionSetDisassembly[0x74] = &Instructions::ld_HL_r_dis<0x74>;
        m_instructionSetDisassembly[0x75] = &Instructions::ld_HL_r_dis<0x75>;
        
        m_instructionSetDisassembly[0x77] = &Instructions::ld_HL_r_dis<0x77>;
        m_instructionSetDisassembly[0x78] = &Instructions::ld_r_r_8_dis<0x78>;
        m_instructionSetDisassembly[0x79] = &Instructions::ld_r_r_8_dis<0x79>;
        m_instructionSetDisassembly[0x7A] = &Instructions::ld_r_r_8_dis<0x7A>;
        m_instructionSetDisassembly[0x7B] = &Instructions::ld_r_r_8_dis<0x7B>;
        m_instructionSetDisassembly[0x7C] = &Instructions::ld_r_r_8_dis<0x7C>;
        m_instructionSetDisassembly[0x7D] = &Instructions::ld_r_r_8_dis<0x7D>;
        m_instructionSetDisassembly[0x7E] = &Instructions::ld_r_HL_dis<0x7E>;
        m_instructionSetDisassembly[0x7F] = &Instructions::ld_r_r_8_dis<0x7F>;
        
        m_instructionSetDisassembly[0xE0] = &Instructions::ldh_an_A_dis;
        m_instructionSetDisassembly[0xE2] = &Instructions::ldh_aC_A_dis;
        m_instructionSetDisassembly[0xEA] = &Instructions::ld_nn_A_dis;
        m_instructionSetDisassembly[0xF0] = &Instructions::ldh_A_an_dis;
        m_instructionSetDisassembly[0xF2] = &Instructions::ldh_A_aC_dis;
        m_instructionSetDisassembly[0xFA] = &Instructions::ld_A_nn_dis;
    }

    void Instructions::fillCbInstructionSetDisassembly()
    {
        std::fill_n(m_cbInstructionSetDisassembly, 255, &Instructions::unhandledDisassembly);
    }

    int Instructions::unhandled(uint16_t, uint16_t) 
    {
        m_registers.incrementPC();
        return 1;
    }
    std::string Instructions::unhandledDisassembly(uint8_t opCode, uint16_t, uint16_t)
    {
        return std::to_string(opCode) +  " : Not handled yet;\n";
    }
    
    int Instructions::ld_A_nn(uint16_t opA, uint16_t opB)
    {
        m_registers.incrementPC();
        
        uint16_t nn = m_memory.read16(m_registers.getPC());
        m_registers.incrementPC();
        m_registers.incrementPC();

        uint8_t val = m_memory.read8(nn);
        m_registers.write8<Registers::A>(val);
        return 4;
    }
    std::string Instructions::ld_A_nn_dis(uint8_t, uint16_t, uint16_t)
    {
        return "LD A, nn;\n";
    }

    int Instructions::ld_nn_A(uint16_t opA, uint16_t opB)
    {
        m_registers.incrementPC();

        uint16_t nn = m_memory.read16(m_registers.getPC());
        m_registers.incrementPC();
        m_registers.incrementPC();

        uint8_t a = m_registers.read8<Registers::A>();
        m_memory.write8(nn, a);

        return 4;
    }
    std::string Instructions::ld_nn_A_dis(uint8_t, uint16_t, uint16_t)
    {
        return "LD nn, A;";
    }

    int Instructions::ldh_A_aC(uint16_t opA, uint16_t opB)
    {
        m_registers.incrementPC();

        uint8_t c = m_registers.read8<Registers::C>();
        uint16_t addr = utils::to16(0xFF, c);
        
        uint8_t val = m_memory.read8(addr);
        m_registers.write8<Registers::A>(val);

        return 2;
    }
    std::string Instructions::ldh_A_aC_dis(uint8_t, uint16_t, uint16_t)
    {
        return "LDH A, (C);\n";
    }

    int Instructions::ldh_aC_A(uint16_t opA, uint16_t opB)
    {
        m_registers.incrementPC();

        uint8_t c = m_registers.read8<Registers::C>();
        uint16_t addr = utils::to16(0xFF, c);

        uint8_t a = m_registers.read8<Registers::A>();

        m_memory.write8(addr, a);

        return 2;
    }
    std::string Instructions::ldh_aC_A_dis(uint8_t, uint16_t, uint16_t)
    {
        return "LDH (C), A;\n";
    }

    int Instructions::ldh_A_an(uint16_t opA, uint16_t opB)
    {
        m_registers.incrementPC();

        uint8_t n = m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();
        uint16_t addr = utils::to16(0xFF, n);

        uint8_t val = m_memory.read8(addr);
        m_registers.write8<Registers::A>(val);

        return 3;
    }
    std::string Instructions::ldh_A_an_dis(uint8_t, uint16_t, uint16_t)
    {
        return "LDH A, (n);\n";
    }

    int Instructions::ldh_an_A(uint16_t opA, uint16_t opB)
    {
        m_registers.incrementPC();

        uint8_t n = m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();
        uint16_t addr = utils::to16(0xFF, n);

        uint8_t a = m_registers.read8<Registers::A>();

        m_memory.write8(addr, a);
        return 3;
    }
    std::string Instructions::ldh_an_A_dis(uint8_t, uint16_t, uint16_t)
    {
        return "LDH (n), A;\n";
    }

    int Instructions::ld_HLd_A(uint16_t, uint16_t)
    {
        m_registers.incrementPC();

        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);

        m_registers.write16<Registers::HL>(--hl);
        m_registers.write8<Registers::A>(val);

        return 2;

    }
    std::string Instructions::ld_HLd_A_dis(uint8_t, uint16_t, uint16_t)
    {
        return "LD (HL-), A;\n";
    }
        
    int Instructions::ld_A_HLd(uint16_t, uint16_t)
    {
        m_registers.incrementPC();

        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t a = m_registers.read8<Registers::A>();
        m_memory.write8(hl, a);

        m_registers.write16<Registers::HL>(--hl);

        return 2;
    }
    std::string Instructions::ld_A_HLd_dis(uint8_t, uint16_t, uint16_t)
    {
        return "LD A, (HL-);\n";
    }
        
    int Instructions::ld_HLi_A(uint16_t, uint16_t)
    {
        m_registers.incrementPC();

        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);

        m_registers.write16<Registers::HL>(++hl);
        m_registers.write8<Registers::A>(val);

        return 2;
    }
    std::string Instructions::ld_HLi_A_dis(uint8_t, uint16_t, uint16_t)
    {
        return "LD (HL+), A;\n";
    }
    
    int Instructions::ld_A_HLi(uint16_t, uint16_t)
    {
        m_registers.incrementPC();

        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t a = m_registers.read8<Registers::A>();
        m_memory.write8(hl, a);

        m_registers.write16<Registers::HL>(++hl);

        return 2;
    }
    std::string Instructions::ld_A_HLi_dis(uint8_t, uint16_t, uint16_t)
    {
        return "LD A, (HL+);\n";
    }

    int Instructions::ld_SP_rr(uint16_t, uint16_t)
    {
        m_registers.incrementPC();

        uint16_t val = m_memory.read16(m_registers.getPC());
        m_registers.incrementPC();
        m_registers.incrementPC();

        m_registers.setSP(val);

        return 3;
    }
    std::string Instructions::ld_SP_rr_dis(uint8_t, uint16_t, uint16_t)
    {
        return "LD rr, SP;\n";
    }

    int Instructions::ld_nn_SP(uint16_t, uint16_t)
    {
        m_registers.incrementPC();

        uint16_t addr = m_memory.read16(m_registers.getPC());
        m_registers.incrementPC();
        m_registers.incrementPC();

        m_memory.write16(addr, m_registers.getSP());

        return 5;
    }

    std::string Instructions::ld_nn_SP_dis(uint8_t, uint16_t, uint16_t)
    {
        return "LD SP, rr;\n";
    }
}