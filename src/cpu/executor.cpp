#include "executor.h"

#include "registery.h"
#include "memory.h"

#include <utility>
#include <limits>

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

    Executor::Executor(Registers& regist, Memory& mem): m_registers(regist), m_memory(mem)
    {
        fillInstructionSet();
        fillCbInstructionSet();

        fillInstructionSetDisassembly();
        fillCbInstructionSetDisassembly();
    }
    
    void Executor::fillInstructionSet()
    {
        std::fill_n(m_instructionSet, 255, &Executor::unhandled);
        
        m_instructionSet[0x01] = &Executor::ld_rr_nn<Registers::BC>;
        m_instructionSet[0x02] = &Executor::ld_r16_A<Registers::BC>;
        m_instructionSet[0x06] = &Executor::ld_r_n_8<0x06>;
        m_instructionSet[0x08] = &Executor::ld_nn_SP;
        m_instructionSet[0x0A] = &Executor::ld_A_r16<Registers::BC>;
        m_instructionSet[0x0E] = &Executor::ld_r_n_8<0x0E>;
        m_instructionSet[0x11] = &Executor::ld_rr_nn<Registers::DE>;
        m_instructionSet[0x12] = &Executor::ld_r16_A<Registers::DE>;
        m_instructionSet[0x16] = &Executor::ld_r_n_8<0x16>;
        m_instructionSet[0x1A] = &Executor::ld_A_r16<Registers::DE>;
        m_instructionSet[0x1E] = &Executor::ld_r_n_8<0x1E>;
        m_instructionSet[0x21] = &Executor::ld_rr_nn<Registers::HL>;
        m_instructionSet[0x22] = &Executor::ld_HLi_A;
        m_instructionSet[0x26] = &Executor::ld_r_n_8<0x26>;
        m_instructionSet[0x2A] = &Executor::ld_A_HLi;
        m_instructionSet[0x2E] = &Executor::ld_r_n_8<0x2E>;
        m_instructionSet[0x31] = &Executor::ld_SP_rr;
        m_instructionSet[0x32] = &Executor::ld_HLd_A;
        m_instructionSet[0x36] = &Executor::ld_HL_n_8<0x36>;
        m_instructionSet[0x3A] = &Executor::ld_A_HLd;
        m_instructionSet[0x3E] = &Executor::ld_r_n_8<0x3E>;

        m_instructionSet[0x40] = &Executor::ld_r_r_8<0x40>;
        m_instructionSet[0x41] = &Executor::ld_r_r_8<0x41>;
        m_instructionSet[0x42] = &Executor::ld_r_r_8<0x42>;
        m_instructionSet[0x43] = &Executor::ld_r_r_8<0x43>;
        m_instructionSet[0x44] = &Executor::ld_r_r_8<0x44>;
        m_instructionSet[0x45] = &Executor::ld_r_r_8<0x45>;
        m_instructionSet[0x46] = &Executor::ld_r_HL<0x46>;
        m_instructionSet[0x47] = &Executor::ld_r_r_8<0x47>;
        m_instructionSet[0x48] = &Executor::ld_r_r_8<0x48>;
        m_instructionSet[0x49] = &Executor::ld_r_r_8<0x49>;
        m_instructionSet[0x4A] = &Executor::ld_r_r_8<0x4A>;
        m_instructionSet[0x4B] = &Executor::ld_r_r_8<0x4B>;
        m_instructionSet[0x4C] = &Executor::ld_r_r_8<0x4C>;
        m_instructionSet[0x4D] = &Executor::ld_r_r_8<0x4D>;
        m_instructionSet[0x4E] = &Executor::ld_r_HL<0x4E>;
        m_instructionSet[0x4F] = &Executor::ld_r_r_8<0x4F>;
        m_instructionSet[0x50] = &Executor::ld_r_r_8<0x50>;
        m_instructionSet[0x51] = &Executor::ld_r_r_8<0x51>;
        m_instructionSet[0x52] = &Executor::ld_r_r_8<0x52>;
        m_instructionSet[0x53] = &Executor::ld_r_r_8<0x53>;
        m_instructionSet[0x54] = &Executor::ld_r_r_8<0x54>;
        m_instructionSet[0x55] = &Executor::ld_r_r_8<0x55>;
        m_instructionSet[0x56] = &Executor::ld_r_HL<0x56>;
        m_instructionSet[0x57] = &Executor::ld_r_r_8<0x57>;
        m_instructionSet[0x58] = &Executor::ld_r_r_8<0x58>;
        m_instructionSet[0x59] = &Executor::ld_r_r_8<0x59>;
        m_instructionSet[0x5A] = &Executor::ld_r_r_8<0x5A>;
        m_instructionSet[0x5B] = &Executor::ld_r_r_8<0x5B>;
        m_instructionSet[0x5C] = &Executor::ld_r_r_8<0x5C>;
        m_instructionSet[0x5D] = &Executor::ld_r_r_8<0x5D>;
        m_instructionSet[0x5E] = &Executor::ld_r_HL<0x5E>;
        m_instructionSet[0x5F] = &Executor::ld_r_r_8<0x5F>;
        m_instructionSet[0x60] = &Executor::ld_r_r_8<0x60>;
        m_instructionSet[0x61] = &Executor::ld_r_r_8<0x61>;
        m_instructionSet[0x62] = &Executor::ld_r_r_8<0x62>;
        m_instructionSet[0x63] = &Executor::ld_r_r_8<0x63>;
        m_instructionSet[0x64] = &Executor::ld_r_r_8<0x64>;
        m_instructionSet[0x65] = &Executor::ld_r_r_8<0x65>;
        m_instructionSet[0x66] = &Executor::ld_r_HL<0x66>;
        m_instructionSet[0x67] = &Executor::ld_r_r_8<0x67>;
        m_instructionSet[0x68] = &Executor::ld_r_r_8<0x68>;
        m_instructionSet[0x69] = &Executor::ld_r_r_8<0x69>;
        m_instructionSet[0x6A] = &Executor::ld_r_r_8<0x6A>;
        m_instructionSet[0x6B] = &Executor::ld_r_r_8<0x6B>;
        m_instructionSet[0x6C] = &Executor::ld_r_r_8<0x6C>;
        m_instructionSet[0x6D] = &Executor::ld_r_r_8<0x6D>;
        m_instructionSet[0x6E] = &Executor::ld_r_HL<0x6E>;
        m_instructionSet[0x6F] = &Executor::ld_r_r_8<0x6F>;
        m_instructionSet[0x70] = &Executor::ld_HL_r<0x70>;
        m_instructionSet[0x71] = &Executor::ld_HL_r<0x71>;
        m_instructionSet[0x72] = &Executor::ld_HL_r<0x72>;
        m_instructionSet[0x73] = &Executor::ld_HL_r<0x73>;
        m_instructionSet[0x74] = &Executor::ld_HL_r<0x74>;
        m_instructionSet[0x75] = &Executor::ld_HL_r<0x75>;
        
        m_instructionSet[0x77] = &Executor::ld_HL_r<0x77>;
        m_instructionSet[0x78] = &Executor::ld_r_r_8<0x78>;
        m_instructionSet[0x79] = &Executor::ld_r_r_8<0x79>;
        m_instructionSet[0x7A] = &Executor::ld_r_r_8<0x7A>;
        m_instructionSet[0x7B] = &Executor::ld_r_r_8<0x7B>;
        m_instructionSet[0x7C] = &Executor::ld_r_r_8<0x7C>;
        m_instructionSet[0x7D] = &Executor::ld_r_r_8<0x7D>;
        m_instructionSet[0x7E] = &Executor::ld_r_HL<0x7E>;
        m_instructionSet[0x7F] = &Executor::ld_r_r_8<0x7F>;
        m_instructionSet[0x80] = &Executor::add_r<Registers::B>;
        m_instructionSet[0x81] = &Executor::add_r<Registers::C>;
        m_instructionSet[0x82] = &Executor::add_r<Registers::D>;
        m_instructionSet[0x83] = &Executor::add_r<Registers::E>;
        m_instructionSet[0x84] = &Executor::add_r<Registers::H>;
        m_instructionSet[0x85] = &Executor::add_r<Registers::L>;
        m_instructionSet[0x86] = &Executor::add_HL;
        m_instructionSet[0x87] = &Executor::add_r<Registers::A>;
        m_instructionSet[0x88] = &Executor::adc_r<Registers::B>;
        m_instructionSet[0x89] = &Executor::adc_r<Registers::C>;
        m_instructionSet[0x8A] = &Executor::adc_r<Registers::D>;
        m_instructionSet[0x8B] = &Executor::adc_r<Registers::E>;
        m_instructionSet[0x8C] = &Executor::adc_r<Registers::H>;
        m_instructionSet[0x8D] = &Executor::adc_r<Registers::L>;
        m_instructionSet[0x8E] = &Executor::adc_HL;
        m_instructionSet[0x8F] = &Executor::adc_r<Registers::A>;


        m_instructionSet[0xC1] = &Executor::pop<Registers::BC>;
        m_instructionSet[0xC5] = &Executor::push<Registers::BC>;
        m_instructionSet[0xC6] = &Executor::add_n;
        m_instructionSet[0xCE] = &Executor::adc_n;
        m_instructionSet[0xD1] = &Executor::pop<Registers::DE>;
        m_instructionSet[0xD5] = &Executor::push<Registers::DE>;
        m_instructionSet[0xE0] = &Executor::ldh_an_A;
        m_instructionSet[0xE1] = &Executor::pop<Registers::HL>;
        m_instructionSet[0xE2] = &Executor::ldh_aC_A;
        m_instructionSet[0xE5] = &Executor::push<Registers::HL>;
        m_instructionSet[0xEA] = &Executor::ld_nn_A;
        m_instructionSet[0xF0] = &Executor::ldh_A_an;
        m_instructionSet[0xF1] = &Executor::pop<Registers::AF>;
        m_instructionSet[0xF2] = &Executor::ldh_A_aC;
        m_instructionSet[0xF5] = &Executor::push<Registers::AF>;
        m_instructionSet[0xF8] = &Executor::ld_HL_SP_r8;
        m_instructionSet[0xF9] = &Executor::ld_SP_HL;
        m_instructionSet[0xFA] = &Executor::ld_A_nn;
    }

    void Executor::fillCbInstructionSet()
    {
        std::fill_n(m_cbInstructionSet, 255, &Executor::unhandled);
    }

    void Executor::fillInstructionSetDisassembly()
    {
        std::fill_n(m_instructionSetDisassembly, 255, &Executor::unhandledDisassembly);
       
        m_instructionSetDisassembly[0x01] = &Executor::ld_rr_nn_dis<Registers::BC>;
        m_instructionSetDisassembly[0x02] = &Executor::ld_r16_A_dis<Registers::BC>;
        m_instructionSetDisassembly[0x06] = &Executor::ld_r_n_8_dis<0x06>;
        m_instructionSetDisassembly[0x08] = &Executor::ld_nn_SP_dis;
        m_instructionSetDisassembly[0x0A] = &Executor::ld_A_r16_dis<Registers::BC>;
        m_instructionSetDisassembly[0x0E] = &Executor::ld_r_n_8_dis<0x0E>;
        m_instructionSetDisassembly[0x11] = &Executor::ld_rr_nn_dis<Registers::DE>;
        m_instructionSetDisassembly[0x12] = &Executor::ld_r16_A_dis<Registers::DE>;
        m_instructionSetDisassembly[0x16] = &Executor::ld_r_n_8_dis<0x16>;
        m_instructionSetDisassembly[0x1A] = &Executor::ld_A_r16_dis<Registers::DE>;
        m_instructionSetDisassembly[0x1E] = &Executor::ld_r_n_8_dis<0x1E>;
        m_instructionSetDisassembly[0x21] = &Executor::ld_rr_nn_dis<Registers::HL>;
        m_instructionSetDisassembly[0x22] = &Executor::ld_HLi_A_dis;
        m_instructionSetDisassembly[0x26] = &Executor::ld_r_n_8_dis<0x26>;
        m_instructionSetDisassembly[0x2A] = &Executor::ld_A_HLi_dis;
        m_instructionSetDisassembly[0x2E] = &Executor::ld_r_n_8_dis<0x2E>;
        m_instructionSetDisassembly[0x31] = &Executor::ld_SP_rr_dis;
        m_instructionSetDisassembly[0x32] = &Executor::ld_HLd_A_dis;
        m_instructionSetDisassembly[0x36] = &Executor::ld_HL_n_8_dis<0x36>;
        m_instructionSetDisassembly[0x3A] = &Executor::ld_A_HLd_dis;
        m_instructionSetDisassembly[0x3E] = &Executor::ld_r_n_8_dis<0x3E>;

        m_instructionSetDisassembly[0x40] = &Executor::ld_r_r_8_dis<0x40>;
        m_instructionSetDisassembly[0x41] = &Executor::ld_r_r_8_dis<0x41>;
        m_instructionSetDisassembly[0x42] = &Executor::ld_r_r_8_dis<0x42>;
        m_instructionSetDisassembly[0x43] = &Executor::ld_r_r_8_dis<0x43>;
        m_instructionSetDisassembly[0x44] = &Executor::ld_r_r_8_dis<0x44>;
        m_instructionSetDisassembly[0x45] = &Executor::ld_r_r_8_dis<0x45>;
        m_instructionSetDisassembly[0x46] = &Executor::ld_r_HL_dis<0x46>;
        m_instructionSetDisassembly[0x47] = &Executor::ld_r_r_8_dis<0x47>;
        m_instructionSetDisassembly[0x48] = &Executor::ld_r_r_8_dis<0x48>;
        m_instructionSetDisassembly[0x49] = &Executor::ld_r_r_8_dis<0x49>;
        m_instructionSetDisassembly[0x4A] = &Executor::ld_r_r_8_dis<0x4A>;
        m_instructionSetDisassembly[0x4B] = &Executor::ld_r_r_8_dis<0x4B>;
        m_instructionSetDisassembly[0x4C] = &Executor::ld_r_r_8_dis<0x4C>;
        m_instructionSetDisassembly[0x4D] = &Executor::ld_r_r_8_dis<0x4D>;
        m_instructionSetDisassembly[0x4E] = &Executor::ld_r_HL_dis<0x4E>;
        m_instructionSetDisassembly[0x4F] = &Executor::ld_r_r_8_dis<0x4F>;
        m_instructionSetDisassembly[0x50] = &Executor::ld_r_r_8_dis<0x50>;
        m_instructionSetDisassembly[0x51] = &Executor::ld_r_r_8_dis<0x51>;
        m_instructionSetDisassembly[0x52] = &Executor::ld_r_r_8_dis<0x52>;
        m_instructionSetDisassembly[0x53] = &Executor::ld_r_r_8_dis<0x53>;
        m_instructionSetDisassembly[0x54] = &Executor::ld_r_r_8_dis<0x54>;
        m_instructionSetDisassembly[0x55] = &Executor::ld_r_r_8_dis<0x55>;
        m_instructionSetDisassembly[0x56] = &Executor::ld_r_HL_dis<0x56>;
        m_instructionSetDisassembly[0x57] = &Executor::ld_r_r_8_dis<0x57>;
        m_instructionSetDisassembly[0x58] = &Executor::ld_r_r_8_dis<0x58>;
        m_instructionSetDisassembly[0x59] = &Executor::ld_r_r_8_dis<0x59>;
        m_instructionSetDisassembly[0x5A] = &Executor::ld_r_r_8_dis<0x5A>;
        m_instructionSetDisassembly[0x5B] = &Executor::ld_r_r_8_dis<0x5B>;
        m_instructionSetDisassembly[0x5C] = &Executor::ld_r_r_8_dis<0x5C>;
        m_instructionSetDisassembly[0x5D] = &Executor::ld_r_r_8_dis<0x5D>;
        m_instructionSetDisassembly[0x5E] = &Executor::ld_r_HL_dis<0x5E>;
        m_instructionSetDisassembly[0x5F] = &Executor::ld_r_r_8_dis<0x5F>;
        m_instructionSetDisassembly[0x60] = &Executor::ld_r_r_8_dis<0x60>;
        m_instructionSetDisassembly[0x61] = &Executor::ld_r_r_8_dis<0x61>;
        m_instructionSetDisassembly[0x62] = &Executor::ld_r_r_8_dis<0x62>;
        m_instructionSetDisassembly[0x63] = &Executor::ld_r_r_8_dis<0x63>;
        m_instructionSetDisassembly[0x64] = &Executor::ld_r_r_8_dis<0x64>;
        m_instructionSetDisassembly[0x65] = &Executor::ld_r_r_8_dis<0x65>;
        m_instructionSetDisassembly[0x66] = &Executor::ld_r_HL_dis<0x66>;
        m_instructionSetDisassembly[0x67] = &Executor::ld_r_r_8_dis<0x67>;
        m_instructionSetDisassembly[0x68] = &Executor::ld_r_r_8_dis<0x68>;
        m_instructionSetDisassembly[0x69] = &Executor::ld_r_r_8_dis<0x69>;
        m_instructionSetDisassembly[0x6A] = &Executor::ld_r_r_8_dis<0x6A>;
        m_instructionSetDisassembly[0x6B] = &Executor::ld_r_r_8_dis<0x6B>;
        m_instructionSetDisassembly[0x6C] = &Executor::ld_r_r_8_dis<0x6C>;
        m_instructionSetDisassembly[0x6D] = &Executor::ld_r_r_8_dis<0x6D>;
        m_instructionSetDisassembly[0x6E] = &Executor::ld_r_HL_dis<0x6E>;
        m_instructionSetDisassembly[0x6F] = &Executor::ld_r_r_8_dis<0x6F>;
        m_instructionSetDisassembly[0x70] = &Executor::ld_HL_r_dis<0x70>;
        m_instructionSetDisassembly[0x71] = &Executor::ld_HL_r_dis<0x71>;
        m_instructionSetDisassembly[0x72] = &Executor::ld_HL_r_dis<0x72>;
        m_instructionSetDisassembly[0x73] = &Executor::ld_HL_r_dis<0x73>;
        m_instructionSetDisassembly[0x74] = &Executor::ld_HL_r_dis<0x74>;
        m_instructionSetDisassembly[0x75] = &Executor::ld_HL_r_dis<0x75>;
        
        m_instructionSetDisassembly[0x77] = &Executor::ld_HL_r_dis<0x77>;
        m_instructionSetDisassembly[0x78] = &Executor::ld_r_r_8_dis<0x78>;
        m_instructionSetDisassembly[0x79] = &Executor::ld_r_r_8_dis<0x79>;
        m_instructionSetDisassembly[0x7A] = &Executor::ld_r_r_8_dis<0x7A>;
        m_instructionSetDisassembly[0x7B] = &Executor::ld_r_r_8_dis<0x7B>;
        m_instructionSetDisassembly[0x7C] = &Executor::ld_r_r_8_dis<0x7C>;
        m_instructionSetDisassembly[0x7D] = &Executor::ld_r_r_8_dis<0x7D>;
        m_instructionSetDisassembly[0x7E] = &Executor::ld_r_HL_dis<0x7E>;
        m_instructionSetDisassembly[0x7F] = &Executor::ld_r_r_8_dis<0x7F>;
        m_instructionSetDisassembly[0x80] = &Executor::add_r_dis<Registers::B>;
        m_instructionSetDisassembly[0x81] = &Executor::add_r_dis<Registers::C>;
        m_instructionSetDisassembly[0x82] = &Executor::add_r_dis<Registers::D>;
        m_instructionSetDisassembly[0x83] = &Executor::add_r_dis<Registers::E>;
        m_instructionSetDisassembly[0x84] = &Executor::add_r_dis<Registers::H>;
        m_instructionSetDisassembly[0x85] = &Executor::add_r_dis<Registers::L>;
        m_instructionSetDisassembly[0x86] = &Executor::add_HL_dis;
        m_instructionSetDisassembly[0x87] = &Executor::add_r_dis<Registers::A>;
        m_instructionSetDisassembly[0x88] = &Executor::adc_r_dis<Registers::B>;
        m_instructionSetDisassembly[0x89] = &Executor::adc_r_dis<Registers::C>;
        m_instructionSetDisassembly[0x8A] = &Executor::adc_r_dis<Registers::D>;
        m_instructionSetDisassembly[0x8B] = &Executor::adc_r_dis<Registers::E>;
        m_instructionSetDisassembly[0x8C] = &Executor::adc_r_dis<Registers::H>;
        m_instructionSetDisassembly[0x8D] = &Executor::adc_r_dis<Registers::L>;
        m_instructionSetDisassembly[0x8E] = &Executor::adc_HL_dis;
        m_instructionSetDisassembly[0x8F] = &Executor::adc_r_dis<Registers::A>;

        m_instructionSetDisassembly[0xC1] = &Executor::pop_dis<Registers::BC>;
        m_instructionSetDisassembly[0xC5] = &Executor::push_dis<Registers::BC>;
        m_instructionSetDisassembly[0xC6] = &Executor::add_n_dis;
        m_instructionSetDisassembly[0xCE] = &Executor::adc_n_dis;
        m_instructionSetDisassembly[0xD1] = &Executor::pop_dis<Registers::DE>;
        m_instructionSetDisassembly[0xD5] = &Executor::push_dis<Registers::DE>;
        m_instructionSetDisassembly[0xE0] = &Executor::ldh_an_A_dis;
        m_instructionSetDisassembly[0xE1] = &Executor::pop_dis<Registers::HL>;
        m_instructionSetDisassembly[0xE2] = &Executor::ldh_aC_A_dis;
        m_instructionSetDisassembly[0xE5] = &Executor::push_dis<Registers::HL>;
        m_instructionSetDisassembly[0xEA] = &Executor::ld_nn_A_dis;
        m_instructionSetDisassembly[0xF0] = &Executor::ldh_A_an_dis;
        m_instructionSetDisassembly[0xF1] = &Executor::pop_dis<Registers::AF>;
        m_instructionSetDisassembly[0xF2] = &Executor::ldh_A_aC_dis;
        m_instructionSetDisassembly[0xF5] = &Executor::push_dis<Registers::AF>;
        m_instructionSetDisassembly[0xF8] = &Executor::ld_HL_SP_r8_dis;
        m_instructionSetDisassembly[0xF9] = &Executor::ld_SP_HL_dis;
        m_instructionSetDisassembly[0xFA] = &Executor::ld_A_nn_dis;
    }

    void Executor::fillCbInstructionSetDisassembly()
    {
        std::fill_n(m_cbInstructionSetDisassembly, 255, &Executor::unhandledDisassembly);
    }

    int Executor::unhandled() 
    {
        return 1;
    }
    std::string Executor::unhandledDisassembly(uint8_t opCode)
    {
        return std::to_string(opCode) +  " : Not handled yet;\n";
    }
    
    int Executor::ld_A_nn()
    {
        uint16_t nn = m_memory.read16(m_registers.getPC());
        m_registers.incrementPC();
        m_registers.incrementPC();

        uint8_t val = m_memory.read8(nn);
        m_registers.write8<Registers::A>(val);
        return 4;
    }
    std::string Executor::ld_A_nn_dis(uint8_t /*opCode*/)
    {
        return "LD A, nn;\n";
    }

    int Executor::ld_nn_A()
    {
        uint16_t nn = m_memory.read16(m_registers.getPC());
        m_registers.incrementPC();
        m_registers.incrementPC();

        uint8_t a = m_registers.read8<Registers::A>();
        m_memory.write8(nn, a);

        return 4;
    }
    std::string Executor::ld_nn_A_dis(uint8_t opCode)
    {
        return "LD nn, A;";
    }

    int Executor::ldh_A_aC()
    {
        uint8_t c = m_registers.read8<Registers::C>();
        uint16_t addr = utils::to16(0xFF, c);
        
        uint8_t val = m_memory.read8(addr);
        m_registers.write8<Registers::A>(val);

        return 2;
    }
    std::string Executor::ldh_A_aC_dis(uint8_t opCode)
    {
        return "LDH A, (C);\n";
    }

    int Executor::ldh_aC_A()
    {
        uint8_t c = m_registers.read8<Registers::C>();
        uint16_t addr = utils::to16(0xFF, c);

        uint8_t a = m_registers.read8<Registers::A>();

        m_memory.write8(addr, a);

        return 2;
    }
    std::string Executor::ldh_aC_A_dis(uint8_t opCode)
    {
        return "LDH (C), A;\n";
    }

    int Executor::ldh_A_an()
    {
        uint8_t n = m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();
        uint16_t addr = utils::to16(0xFF, n);

        uint8_t val = m_memory.read8(addr);
        m_registers.write8<Registers::A>(val);

        return 3;
    }
    std::string Executor::ldh_A_an_dis(uint8_t opCode)
    {
        return "LDH A, (n);\n";
    }

    int Executor::ldh_an_A()
    {
        uint8_t n = m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();
        uint16_t addr = utils::to16(0xFF, n);

        uint8_t a = m_registers.read8<Registers::A>();

        m_memory.write8(addr, a);
        return 3;
    }
    std::string Executor::ldh_an_A_dis(uint8_t opCode)
    {
        return "LDH (n), A;\n";
    }

    int Executor::ld_HLd_A()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);

        m_registers.write16<Registers::HL>(--hl);
        m_registers.write8<Registers::A>(val);

        return 2;

    }
    std::string Executor::ld_HLd_A_dis(uint8_t opCode)
    {
        return "LD (HL-), A;\n";
    }
        
    int Executor::ld_A_HLd()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t a = m_registers.read8<Registers::A>();
        m_memory.write8(hl, a);

        m_registers.write16<Registers::HL>(--hl);

        return 2;
    }
    std::string Executor::ld_A_HLd_dis(uint8_t opCode)
    {
        return "LD A, (HL-);\n";
    }
        
    int Executor::ld_HLi_A()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);

        m_registers.write16<Registers::HL>(++hl);
        m_registers.write8<Registers::A>(val);

        return 2;
    }
    std::string Executor::ld_HLi_A_dis(uint8_t opCode)
    {
        return "LD (HL+), A;\n";
    }
    
    int Executor::ld_A_HLi()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t a = m_registers.read8<Registers::A>();
        m_memory.write8(hl, a);

        m_registers.write16<Registers::HL>(++hl);

        return 2;
    }
    std::string Executor::ld_A_HLi_dis(uint8_t opCode)
    {
        return "LD A, (HL+);\n";
    }

    int Executor::ld_SP_rr()
    {
        uint16_t val = m_memory.read16(m_registers.getPC());
        m_registers.incrementPC();
        m_registers.incrementPC();

        m_registers.setSP(val);

        return 3;
    }
    std::string Executor::ld_SP_rr_dis(uint8_t opCode)
    {
        return "LD rr, SP;\n";
    }

    int Executor::ld_nn_SP()
    {
        uint16_t addr = m_memory.read16(m_registers.getPC());
        m_registers.incrementPC();
        m_registers.incrementPC();

        m_memory.write16(addr, m_registers.getSP());

        return 5;
    }
    std::string Executor::ld_nn_SP_dis(uint8_t opCode)
    {
        return "LD SP, rr;\n";
    }

    int Executor::ld_SP_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        m_registers.setSP(hl);

        return 2;
    }
    std::string Executor::ld_SP_HL_dis(uint8_t opCode)
    {
        return "LD SP, HL;\n";
    }

    int Executor::ld_HL_SP_r8()
    {
        int8_t val = (int8_t) m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();
        
        uint16_t result = m_registers.getSP() + val;

        m_registers.write16<Registers::HL>((uint16_t) result);

        m_registers.resetFlags();

        bool hc = ((val ^ m_registers.getSP() ^ result) & 0x10) == 0x10;
        bool c = ((val ^ m_registers.getSP() ^ result) & 0x100) == 0x100;
        m_registers.setFlag(Registers::Flag::H, hc);
        m_registers.setFlag(Registers::Flag::C, c);

        return 3;
    }
    std::string Executor::ld_HL_SP_r8_dis(uint8_t opCode)
    {
        return "LD HL, SP + r8;\n";
    }

    void Executor::add(int a, int b)
    {
        int res = a + b;
        int carryBits = a ^ b ^ res;

        m_registers.write8<Registers::A>(res);
        updateFlagsAdd(carryBits, res);
    }

    int Executor::add_HL()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();
        
        uint16_t hl = m_registers.read16<Registers::HL>();
        int b = (int8_t) m_memory.read8(hl);
        add(a, b);

        return 2;
    }
    std::string Executor::add_HL_dis(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADD (HL);\n";
    }

    int Executor::add_n()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();

        int b = (int8_t) m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();
        add(a, b);

        return 2;
    }
    std::string Executor::add_n_dis(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADD n;\n";
    }
    
    void cpu::Executor::adc(int a, int b)
    {
        int carryFlag = m_registers.isSetFlag(Registers::Flag::C) ? 1 : 0;
        int res = a + b + carryFlag;
        int carryBits = a ^ b ^ carryFlag ^ res;

        m_registers.write8<Registers::A>(res);
        updateFlagsAdd(carryBits, res);
    }

    int Executor::adc_HL()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        uint16_t hl = m_registers.read16<Registers::HL>();
        int b = (int8_t)m_memory.read8(hl);
        adc(a, b);

        return 2;
    }
    std::string Executor::adc_HL_dis(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADC (HL);\n";
    }

    int Executor::adc_n()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        int b = (int8_t)m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();
        adc(a, b);

        return 2;
    }
    std::string Executor::adc_n_dis(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADC n;\n";
    }
}