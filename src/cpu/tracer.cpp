#include "tracer.h"

#include "registery.h"
#include "memory.h"

#include <utility>
#include <limits>

namespace cpu
{
    Tracer::Tracer(Registers& regist, Memory& /*mem*/) : 
        m_registers(regist)//, m_memory(mem)
    {
        fillInstructionSet();
        fillCbInstructionSet();
    }

    void Tracer::fillInstructionSet()
    {
        std::fill_n(m_instructionSet, 255, &Tracer::unhandled);

        m_instructionSet[0x01] = &Tracer::ld_rr_nn<Registers::BC>;
        m_instructionSet[0x02] = &Tracer::ld_r16_A<Registers::BC>;
        m_instructionSet[0x06] = &Tracer::ld_r_n_8<0x06>;
        m_instructionSet[0x08] = &Tracer::ld_nn_SP;
        m_instructionSet[0x0A] = &Tracer::ld_A_r16<Registers::BC>;
        m_instructionSet[0x0E] = &Tracer::ld_r_n_8<0x0E>;
        m_instructionSet[0x11] = &Tracer::ld_rr_nn<Registers::DE>;
        m_instructionSet[0x12] = &Tracer::ld_r16_A<Registers::DE>;
        m_instructionSet[0x16] = &Tracer::ld_r_n_8<0x16>;
        m_instructionSet[0x1A] = &Tracer::ld_A_r16<Registers::DE>;
        m_instructionSet[0x1E] = &Tracer::ld_r_n_8<0x1E>;
        m_instructionSet[0x21] = &Tracer::ld_rr_nn<Registers::HL>;
        m_instructionSet[0x22] = &Tracer::ld_HLi_A;
        m_instructionSet[0x26] = &Tracer::ld_r_n_8<0x26>;
        m_instructionSet[0x2A] = &Tracer::ld_A_HLi;
        m_instructionSet[0x2E] = &Tracer::ld_r_n_8<0x2E>;
        m_instructionSet[0x31] = &Tracer::ld_SP_rr;
        m_instructionSet[0x32] = &Tracer::ld_HLd_A;
        m_instructionSet[0x36] = &Tracer::ld_HL_n_8<0x36>;
        m_instructionSet[0x3A] = &Tracer::ld_A_HLd;
        m_instructionSet[0x3E] = &Tracer::ld_r_n_8<0x3E>;

        m_instructionSet[0x40] = &Tracer::ld_r_r_8<0x40>;
        m_instructionSet[0x41] = &Tracer::ld_r_r_8<0x41>;
        m_instructionSet[0x42] = &Tracer::ld_r_r_8<0x42>;
        m_instructionSet[0x43] = &Tracer::ld_r_r_8<0x43>;
        m_instructionSet[0x44] = &Tracer::ld_r_r_8<0x44>;
        m_instructionSet[0x45] = &Tracer::ld_r_r_8<0x45>;
        m_instructionSet[0x46] = &Tracer::ld_r_HL<0x46>;
        m_instructionSet[0x47] = &Tracer::ld_r_r_8<0x47>;
        m_instructionSet[0x48] = &Tracer::ld_r_r_8<0x48>;
        m_instructionSet[0x49] = &Tracer::ld_r_r_8<0x49>;
        m_instructionSet[0x4A] = &Tracer::ld_r_r_8<0x4A>;
        m_instructionSet[0x4B] = &Tracer::ld_r_r_8<0x4B>;
        m_instructionSet[0x4C] = &Tracer::ld_r_r_8<0x4C>;
        m_instructionSet[0x4D] = &Tracer::ld_r_r_8<0x4D>;
        m_instructionSet[0x4E] = &Tracer::ld_r_HL<0x4E>;
        m_instructionSet[0x4F] = &Tracer::ld_r_r_8<0x4F>;
        m_instructionSet[0x50] = &Tracer::ld_r_r_8<0x50>;
        m_instructionSet[0x51] = &Tracer::ld_r_r_8<0x51>;
        m_instructionSet[0x52] = &Tracer::ld_r_r_8<0x52>;
        m_instructionSet[0x53] = &Tracer::ld_r_r_8<0x53>;
        m_instructionSet[0x54] = &Tracer::ld_r_r_8<0x54>;
        m_instructionSet[0x55] = &Tracer::ld_r_r_8<0x55>;
        m_instructionSet[0x56] = &Tracer::ld_r_HL<0x56>;
        m_instructionSet[0x57] = &Tracer::ld_r_r_8<0x57>;
        m_instructionSet[0x58] = &Tracer::ld_r_r_8<0x58>;
        m_instructionSet[0x59] = &Tracer::ld_r_r_8<0x59>;
        m_instructionSet[0x5A] = &Tracer::ld_r_r_8<0x5A>;
        m_instructionSet[0x5B] = &Tracer::ld_r_r_8<0x5B>;
        m_instructionSet[0x5C] = &Tracer::ld_r_r_8<0x5C>;
        m_instructionSet[0x5D] = &Tracer::ld_r_r_8<0x5D>;
        m_instructionSet[0x5E] = &Tracer::ld_r_HL<0x5E>;
        m_instructionSet[0x5F] = &Tracer::ld_r_r_8<0x5F>;
        m_instructionSet[0x60] = &Tracer::ld_r_r_8<0x60>;
        m_instructionSet[0x61] = &Tracer::ld_r_r_8<0x61>;
        m_instructionSet[0x62] = &Tracer::ld_r_r_8<0x62>;
        m_instructionSet[0x63] = &Tracer::ld_r_r_8<0x63>;
        m_instructionSet[0x64] = &Tracer::ld_r_r_8<0x64>;
        m_instructionSet[0x65] = &Tracer::ld_r_r_8<0x65>;
        m_instructionSet[0x66] = &Tracer::ld_r_HL<0x66>;
        m_instructionSet[0x67] = &Tracer::ld_r_r_8<0x67>;
        m_instructionSet[0x68] = &Tracer::ld_r_r_8<0x68>;
        m_instructionSet[0x69] = &Tracer::ld_r_r_8<0x69>;
        m_instructionSet[0x6A] = &Tracer::ld_r_r_8<0x6A>;
        m_instructionSet[0x6B] = &Tracer::ld_r_r_8<0x6B>;
        m_instructionSet[0x6C] = &Tracer::ld_r_r_8<0x6C>;
        m_instructionSet[0x6D] = &Tracer::ld_r_r_8<0x6D>;
        m_instructionSet[0x6E] = &Tracer::ld_r_HL<0x6E>;
        m_instructionSet[0x6F] = &Tracer::ld_r_r_8<0x6F>;
        m_instructionSet[0x70] = &Tracer::ld_HL_r<0x70>;
        m_instructionSet[0x71] = &Tracer::ld_HL_r<0x71>;
        m_instructionSet[0x72] = &Tracer::ld_HL_r<0x72>;
        m_instructionSet[0x73] = &Tracer::ld_HL_r<0x73>;
        m_instructionSet[0x74] = &Tracer::ld_HL_r<0x74>;
        m_instructionSet[0x75] = &Tracer::ld_HL_r<0x75>;

        m_instructionSet[0x77] = &Tracer::ld_HL_r<0x77>;
        m_instructionSet[0x78] = &Tracer::ld_r_r_8<0x78>;
        m_instructionSet[0x79] = &Tracer::ld_r_r_8<0x79>;
        m_instructionSet[0x7A] = &Tracer::ld_r_r_8<0x7A>;
        m_instructionSet[0x7B] = &Tracer::ld_r_r_8<0x7B>;
        m_instructionSet[0x7C] = &Tracer::ld_r_r_8<0x7C>;
        m_instructionSet[0x7D] = &Tracer::ld_r_r_8<0x7D>;
        m_instructionSet[0x7E] = &Tracer::ld_r_HL<0x7E>;
        m_instructionSet[0x7F] = &Tracer::ld_r_r_8<0x7F>;
        m_instructionSet[0x80] = &Tracer::add_r<Registers::B>;
        m_instructionSet[0x81] = &Tracer::add_r<Registers::C>;
        m_instructionSet[0x82] = &Tracer::add_r<Registers::D>;
        m_instructionSet[0x83] = &Tracer::add_r<Registers::E>;
        m_instructionSet[0x84] = &Tracer::add_r<Registers::H>;
        m_instructionSet[0x85] = &Tracer::add_r<Registers::L>;
        m_instructionSet[0x86] = &Tracer::add_HL;
        m_instructionSet[0x87] = &Tracer::add_r<Registers::A>;
        m_instructionSet[0x88] = &Tracer::adc_r<Registers::B>;
        m_instructionSet[0x89] = &Tracer::adc_r<Registers::C>;
        m_instructionSet[0x8A] = &Tracer::adc_r<Registers::D>;
        m_instructionSet[0x8B] = &Tracer::adc_r<Registers::E>;
        m_instructionSet[0x8C] = &Tracer::adc_r<Registers::H>;
        m_instructionSet[0x8D] = &Tracer::adc_r<Registers::L>;
        m_instructionSet[0x8E] = &Tracer::adc_HL;
        m_instructionSet[0x8F] = &Tracer::adc_r<Registers::A>;

        m_instructionSet[0xC1] = &Tracer::pop<Registers::BC>;
        m_instructionSet[0xC5] = &Tracer::push<Registers::BC>;
        m_instructionSet[0xC6] = &Tracer::add_n;
        m_instructionSet[0xCE] = &Tracer::adc_n;
        m_instructionSet[0xD1] = &Tracer::pop<Registers::DE>;
        m_instructionSet[0xD5] = &Tracer::push<Registers::DE>;
        m_instructionSet[0xE0] = &Tracer::ldh_an_A;
        m_instructionSet[0xE1] = &Tracer::pop<Registers::HL>;
        m_instructionSet[0xE2] = &Tracer::ldh_aC_A;
        m_instructionSet[0xE5] = &Tracer::push<Registers::HL>;
        m_instructionSet[0xEA] = &Tracer::ld_nn_A;
        m_instructionSet[0xF0] = &Tracer::ldh_A_an;
        m_instructionSet[0xF1] = &Tracer::pop<Registers::AF>;
        m_instructionSet[0xF2] = &Tracer::ldh_A_aC;
        m_instructionSet[0xF5] = &Tracer::push<Registers::AF>;
        m_instructionSet[0xF8] = &Tracer::ld_HL_SP_r8;
        m_instructionSet[0xF9] = &Tracer::ld_SP_HL;
        m_instructionSet[0xFA] = &Tracer::ld_A_nn;
    }

    void Tracer::fillCbInstructionSet()
    {
        std::fill_n(m_cbInstructionSet, 255, &Tracer::unhandled);
    }

    std::string Tracer::unhandled(uint8_t opCode)
    {
        return std::to_string(opCode) + " : Not handled yet;\n";
    }

    std::string Tracer::ld_A_nn(uint8_t /*opCode*/)
    {
        return "LD A, nn;\n";
    }

    std::string Tracer::ld_nn_A(uint8_t opCode)
    {
        return "LD nn, A;";
    }

    std::string Tracer::ldh_A_aC(uint8_t opCode)
    {
        return "LDH A, (C);\n";
    }

    std::string Tracer::ldh_aC_A(uint8_t opCode)
    {
        return "LDH (C), A;\n";
    }

    std::string Tracer::ldh_A_an(uint8_t opCode)
    {
        return "LDH A, (n);\n";
    }

    std::string Tracer::ldh_an_A(uint8_t opCode)
    {
        return "LDH (n), A;\n";
    }

    std::string Tracer::ld_HLd_A(uint8_t opCode)
    {
        return "LD (HL-), A;\n";
    }

    std::string Tracer::ld_A_HLd(uint8_t opCode)
    {
        return "LD A, (HL-);\n";
    }

    std::string Tracer::ld_HLi_A(uint8_t opCode)
    {
        return "LD (HL+), A;\n";
    }

    std::string Tracer::ld_A_HLi(uint8_t opCode)
    {
        return "LD A, (HL+);\n";
    }

    std::string Tracer::ld_SP_rr(uint8_t opCode)
    {
        return "LD rr, SP;\n";
    }

    std::string Tracer::ld_nn_SP(uint8_t opCode)
    {
        return "LD SP, rr;\n";
    }

    std::string Tracer::ld_SP_HL(uint8_t opCode)
    {
        return "LD SP, HL;\n";
    }

    std::string Tracer::ld_HL_SP_r8(uint8_t opCode)
    {
        return "LD HL, SP + r8;\n";
    }

    std::string Tracer::add_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADD (HL);\n";
    }

    std::string Tracer::add_n(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADD n;\n";
    }

    std::string Tracer::adc_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADC (HL);\n";
    }

    std::string Tracer::adc_n(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADC n;\n";
    }
}