#include "logger.h"

#include "registery.h"
#include "memory.h"

#include <utility>
#include <limits>

namespace cpu
{
    Logger::Logger(Registers& regist, Memory& /*mem*/) :
        m_registers(regist)//, m_memory(mem)
    {
        fillInstructionSet();
        fillCbInstructionSet();
    }

    void Logger::fillInstructionSet()
    {
        std::fill_n(m_instructionSet, 255, &Logger::unhandled);

        m_instructionSet[0x01] = &Logger::ld_rr_nn<Registers::BC>;
        m_instructionSet[0x02] = &Logger::ld_r16_A<Registers::BC>;
        m_instructionSet[0x03] = &Logger::inc_rr<Registers::BC>;
        m_instructionSet[0x04] = &Logger::inc_r<Registers::B>;
        m_instructionSet[0x05] = &Logger::dec_r<Registers::B>;
        m_instructionSet[0x06] = &Logger::ld_r_n_8<0x06>;
        m_instructionSet[0x07] = &Logger::rlca;
        m_instructionSet[0x08] = &Logger::ld_nn_SP;
        m_instructionSet[0x09] = &Logger::add_HL_rr<Registers::BC>;
        m_instructionSet[0x0A] = &Logger::ld_A_r16<Registers::BC>;
        m_instructionSet[0x0B] = &Logger::dec_rr<Registers::BC>;
        m_instructionSet[0x0C] = &Logger::inc_r<Registers::C>;
        m_instructionSet[0x0D] = &Logger::dec_r<Registers::C>;
        m_instructionSet[0x0E] = &Logger::ld_r_n_8<0x0E>;
        m_instructionSet[0x11] = &Logger::ld_rr_nn<Registers::DE>;
        m_instructionSet[0x12] = &Logger::ld_r16_A<Registers::DE>;
        m_instructionSet[0x13] = &Logger::inc_rr<Registers::DE>;
        m_instructionSet[0x14] = &Logger::inc_r<Registers::D>;
        m_instructionSet[0x15] = &Logger::dec_r<Registers::D>;
        m_instructionSet[0x16] = &Logger::ld_r_n_8<0x16>;
        m_instructionSet[0x17] = &Logger::rla;
        m_instructionSet[0x19] = &Logger::add_HL_rr<Registers::DE>;
        m_instructionSet[0x1A] = &Logger::ld_A_r16<Registers::DE>;
        m_instructionSet[0x1B] = &Logger::dec_rr<Registers::DE>;
        m_instructionSet[0x1C] = &Logger::inc_r<Registers::E>;
        m_instructionSet[0x1D] = &Logger::dec_r<Registers::E>;
        m_instructionSet[0x1E] = &Logger::ld_r_n_8<0x1E>;
        m_instructionSet[0x21] = &Logger::ld_rr_nn<Registers::HL>;
        m_instructionSet[0x22] = &Logger::ld_HLi_A;
        m_instructionSet[0x23] = &Logger::inc_rr<Registers::HL>;
        m_instructionSet[0x24] = &Logger::inc_r<Registers::H>;
        m_instructionSet[0x25] = &Logger::dec_r<Registers::H>;
        m_instructionSet[0x26] = &Logger::ld_r_n_8<0x26>;
        m_instructionSet[0x27] = &Logger::daa;
        m_instructionSet[0x29] = &Logger::add_HL_rr<Registers::HL>;
        m_instructionSet[0x2A] = &Logger::ld_A_HLi;
        m_instructionSet[0x2B] = &Logger::inc_rr<Registers::HL>;
        m_instructionSet[0x2C] = &Logger::inc_r<Registers::L>;
        m_instructionSet[0x2D] = &Logger::dec_r<Registers::L>;
        m_instructionSet[0x2E] = &Logger::ld_r_n_8<0x2E>;
        m_instructionSet[0x2F] = &Logger::cpl;
        m_instructionSet[0x31] = &Logger::ld_SP_rr;
        m_instructionSet[0x32] = &Logger::ld_HLd_A;
        m_instructionSet[0x33] = &Logger::inc_SP;
        m_instructionSet[0x34] = &Logger::inc_HL;
        m_instructionSet[0x35] = &Logger::dec_HL;
        m_instructionSet[0x36] = &Logger::ld_HL_n_8<0x36>;
        m_instructionSet[0x37] = &Logger::scf;
        m_instructionSet[0x39] = &Logger::add_HL_SP;
        m_instructionSet[0x3A] = &Logger::ld_A_HLd;
        m_instructionSet[0x3B] = &Logger::dec_SP;
        m_instructionSet[0x3C] = &Logger::inc_r<Registers::A>;
        m_instructionSet[0x3D] = &Logger::dec_r<Registers::A>;
        m_instructionSet[0x3E] = &Logger::ld_r_n_8<0x3E>;
        m_instructionSet[0x3F] = &Logger::ccf;
        m_instructionSet[0x40] = &Logger::ld_r_r_8<0x40>;
        m_instructionSet[0x41] = &Logger::ld_r_r_8<0x41>;
        m_instructionSet[0x42] = &Logger::ld_r_r_8<0x42>;
        m_instructionSet[0x43] = &Logger::ld_r_r_8<0x43>;
        m_instructionSet[0x44] = &Logger::ld_r_r_8<0x44>;
        m_instructionSet[0x45] = &Logger::ld_r_r_8<0x45>;
        m_instructionSet[0x46] = &Logger::ld_r_HL<0x46>;
        m_instructionSet[0x47] = &Logger::ld_r_r_8<0x47>;
        m_instructionSet[0x48] = &Logger::ld_r_r_8<0x48>;
        m_instructionSet[0x49] = &Logger::ld_r_r_8<0x49>;
        m_instructionSet[0x4A] = &Logger::ld_r_r_8<0x4A>;
        m_instructionSet[0x4B] = &Logger::ld_r_r_8<0x4B>;
        m_instructionSet[0x4C] = &Logger::ld_r_r_8<0x4C>;
        m_instructionSet[0x4D] = &Logger::ld_r_r_8<0x4D>;
        m_instructionSet[0x4E] = &Logger::ld_r_HL<0x4E>;
        m_instructionSet[0x4F] = &Logger::ld_r_r_8<0x4F>;
        m_instructionSet[0x50] = &Logger::ld_r_r_8<0x50>;
        m_instructionSet[0x51] = &Logger::ld_r_r_8<0x51>;
        m_instructionSet[0x52] = &Logger::ld_r_r_8<0x52>;
        m_instructionSet[0x53] = &Logger::ld_r_r_8<0x53>;
        m_instructionSet[0x54] = &Logger::ld_r_r_8<0x54>;
        m_instructionSet[0x55] = &Logger::ld_r_r_8<0x55>;
        m_instructionSet[0x56] = &Logger::ld_r_HL<0x56>;
        m_instructionSet[0x57] = &Logger::ld_r_r_8<0x57>;
        m_instructionSet[0x58] = &Logger::ld_r_r_8<0x58>;
        m_instructionSet[0x59] = &Logger::ld_r_r_8<0x59>;
        m_instructionSet[0x5A] = &Logger::ld_r_r_8<0x5A>;
        m_instructionSet[0x5B] = &Logger::ld_r_r_8<0x5B>;
        m_instructionSet[0x5C] = &Logger::ld_r_r_8<0x5C>;
        m_instructionSet[0x5D] = &Logger::ld_r_r_8<0x5D>;
        m_instructionSet[0x5E] = &Logger::ld_r_HL<0x5E>;
        m_instructionSet[0x5F] = &Logger::ld_r_r_8<0x5F>;
        m_instructionSet[0x60] = &Logger::ld_r_r_8<0x60>;
        m_instructionSet[0x61] = &Logger::ld_r_r_8<0x61>;
        m_instructionSet[0x62] = &Logger::ld_r_r_8<0x62>;
        m_instructionSet[0x63] = &Logger::ld_r_r_8<0x63>;
        m_instructionSet[0x64] = &Logger::ld_r_r_8<0x64>;
        m_instructionSet[0x65] = &Logger::ld_r_r_8<0x65>;
        m_instructionSet[0x66] = &Logger::ld_r_HL<0x66>;
        m_instructionSet[0x67] = &Logger::ld_r_r_8<0x67>;
        m_instructionSet[0x68] = &Logger::ld_r_r_8<0x68>;
        m_instructionSet[0x69] = &Logger::ld_r_r_8<0x69>;
        m_instructionSet[0x6A] = &Logger::ld_r_r_8<0x6A>;
        m_instructionSet[0x6B] = &Logger::ld_r_r_8<0x6B>;
        m_instructionSet[0x6C] = &Logger::ld_r_r_8<0x6C>;
        m_instructionSet[0x6D] = &Logger::ld_r_r_8<0x6D>;
        m_instructionSet[0x6E] = &Logger::ld_r_HL<0x6E>;
        m_instructionSet[0x6F] = &Logger::ld_r_r_8<0x6F>;
        m_instructionSet[0x70] = &Logger::ld_HL_r<0x70>;
        m_instructionSet[0x71] = &Logger::ld_HL_r<0x71>;
        m_instructionSet[0x72] = &Logger::ld_HL_r<0x72>;
        m_instructionSet[0x73] = &Logger::ld_HL_r<0x73>;
        m_instructionSet[0x74] = &Logger::ld_HL_r<0x74>;
        m_instructionSet[0x75] = &Logger::ld_HL_r<0x75>;

        m_instructionSet[0x77] = &Logger::ld_HL_r<0x77>;
        m_instructionSet[0x78] = &Logger::ld_r_r_8<0x78>;
        m_instructionSet[0x79] = &Logger::ld_r_r_8<0x79>;
        m_instructionSet[0x7A] = &Logger::ld_r_r_8<0x7A>;
        m_instructionSet[0x7B] = &Logger::ld_r_r_8<0x7B>;
        m_instructionSet[0x7C] = &Logger::ld_r_r_8<0x7C>;
        m_instructionSet[0x7D] = &Logger::ld_r_r_8<0x7D>;
        m_instructionSet[0x7E] = &Logger::ld_r_HL<0x7E>;
        m_instructionSet[0x7F] = &Logger::ld_r_r_8<0x7F>;
        m_instructionSet[0x80] = &Logger::add_r<Registers::B>;
        m_instructionSet[0x81] = &Logger::add_r<Registers::C>;
        m_instructionSet[0x82] = &Logger::add_r<Registers::D>;
        m_instructionSet[0x83] = &Logger::add_r<Registers::E>;
        m_instructionSet[0x84] = &Logger::add_r<Registers::H>;
        m_instructionSet[0x85] = &Logger::add_r<Registers::L>;
        m_instructionSet[0x86] = &Logger::add_HL;
        m_instructionSet[0x87] = &Logger::add_r<Registers::A>;
        m_instructionSet[0x88] = &Logger::adc_r<Registers::B>;
        m_instructionSet[0x89] = &Logger::adc_r<Registers::C>;
        m_instructionSet[0x8A] = &Logger::adc_r<Registers::D>;
        m_instructionSet[0x8B] = &Logger::adc_r<Registers::E>;
        m_instructionSet[0x8C] = &Logger::adc_r<Registers::H>;
        m_instructionSet[0x8D] = &Logger::adc_r<Registers::L>;
        m_instructionSet[0x8E] = &Logger::adc_HL;
        m_instructionSet[0x8F] = &Logger::adc_r<Registers::A>;
        m_instructionSet[0x90] = &Logger::sub_r<Registers::B>;
        m_instructionSet[0x91] = &Logger::sub_r<Registers::C>;
        m_instructionSet[0x92] = &Logger::sub_r<Registers::D>;
        m_instructionSet[0x93] = &Logger::sub_r<Registers::E>;
        m_instructionSet[0x94] = &Logger::sub_r<Registers::H>;
        m_instructionSet[0x95] = &Logger::sub_r<Registers::L>;
        m_instructionSet[0x96] = &Logger::sub_HL;
        m_instructionSet[0x97] = &Logger::sub_r<Registers::A>;
        m_instructionSet[0x98] = &Logger::sbc_r<Registers::B>;
        m_instructionSet[0x99] = &Logger::sbc_r<Registers::C>;
        m_instructionSet[0x9A] = &Logger::sbc_r<Registers::D>;
        m_instructionSet[0x9B] = &Logger::sbc_r<Registers::E>;
        m_instructionSet[0x9C] = &Logger::sbc_r<Registers::H>;
        m_instructionSet[0x9D] = &Logger::sbc_r<Registers::L>;
        m_instructionSet[0x9E] = &Logger::sbc_HL;
        m_instructionSet[0x9F] = &Logger::sbc_r<Registers::A>;
        m_instructionSet[0xA0] = &Logger::and_r<Registers::B>;
        m_instructionSet[0xA1] = &Logger::and_r<Registers::C>;
        m_instructionSet[0xA2] = &Logger::and_r<Registers::D>;
        m_instructionSet[0xA3] = &Logger::and_r<Registers::E>;
        m_instructionSet[0xA4] = &Logger::and_r<Registers::H>;
        m_instructionSet[0xA5] = &Logger::and_r<Registers::L>;
        m_instructionSet[0xA6] = &Logger::and_HL;
        m_instructionSet[0xA7] = &Logger::and_r<Registers::A>;
        m_instructionSet[0xA7] = &Logger::and_r<Registers::A>;
        m_instructionSet[0xA8] = &Logger::xor_r<Registers::B>;
        m_instructionSet[0xA9] = &Logger::xor_r<Registers::C>;
        m_instructionSet[0xAA] = &Logger::xor_r<Registers::D>;
        m_instructionSet[0xAB] = &Logger::xor_r<Registers::E>;
        m_instructionSet[0xAC] = &Logger::xor_r<Registers::H>;
        m_instructionSet[0xAD] = &Logger::xor_r<Registers::L>;
        m_instructionSet[0xAE] = &Logger::xor_HL;
        m_instructionSet[0xAF] = &Logger::xor_r<Registers::A>;
        m_instructionSet[0xB0] = &Logger::or_r<Registers::B>;
        m_instructionSet[0xB1] = &Logger::or_r<Registers::C>;
        m_instructionSet[0xB2] = &Logger::or_r<Registers::D>;
        m_instructionSet[0xB3] = &Logger::or_r<Registers::E>;
        m_instructionSet[0xB4] = &Logger::or_r<Registers::H>;
        m_instructionSet[0xB5] = &Logger::or_r<Registers::L>;
        m_instructionSet[0xB6] = &Logger::or_HL;
        m_instructionSet[0xB7] = &Logger::or_r<Registers::A>;
        m_instructionSet[0xB8] = &Logger::cp_r<Registers::B>;
        m_instructionSet[0xB9] = &Logger::cp_r<Registers::C>;
        m_instructionSet[0xBA] = &Logger::cp_r<Registers::D>;
        m_instructionSet[0xBB] = &Logger::cp_r<Registers::E>;
        m_instructionSet[0xBC] = &Logger::cp_r<Registers::H>;
        m_instructionSet[0xBD] = &Logger::cp_r<Registers::L>;
        m_instructionSet[0xBE] = &Logger::cp_HL;
        m_instructionSet[0xBF] = &Logger::cp_r<Registers::A>;
        m_instructionSet[0xC1] = &Logger::pop<Registers::BC>;
        m_instructionSet[0xC5] = &Logger::push<Registers::BC>;
        m_instructionSet[0xC6] = &Logger::add_n;
        m_instructionSet[0xCE] = &Logger::adc_n;
        m_instructionSet[0xD1] = &Logger::pop<Registers::DE>;
        m_instructionSet[0xD5] = &Logger::push<Registers::DE>;
        m_instructionSet[0xD6] = &Logger::sub_n;
        m_instructionSet[0xDE] = &Logger::sbc_n;
        m_instructionSet[0xE0] = &Logger::ldh_an_A;
        m_instructionSet[0xE1] = &Logger::pop<Registers::HL>;
        m_instructionSet[0xE2] = &Logger::ldh_aC_A;
        m_instructionSet[0xE5] = &Logger::push<Registers::HL>;
        m_instructionSet[0xE6] = &Logger::and_n;
        m_instructionSet[0xE8] = &Logger::add_SP_n;
        m_instructionSet[0xEA] = &Logger::ld_nn_A;
        m_instructionSet[0xEE] = &Logger::xor_n;
        m_instructionSet[0xF0] = &Logger::ldh_A_an;
        m_instructionSet[0xF1] = &Logger::pop<Registers::AF>;
        m_instructionSet[0xF2] = &Logger::ldh_A_aC;
        m_instructionSet[0xF5] = &Logger::push<Registers::AF>;
        m_instructionSet[0xF6] = &Logger::or_n;
        m_instructionSet[0xF8] = &Logger::ld_HL_SP_r8;
        m_instructionSet[0xF9] = &Logger::ld_SP_HL;
        m_instructionSet[0xFA] = &Logger::ld_A_nn;
        m_instructionSet[0xFE] = &Logger::cp_n;
    }

    void Logger::fillCbInstructionSet()
    {
        std::fill_n(m_cbInstructionSet, 255, &Logger::unhandled);
    }

    std::string Logger::unhandled(uint8_t opCode)
    {
        return std::to_string(opCode) + " : Not handled yet;\n";
    }

    std::string Logger::ld_A_nn(uint8_t /*opCode*/)
    {
        return "LD A, nn;\n";
    }

    std::string Logger::ld_nn_A(uint8_t opCode)
    {
        return "LD nn, A;";
    }

    std::string Logger::ldh_A_aC(uint8_t opCode)
    {
        return "LDH A, (C);\n";
    }

    std::string Logger::ldh_aC_A(uint8_t opCode)
    {
        return "LDH (C), A;\n";
    }

    std::string Logger::ldh_A_an(uint8_t opCode)
    {
        return "LDH A, (n);\n";
    }

    std::string Logger::ldh_an_A(uint8_t opCode)
    {
        return "LDH (n), A;\n";
    }

    std::string Logger::ld_HLd_A(uint8_t opCode)
    {
        return "LD (HL-), A;\n";
    }

    std::string Logger::ld_A_HLd(uint8_t opCode)
    {
        return "LD A, (HL-);\n";
    }

    std::string Logger::ld_HLi_A(uint8_t opCode)
    {
        return "LD (HL+), A;\n";
    }

    std::string Logger::ld_A_HLi(uint8_t opCode)
    {
        return "LD A, (HL+);\n";
    }

    std::string Logger::ld_SP_rr(uint8_t opCode)
    {
        return "LD rr, SP;\n";
    }

    std::string Logger::ld_nn_SP(uint8_t opCode)
    {
        return "LD SP, rr;\n";
    }

    std::string Logger::ld_SP_HL(uint8_t opCode)
    {
        return "LD SP, HL;\n";
    }

    std::string Logger::ld_HL_SP_r8(uint8_t opCode)
    {
        return "LD HL, SP + r8;\n";
    }

    std::string Logger::add_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADD (HL);\n";
    }

    std::string Logger::add_n(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADD n;\n";
    }

    std::string Logger::adc_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADC (HL);\n";
    }

    std::string Logger::adc_n(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADC n;\n";
    }

    std::string Logger::sub_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : SUB (HL);\n";
    }

    std::string Logger::sub_n(uint8_t opCode)
    {
        return std::to_string(opCode) + " : SUB n;\n";
    }

    std::string Logger::sbc_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : SBC (HL);\n";
    }

    std::string Logger::sbc_n(uint8_t opCode)
    {
        return std::to_string(opCode) + " : SBC n;\n";
    }

    std::string Logger::cp_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : CP (HL);\n";
    }

    std::string Logger::cp_n(uint8_t opCode)
    {
        return std::to_string(opCode) + " : CP n;\n";
    }

    std::string Logger::inc_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : DEC (HL);\n";
    }

    std::string Logger::dec_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : INC (HL);\n";
    }

    std::string Logger::and_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : AND (HL);\n";
    }

    std::string Logger::and_n(uint8_t opCode)
    {
        return std::to_string(opCode) + " : AND n;\n";
    }

    std::string Logger::or_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : OR (HL);\n";
    }

    std::string Logger::or_n(uint8_t opCode)
    {
        return std::to_string(opCode) + " : OR n;\n";
    }

    std::string Logger::xor_HL(uint8_t opCode)
    {
        return std::to_string(opCode) + " : XOR (HL);\n";
    }

    std::string Logger::xor_n(uint8_t opCode)
    {
        return std::to_string(opCode) + " : XOR n;\n";
    }

    std::string Logger::scf(uint8_t opCode)
    {
        return std::to_string(opCode) + " : CCF;\n";
    }

    std::string Logger::ccf(uint8_t opCode)
    {
        return std::to_string(opCode) + " : SCF;\n";
    }

    std::string Logger::cpl(uint8_t opCode)
    {
        return std::to_string(opCode) + " : CPL;\n";
    }

    std::string Logger::daa(uint8_t opCode)
    {
        return std::to_string(opCode) + " : DAA;\n";
    }

    std::string Logger::add_HL_SP(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADD HL SP;\n";
    }

    std::string Logger::add_SP_n(uint8_t opCode)
    {
        return std::to_string(opCode) + " : ADD SP n;\n";
    }

    std::string Logger::inc_SP(uint8_t opCode)
    {
        return std::to_string(opCode) + " : INC SP;\n";
    }

    std::string Logger::dec_SP(uint8_t opCode)
    {
        return std::to_string(opCode) + " : DEC SP;\n";
    }

    std::string Logger::rlca(uint8_t opCode)
    {
        return std::to_string(opCode) + " : RLCA;\n";
    }

    std::string Logger::rla(uint8_t opCode)
    {
        return std::to_string(opCode) + " : RLA;\n";
    }
}