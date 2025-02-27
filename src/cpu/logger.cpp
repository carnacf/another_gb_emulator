#include "logger.h"

#include "registery.h"
#include "memory.h"

#include <utility>
#include <limits>

namespace cpu
{
    Logger::Logger(Registers& regist, Memory& mem) :
        m_registers(regist), m_memory(mem)
    {
        fillInstructionSet();
        fillCbInstructionSet();
    }

    void Logger::fillInstructionSet()
    {
        std::fill_n(m_instructionSet, 256, &Logger::unhandled);

        m_instructionSet[0x00] = &Logger::nop;
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
        m_instructionSet[0x0F] = &Logger::rrca;
        m_instructionSet[0x10] = &Logger::stop;
        m_instructionSet[0x11] = &Logger::ld_rr_nn<Registers::DE>;
        m_instructionSet[0x12] = &Logger::ld_r16_A<Registers::DE>;
        m_instructionSet[0x13] = &Logger::inc_rr<Registers::DE>;
        m_instructionSet[0x14] = &Logger::inc_r<Registers::D>;
        m_instructionSet[0x15] = &Logger::dec_r<Registers::D>;
        m_instructionSet[0x16] = &Logger::ld_r_n_8<0x16>;
        m_instructionSet[0x17] = &Logger::rla;
        m_instructionSet[0x18] = &Logger::jr_e;
        m_instructionSet[0x19] = &Logger::add_HL_rr<Registers::DE>;
        m_instructionSet[0x1A] = &Logger::ld_A_r16<Registers::DE>;
        m_instructionSet[0x1B] = &Logger::dec_rr<Registers::DE>;
        m_instructionSet[0x1C] = &Logger::inc_r<Registers::E>;
        m_instructionSet[0x1D] = &Logger::dec_r<Registers::E>;
        m_instructionSet[0x1E] = &Logger::ld_r_n_8<0x1E>;
        m_instructionSet[0x1F] = &Logger::rra;
        m_instructionSet[0x20] = &Logger::jr_nz_n;
        m_instructionSet[0x21] = &Logger::ld_rr_nn<Registers::HL>;
        m_instructionSet[0x22] = &Logger::ld_HLi_A;
        m_instructionSet[0x23] = &Logger::inc_rr<Registers::HL>;
        m_instructionSet[0x24] = &Logger::inc_r<Registers::H>;
        m_instructionSet[0x25] = &Logger::dec_r<Registers::H>;
        m_instructionSet[0x26] = &Logger::ld_r_n_8<0x26>;
        m_instructionSet[0x27] = &Logger::daa;
        m_instructionSet[0x28] = &Logger::jr_z_n;
        m_instructionSet[0x29] = &Logger::add_HL_rr<Registers::HL>;
        m_instructionSet[0x2A] = &Logger::ld_A_HLi;
        m_instructionSet[0x2B] = &Logger::inc_rr<Registers::HL>;
        m_instructionSet[0x2C] = &Logger::inc_r<Registers::L>;
        m_instructionSet[0x2D] = &Logger::dec_r<Registers::L>;
        m_instructionSet[0x2E] = &Logger::ld_r_n_8<0x2E>;
        m_instructionSet[0x2F] = &Logger::cpl;
        m_instructionSet[0x30] = &Logger::jr_nc_n;
        m_instructionSet[0x31] = &Logger::ld_SP_nn;
        m_instructionSet[0x32] = &Logger::ld_HLd_A;
        m_instructionSet[0x33] = &Logger::inc_SP;
        m_instructionSet[0x34] = &Logger::inc_HL;
        m_instructionSet[0x35] = &Logger::dec_HL;
        m_instructionSet[0x36] = &Logger::ld_HL_n_8;
        m_instructionSet[0x37] = &Logger::scf;
        m_instructionSet[0x38] = &Logger::jr_c_n;
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
        m_instructionSet[0x76] = &Logger::halt;
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
        m_instructionSet[0xC0] = &Logger::ret_nz;
        m_instructionSet[0xC1] = &Logger::pop<Registers::BC>;
        m_instructionSet[0xC2] = &Logger::jp_nz_nn;
        m_instructionSet[0xC3] = &Logger::jp_nn;
        m_instructionSet[0xC4] = &Logger::call_nz_nn;
        m_instructionSet[0xC5] = &Logger::push<Registers::BC>;
        m_instructionSet[0xC6] = &Logger::add_n;
        m_instructionSet[0xC7] = &Logger::rst<0x00>;
        m_instructionSet[0xC8] = &Logger::ret_z;
        m_instructionSet[0xC9] = &Logger::ret;
        m_instructionSet[0xCA] = &Logger::jp_z_nn;
        m_instructionSet[0xCB] = &Logger::cb;
        m_instructionSet[0xCC] = &Logger::call_z_nn;
        m_instructionSet[0xCD] = &Logger::call_nn;
        m_instructionSet[0xCE] = &Logger::adc_n;
        m_instructionSet[0xCF] = &Logger::rst<0x08>;
        m_instructionSet[0xD0] = &Logger::ret_nc;
        m_instructionSet[0xD1] = &Logger::pop<Registers::DE>;
        m_instructionSet[0xD2] = &Logger::jp_nc_nn;
        m_instructionSet[0xD4] = &Logger::call_nc_nn;
        m_instructionSet[0xD5] = &Logger::push<Registers::DE>;
        m_instructionSet[0xD6] = &Logger::sub_n;
        m_instructionSet[0xD7] = &Logger::rst<0x10>;
        m_instructionSet[0xD8] = &Logger::ret_c;
        m_instructionSet[0xD9] = &Logger::reti;
        m_instructionSet[0xDA] = &Logger::jp_c_nn;
        m_instructionSet[0xDC] = &Logger::call_c_nn;
        m_instructionSet[0xDE] = &Logger::sbc_n;
        m_instructionSet[0xDF] = &Logger::rst<0x18>;
        m_instructionSet[0xE0] = &Logger::ldh_an_A;
        m_instructionSet[0xE1] = &Logger::pop<Registers::HL>;
        m_instructionSet[0xE2] = &Logger::ldh_aC_A;
        m_instructionSet[0xE5] = &Logger::push<Registers::HL>;
        m_instructionSet[0xE6] = &Logger::and_n;
        m_instructionSet[0xE7] = &Logger::rst<0x20>;
        m_instructionSet[0xE8] = &Logger::add_SP_n;
        m_instructionSet[0xE9] = &Logger::jp_HL;
        m_instructionSet[0xEA] = &Logger::ld_nn_A;
        m_instructionSet[0xEE] = &Logger::xor_n;
        m_instructionSet[0xEF] = &Logger::rst<0x28>;
        m_instructionSet[0xF0] = &Logger::ldh_A_an;
        m_instructionSet[0xF1] = &Logger::pop<Registers::AF>;
        m_instructionSet[0xF2] = &Logger::ldh_A_aC;
        m_instructionSet[0xF3] = &Logger::di;
        m_instructionSet[0xF5] = &Logger::push<Registers::AF>;
        m_instructionSet[0xF6] = &Logger::or_n;
        m_instructionSet[0xF7] = &Logger::rst<0x30>;
        m_instructionSet[0xF8] = &Logger::ld_HL_SP_r8;
        m_instructionSet[0xF9] = &Logger::ld_SP_HL;
        m_instructionSet[0xFA] = &Logger::ld_A_nn;
        m_instructionSet[0xFB] = &Logger::ei;
        m_instructionSet[0xFE] = &Logger::cp_n;
        m_instructionSet[0xFF] = &Logger::rst<0x38>;
    }

    void Logger::fillCbInstructionSet()
    {
        m_cbInstructionSet[0x00] = &Logger::rlc_r<Registers::B>;
        m_cbInstructionSet[0x01] = &Logger::rlc_r<Registers::C>;
        m_cbInstructionSet[0x02] = &Logger::rlc_r<Registers::D>;
        m_cbInstructionSet[0x03] = &Logger::rlc_r<Registers::E>;
        m_cbInstructionSet[0x04] = &Logger::rlc_r<Registers::H>;
        m_cbInstructionSet[0x05] = &Logger::rlc_r<Registers::L>;
        m_cbInstructionSet[0x06] = &Logger::rlc_HL;
        m_cbInstructionSet[0x07] = &Logger::rlc_r<Registers::A>;
        m_cbInstructionSet[0x08] = &Logger::rrc_r<Registers::B>;
        m_cbInstructionSet[0x09] = &Logger::rrc_r<Registers::C>;
        m_cbInstructionSet[0x0A] = &Logger::rrc_r<Registers::D>;
        m_cbInstructionSet[0x0B] = &Logger::rrc_r<Registers::E>;
        m_cbInstructionSet[0x0C] = &Logger::rrc_r<Registers::H>;
        m_cbInstructionSet[0x0D] = &Logger::rrc_r<Registers::L>;
        m_cbInstructionSet[0x0E] = &Logger::rrc_HL;
        m_cbInstructionSet[0x0F] = &Logger::rrc_r<Registers::A>;
        m_cbInstructionSet[0x10] = &Logger::rl_r<Registers::B>;
        m_cbInstructionSet[0x11] = &Logger::rl_r<Registers::C>;
        m_cbInstructionSet[0x12] = &Logger::rl_r<Registers::D>;
        m_cbInstructionSet[0x13] = &Logger::rl_r<Registers::E>;
        m_cbInstructionSet[0x14] = &Logger::rl_r<Registers::H>;
        m_cbInstructionSet[0x15] = &Logger::rl_r<Registers::L>;
        m_cbInstructionSet[0x16] = &Logger::rl_HL;
        m_cbInstructionSet[0x17] = &Logger::rl_r<Registers::A>;
        m_cbInstructionSet[0x18] = &Logger::rr_r<Registers::B>;
        m_cbInstructionSet[0x19] = &Logger::rr_r<Registers::C>;
        m_cbInstructionSet[0x1A] = &Logger::rr_r<Registers::D>;
        m_cbInstructionSet[0x1B] = &Logger::rr_r<Registers::E>;
        m_cbInstructionSet[0x1C] = &Logger::rr_r<Registers::H>;
        m_cbInstructionSet[0x1D] = &Logger::rr_r<Registers::L>;
        m_cbInstructionSet[0x1E] = &Logger::rr_HL;
        m_cbInstructionSet[0x1F] = &Logger::rr_r<Registers::A>;
        m_cbInstructionSet[0x20] = &Logger::sla_r<Registers::B>;
        m_cbInstructionSet[0x21] = &Logger::sla_r<Registers::C>;
        m_cbInstructionSet[0x22] = &Logger::sla_r<Registers::D>;
        m_cbInstructionSet[0x23] = &Logger::sla_r<Registers::E>;
        m_cbInstructionSet[0x24] = &Logger::sla_r<Registers::H>;
        m_cbInstructionSet[0x25] = &Logger::sla_r<Registers::L>;
        m_cbInstructionSet[0x26] = &Logger::sla_HL;
        m_cbInstructionSet[0x27] = &Logger::sla_r<Registers::A>;
        m_cbInstructionSet[0x28] = &Logger::sra_r<Registers::B>;
        m_cbInstructionSet[0x29] = &Logger::sra_r<Registers::C>;
        m_cbInstructionSet[0x2A] = &Logger::sra_r<Registers::D>;
        m_cbInstructionSet[0x2B] = &Logger::sra_r<Registers::E>;
        m_cbInstructionSet[0x2C] = &Logger::sra_r<Registers::H>;
        m_cbInstructionSet[0x2D] = &Logger::sra_r<Registers::L>;
        m_cbInstructionSet[0x2E] = &Logger::sra_HL;
        m_cbInstructionSet[0x2F] = &Logger::swap_r<Registers::A>;
        m_cbInstructionSet[0x30] = &Logger::swap_r<Registers::B>;
        m_cbInstructionSet[0x31] = &Logger::swap_r<Registers::C>;
        m_cbInstructionSet[0x32] = &Logger::swap_r<Registers::D>;
        m_cbInstructionSet[0x33] = &Logger::swap_r<Registers::E>;
        m_cbInstructionSet[0x34] = &Logger::swap_r<Registers::H>;
        m_cbInstructionSet[0x35] = &Logger::swap_r<Registers::L>;
        m_cbInstructionSet[0x36] = &Logger::swap_HL;
        m_cbInstructionSet[0x37] = &Logger::swap_r<Registers::A>;
        m_cbInstructionSet[0x38] = &Logger::srl_r<Registers::B>;
        m_cbInstructionSet[0x39] = &Logger::srl_r<Registers::C>;
        m_cbInstructionSet[0x3A] = &Logger::srl_r<Registers::D>;
        m_cbInstructionSet[0x3B] = &Logger::srl_r<Registers::E>;
        m_cbInstructionSet[0x3C] = &Logger::srl_r<Registers::H>;
        m_cbInstructionSet[0x3D] = &Logger::srl_r<Registers::L>;
        m_cbInstructionSet[0x3E] = &Logger::srl_HL;
        m_cbInstructionSet[0x3F] = &Logger::srl_r<Registers::A>;
        m_cbInstructionSet[0x40] = &Logger::bit_n_r<0, Registers::B>;
        m_cbInstructionSet[0x41] = &Logger::bit_n_r<0, Registers::C>;
        m_cbInstructionSet[0x42] = &Logger::bit_n_r<0, Registers::D>;
        m_cbInstructionSet[0x43] = &Logger::bit_n_r<0, Registers::E>;
        m_cbInstructionSet[0x44] = &Logger::bit_n_r<0, Registers::H>;
        m_cbInstructionSet[0x45] = &Logger::bit_n_r<0, Registers::L>;
        m_cbInstructionSet[0x46] = &Logger::bit_n_HL<0>;
        m_cbInstructionSet[0x47] = &Logger::bit_n_r<0, Registers::A>;
        m_cbInstructionSet[0x48] = &Logger::bit_n_r<1, Registers::B>;
        m_cbInstructionSet[0x49] = &Logger::bit_n_r<1, Registers::C>;
        m_cbInstructionSet[0x4A] = &Logger::bit_n_r<1, Registers::D>;
        m_cbInstructionSet[0x4B] = &Logger::bit_n_r<1, Registers::E>;
        m_cbInstructionSet[0x4C] = &Logger::bit_n_r<1, Registers::H>;
        m_cbInstructionSet[0x4D] = &Logger::bit_n_r<1, Registers::L>;
        m_cbInstructionSet[0x4E] = &Logger::bit_n_HL<1>;
        m_cbInstructionSet[0x4F] = &Logger::bit_n_r<1, Registers::A>;
        m_cbInstructionSet[0x50] = &Logger::bit_n_r<2, Registers::B>;
        m_cbInstructionSet[0x51] = &Logger::bit_n_r<2, Registers::C>;
        m_cbInstructionSet[0x52] = &Logger::bit_n_r<2, Registers::D>;
        m_cbInstructionSet[0x53] = &Logger::bit_n_r<2, Registers::E>;
        m_cbInstructionSet[0x54] = &Logger::bit_n_r<2, Registers::H>;
        m_cbInstructionSet[0x55] = &Logger::bit_n_r<2, Registers::L>;
        m_cbInstructionSet[0x56] = &Logger::bit_n_HL<2>;
        m_cbInstructionSet[0x57] = &Logger::bit_n_r<2, Registers::A>;
        m_cbInstructionSet[0x58] = &Logger::bit_n_r<3, Registers::B>;
        m_cbInstructionSet[0x59] = &Logger::bit_n_r<3, Registers::C>;
        m_cbInstructionSet[0x5A] = &Logger::bit_n_r<3, Registers::D>;
        m_cbInstructionSet[0x5B] = &Logger::bit_n_r<3, Registers::E>;
        m_cbInstructionSet[0x5C] = &Logger::bit_n_r<3, Registers::H>;
        m_cbInstructionSet[0x5D] = &Logger::bit_n_r<3, Registers::L>;
        m_cbInstructionSet[0x5E] = &Logger::bit_n_HL<3>;
        m_cbInstructionSet[0x5F] = &Logger::bit_n_r<3, Registers::A>;
        m_cbInstructionSet[0x60] = &Logger::bit_n_r<4, Registers::B>;
        m_cbInstructionSet[0x61] = &Logger::bit_n_r<4, Registers::C>;
        m_cbInstructionSet[0x62] = &Logger::bit_n_r<4, Registers::D>;
        m_cbInstructionSet[0x63] = &Logger::bit_n_r<4, Registers::E>;
        m_cbInstructionSet[0x64] = &Logger::bit_n_r<4, Registers::H>;
        m_cbInstructionSet[0x65] = &Logger::bit_n_r<4, Registers::L>;
        m_cbInstructionSet[0x66] = &Logger::bit_n_HL<4>;
        m_cbInstructionSet[0x67] = &Logger::bit_n_r<4, Registers::A>;
        m_cbInstructionSet[0x68] = &Logger::bit_n_r<5, Registers::B>;
        m_cbInstructionSet[0x69] = &Logger::bit_n_r<5, Registers::C>;
        m_cbInstructionSet[0x6A] = &Logger::bit_n_r<5, Registers::D>;
        m_cbInstructionSet[0x6B] = &Logger::bit_n_r<5, Registers::E>;
        m_cbInstructionSet[0x6C] = &Logger::bit_n_r<5, Registers::H>;
        m_cbInstructionSet[0x6D] = &Logger::bit_n_r<5, Registers::L>;
        m_cbInstructionSet[0x6E] = &Logger::bit_n_HL<5>;
        m_cbInstructionSet[0x6F] = &Logger::bit_n_r<6, Registers::A>;
        m_cbInstructionSet[0x70] = &Logger::bit_n_r<6, Registers::B>;
        m_cbInstructionSet[0x71] = &Logger::bit_n_r<6, Registers::C>;
        m_cbInstructionSet[0x72] = &Logger::bit_n_r<6, Registers::D>;
        m_cbInstructionSet[0x73] = &Logger::bit_n_r<6, Registers::E>;
        m_cbInstructionSet[0x74] = &Logger::bit_n_r<6, Registers::H>;
        m_cbInstructionSet[0x75] = &Logger::bit_n_r<6, Registers::L>;
        m_cbInstructionSet[0x76] = &Logger::bit_n_HL<6>;
        m_cbInstructionSet[0x77] = &Logger::bit_n_r<6, Registers::A>;
        m_cbInstructionSet[0x78] = &Logger::bit_n_r<7, Registers::B>;
        m_cbInstructionSet[0x79] = &Logger::bit_n_r<7, Registers::C>;
        m_cbInstructionSet[0x7A] = &Logger::bit_n_r<7, Registers::D>;
        m_cbInstructionSet[0x7B] = &Logger::bit_n_r<7, Registers::E>;
        m_cbInstructionSet[0x7C] = &Logger::bit_n_r<7, Registers::H>;
        m_cbInstructionSet[0x7D] = &Logger::bit_n_r<7, Registers::L>;
        m_cbInstructionSet[0x7E] = &Logger::bit_n_HL<7>;
        m_cbInstructionSet[0x7F] = &Logger::bit_n_r<7, Registers::A>;
        m_cbInstructionSet[0x80] = &Logger::res_n_r<0, Registers::B>;
        m_cbInstructionSet[0x81] = &Logger::res_n_r<0, Registers::C>;
        m_cbInstructionSet[0x82] = &Logger::res_n_r<0, Registers::D>;
        m_cbInstructionSet[0x83] = &Logger::res_n_r<0, Registers::E>;
        m_cbInstructionSet[0x84] = &Logger::res_n_r<0, Registers::H>;
        m_cbInstructionSet[0x85] = &Logger::res_n_r<0, Registers::L>;
        m_cbInstructionSet[0x86] = &Logger::res_n_HL<0>;
        m_cbInstructionSet[0x87] = &Logger::res_n_r<0, Registers::A>;
        m_cbInstructionSet[0x88] = &Logger::res_n_r<1, Registers::B>;
        m_cbInstructionSet[0x89] = &Logger::res_n_r<1, Registers::C>;
        m_cbInstructionSet[0x8A] = &Logger::res_n_r<1, Registers::D>;
        m_cbInstructionSet[0x8B] = &Logger::res_n_r<1, Registers::E>;
        m_cbInstructionSet[0x8C] = &Logger::res_n_r<1, Registers::H>;
        m_cbInstructionSet[0x8D] = &Logger::res_n_r<1, Registers::L>;
        m_cbInstructionSet[0x8E] = &Logger::res_n_HL<1>;
        m_cbInstructionSet[0x8F] = &Logger::res_n_r<1, Registers::A>;
        m_cbInstructionSet[0x90] = &Logger::res_n_r<2, Registers::B>;
        m_cbInstructionSet[0x91] = &Logger::res_n_r<2, Registers::C>;
        m_cbInstructionSet[0x92] = &Logger::res_n_r<2, Registers::D>;
        m_cbInstructionSet[0x93] = &Logger::res_n_r<2, Registers::E>;
        m_cbInstructionSet[0x94] = &Logger::res_n_r<2, Registers::H>;
        m_cbInstructionSet[0x95] = &Logger::res_n_r<2, Registers::L>;
        m_cbInstructionSet[0x96] = &Logger::res_n_HL<2>;
        m_cbInstructionSet[0x97] = &Logger::res_n_r<2, Registers::A>;
        m_cbInstructionSet[0x98] = &Logger::res_n_r<3, Registers::B>;
        m_cbInstructionSet[0x99] = &Logger::res_n_r<3, Registers::C>;
        m_cbInstructionSet[0x9A] = &Logger::res_n_r<3, Registers::D>;
        m_cbInstructionSet[0x9B] = &Logger::res_n_r<3, Registers::E>;
        m_cbInstructionSet[0x9C] = &Logger::res_n_r<3, Registers::H>;
        m_cbInstructionSet[0x9D] = &Logger::res_n_r<3, Registers::L>;
        m_cbInstructionSet[0x9E] = &Logger::res_n_HL<3>;
        m_cbInstructionSet[0x9F] = &Logger::res_n_r<3, Registers::A>;
        m_cbInstructionSet[0xA0] = &Logger::res_n_r<4, Registers::B>;
        m_cbInstructionSet[0xA1] = &Logger::res_n_r<4, Registers::C>;
        m_cbInstructionSet[0xA2] = &Logger::res_n_r<4, Registers::D>;
        m_cbInstructionSet[0xA3] = &Logger::res_n_r<4, Registers::E>;
        m_cbInstructionSet[0xA4] = &Logger::res_n_r<4, Registers::H>;
        m_cbInstructionSet[0xA5] = &Logger::res_n_r<4, Registers::L>;
        m_cbInstructionSet[0xA6] = &Logger::res_n_HL<4>;
        m_cbInstructionSet[0xA7] = &Logger::res_n_r<4, Registers::A>;
        m_cbInstructionSet[0xA8] = &Logger::res_n_r<5, Registers::B>;
        m_cbInstructionSet[0xA9] = &Logger::res_n_r<5, Registers::C>;
        m_cbInstructionSet[0xAA] = &Logger::res_n_r<5, Registers::D>;
        m_cbInstructionSet[0xAB] = &Logger::res_n_r<5, Registers::E>;
        m_cbInstructionSet[0xAC] = &Logger::res_n_r<5, Registers::H>;
        m_cbInstructionSet[0xAD] = &Logger::res_n_r<5, Registers::L>;
        m_cbInstructionSet[0xAE] = &Logger::res_n_HL<5>;
        m_cbInstructionSet[0xAF] = &Logger::res_n_r<6, Registers::A>;
        m_cbInstructionSet[0xB0] = &Logger::res_n_r<6, Registers::B>;
        m_cbInstructionSet[0xB1] = &Logger::res_n_r<6, Registers::C>;
        m_cbInstructionSet[0xB2] = &Logger::res_n_r<6, Registers::D>;
        m_cbInstructionSet[0xB3] = &Logger::res_n_r<6, Registers::E>;
        m_cbInstructionSet[0xB4] = &Logger::res_n_r<6, Registers::H>;
        m_cbInstructionSet[0xB5] = &Logger::res_n_r<6, Registers::L>;
        m_cbInstructionSet[0xB6] = &Logger::res_n_HL<6>;
        m_cbInstructionSet[0xB7] = &Logger::res_n_r<6, Registers::A>;
        m_cbInstructionSet[0xB8] = &Logger::res_n_r<7, Registers::B>;
        m_cbInstructionSet[0xB9] = &Logger::res_n_r<7, Registers::C>;
        m_cbInstructionSet[0xBA] = &Logger::res_n_r<7, Registers::D>;
        m_cbInstructionSet[0xBB] = &Logger::res_n_r<7, Registers::E>;
        m_cbInstructionSet[0xBC] = &Logger::res_n_r<7, Registers::H>;
        m_cbInstructionSet[0xBD] = &Logger::res_n_r<7, Registers::L>;
        m_cbInstructionSet[0xBE] = &Logger::res_n_HL<7>;
        m_cbInstructionSet[0xBF] = &Logger::res_n_r<7, Registers::A>;
        m_cbInstructionSet[0xC0] = &Logger::set_n_r<0, Registers::B>;
        m_cbInstructionSet[0xC1] = &Logger::set_n_r<0, Registers::C>;
        m_cbInstructionSet[0xC2] = &Logger::set_n_r<0, Registers::D>;
        m_cbInstructionSet[0xC3] = &Logger::set_n_r<0, Registers::E>;
        m_cbInstructionSet[0xC4] = &Logger::set_n_r<0, Registers::H>;
        m_cbInstructionSet[0xC5] = &Logger::set_n_r<0, Registers::L>;
        m_cbInstructionSet[0xC6] = &Logger::set_n_HL<0>;
        m_cbInstructionSet[0xC7] = &Logger::set_n_r<0, Registers::A>;
        m_cbInstructionSet[0xC8] = &Logger::set_n_r<1, Registers::B>;
        m_cbInstructionSet[0xC9] = &Logger::set_n_r<1, Registers::C>;
        m_cbInstructionSet[0xCA] = &Logger::set_n_r<1, Registers::D>;
        m_cbInstructionSet[0xCB] = &Logger::set_n_r<1, Registers::E>;
        m_cbInstructionSet[0xCC] = &Logger::set_n_r<1, Registers::H>;
        m_cbInstructionSet[0xCD] = &Logger::set_n_r<1, Registers::L>;
        m_cbInstructionSet[0xCE] = &Logger::set_n_HL<1>;
        m_cbInstructionSet[0xCF] = &Logger::set_n_r<1, Registers::A>;
        m_cbInstructionSet[0xD0] = &Logger::set_n_r<2, Registers::B>;
        m_cbInstructionSet[0xD1] = &Logger::set_n_r<2, Registers::C>;
        m_cbInstructionSet[0xD2] = &Logger::set_n_r<2, Registers::D>;
        m_cbInstructionSet[0xD3] = &Logger::set_n_r<2, Registers::E>;
        m_cbInstructionSet[0xD4] = &Logger::set_n_r<2, Registers::H>;
        m_cbInstructionSet[0xD5] = &Logger::set_n_r<2, Registers::L>;
        m_cbInstructionSet[0xD6] = &Logger::set_n_HL<2>;
        m_cbInstructionSet[0xD7] = &Logger::set_n_r<2, Registers::A>;
        m_cbInstructionSet[0xD8] = &Logger::set_n_r<3, Registers::B>;
        m_cbInstructionSet[0xD9] = &Logger::set_n_r<3, Registers::C>;
        m_cbInstructionSet[0xDA] = &Logger::set_n_r<3, Registers::D>;
        m_cbInstructionSet[0xDB] = &Logger::set_n_r<3, Registers::E>;
        m_cbInstructionSet[0xDC] = &Logger::set_n_r<3, Registers::H>;
        m_cbInstructionSet[0xDD] = &Logger::set_n_r<3, Registers::L>;
        m_cbInstructionSet[0xDE] = &Logger::set_n_HL<3>;
        m_cbInstructionSet[0xDF] = &Logger::set_n_r<3, Registers::A>;
        m_cbInstructionSet[0xE0] = &Logger::set_n_r<4, Registers::B>;
        m_cbInstructionSet[0xE1] = &Logger::set_n_r<4, Registers::C>;
        m_cbInstructionSet[0xE2] = &Logger::set_n_r<4, Registers::D>;
        m_cbInstructionSet[0xE3] = &Logger::set_n_r<4, Registers::E>;
        m_cbInstructionSet[0xE4] = &Logger::set_n_r<4, Registers::H>;
        m_cbInstructionSet[0xE5] = &Logger::set_n_r<4, Registers::L>;
        m_cbInstructionSet[0xE6] = &Logger::set_n_HL<4>;
        m_cbInstructionSet[0xE7] = &Logger::set_n_r<4, Registers::A>;
        m_cbInstructionSet[0xE8] = &Logger::set_n_r<5, Registers::B>;
        m_cbInstructionSet[0xE9] = &Logger::set_n_r<5, Registers::C>;
        m_cbInstructionSet[0xEA] = &Logger::set_n_r<5, Registers::D>;
        m_cbInstructionSet[0xEB] = &Logger::set_n_r<5, Registers::E>;
        m_cbInstructionSet[0xEC] = &Logger::set_n_r<5, Registers::H>;
        m_cbInstructionSet[0xED] = &Logger::set_n_r<5, Registers::L>;
        m_cbInstructionSet[0xEE] = &Logger::set_n_HL<5>;
        m_cbInstructionSet[0xEF] = &Logger::set_n_r<6, Registers::A>;
        m_cbInstructionSet[0xF0] = &Logger::set_n_r<6, Registers::B>;
        m_cbInstructionSet[0xF1] = &Logger::set_n_r<6, Registers::C>;
        m_cbInstructionSet[0xF2] = &Logger::set_n_r<6, Registers::D>;
        m_cbInstructionSet[0xF3] = &Logger::set_n_r<6, Registers::E>;
        m_cbInstructionSet[0xF4] = &Logger::set_n_r<6, Registers::H>;
        m_cbInstructionSet[0xF5] = &Logger::set_n_r<6, Registers::L>;
        m_cbInstructionSet[0xF6] = &Logger::set_n_HL<6>;
        m_cbInstructionSet[0xF7] = &Logger::set_n_r<6, Registers::A>;
        m_cbInstructionSet[0xF8] = &Logger::set_n_r<7, Registers::B>;
        m_cbInstructionSet[0xF9] = &Logger::set_n_r<7, Registers::C>;
        m_cbInstructionSet[0xFA] = &Logger::set_n_r<7, Registers::D>;
        m_cbInstructionSet[0xFB] = &Logger::set_n_r<7, Registers::E>;
        m_cbInstructionSet[0xFC] = &Logger::set_n_r<7, Registers::H>;
        m_cbInstructionSet[0xFD] = &Logger::set_n_r<7, Registers::L>;
        m_cbInstructionSet[0xFE] = &Logger::set_n_HL<7>;
        m_cbInstructionSet[0xFF] = &Logger::set_n_r<7, Registers::A>;
    }

    void Logger::unhandled(uint8_t opCode)
    {
        printf("0x%2X : Not handled yet;\n", opCode);
    }

    void Logger::nop(uint8_t opCode)
    {
        printf("0x%2X : NOP;\n", opCode);
    }

    void Logger::ld_HL_n_8(uint8_t opCode)
    {
        printf("0x%2X : LD (HL), n; \n", opCode);
    }

    void Logger::ld_A_nn(uint8_t opCode)
    {
        printf("0x%2X : LD A, nn;\n", opCode);
    }

    void Logger::ld_nn_A(uint8_t opCode)
    {
        printf("0x%2X : LD nn, A;\n", opCode);
    }

    void Logger::ldh_A_aC(uint8_t opCode)
    {
        printf("0x%2X : : LDH A, (C);\n", opCode);
    }

    void Logger::ldh_aC_A(uint8_t opCode)
    {
        printf("0x%2X : LDH (C), A;\n", opCode);
    }

    void Logger::ldh_A_an(uint8_t opCode)
    {
        printf("0x%2X : LDH A, (n);\n", opCode);
    }

    void Logger::ldh_an_A(uint8_t opCode)
    {
        printf("0x%2X : LDH (n), A;\n", opCode);
    }

    void Logger::ld_HLd_A(uint8_t opCode)
    {
        printf("0x%2X : LD (HL-), A;\n", opCode);
    }

    void Logger::ld_A_HLd(uint8_t opCode)
    {
        printf("0x%2X : LD A, (HL-);\n", opCode);
    }

    void Logger::ld_HLi_A(uint8_t opCode)
    {
        printf("0x%2X : LD (HL+), A;\n", opCode);
    }

    void Logger::ld_A_HLi(uint8_t opCode)
    {
        printf("0x%2X : LD A, (HL+);\n", opCode);
    }

    void Logger::ld_SP_nn(uint8_t opCode)
    {
        printf("0x%2X : LD SP, nn;\n", opCode);
    }

    void Logger::ld_nn_SP(uint8_t opCode)
    {
        printf("0x%2X : LD SP, rr;\n", opCode);
    }

    void Logger::ld_SP_HL(uint8_t opCode)
    {
        printf("0x%2X : LD SP, HL;\n", opCode);
    }

    void Logger::ld_HL_SP_r8(uint8_t opCode)
    {
        printf("0x%2X : LD HL, SP + r8;\n", opCode);
    }

    void Logger::add_HL(uint8_t opCode)
    {
        printf("0x%2X : ADD (HL);\n", opCode);
    }

    void Logger::add_n(uint8_t opCode)
    {
        printf("0x%2X : ADD n;\n", opCode);
    }

    void Logger::adc_HL(uint8_t opCode)
    {
        printf("0x%2X : ADC (HL);\n", opCode);
    }

    void Logger::adc_n(uint8_t opCode)
    {
        printf("0x%2X : ADC n;\n", opCode);
    }

    void Logger::sub_HL(uint8_t opCode)
    {
        printf("0x%2X : SUB (HL);\n", opCode);
    }

    void Logger::sub_n(uint8_t opCode)
    {
        printf("0x%2X : SUB n;\n", opCode);
    }

    void Logger::sbc_HL(uint8_t opCode)
    {
        printf("0x%2X : SBC (HL);\n", opCode);
    }

    void Logger::sbc_n(uint8_t opCode)
    {
        printf("0x%2X : SBC n;\n", opCode);
    }

    void Logger::cp_HL(uint8_t opCode)
    {
        printf("0x%2X : CP (HL);\n", opCode);
    }

    void Logger::cp_n(uint8_t opCode)
    {
        printf("0x%2X : CP n;\n", opCode);
    }

    void Logger::inc_HL(uint8_t opCode)
    {
        printf("0x%2X : DEC (HL);\n", opCode);
    }

    void Logger::dec_HL(uint8_t opCode)
    {
        printf("0x%2X : INC (HL);\n", opCode);
    }

    void Logger::and_HL(uint8_t opCode)
    {
        printf("0x%2X : AND (HL);\n", opCode);
    }

    void Logger::and_n(uint8_t opCode)
    {
        printf("0x%2X : AND n;\n", opCode);
    }

    void Logger::or_HL(uint8_t opCode)
    {
        printf("0x%2X : OR (HL);\n", opCode);
    }

    void Logger::or_n(uint8_t opCode)
    {
        printf("0x%2X : OR n;\n", opCode);
    }

    void Logger::xor_HL(uint8_t opCode)
    {
        printf("0x%2X : XOR (HL);\n", opCode);
    }

    void Logger::xor_n(uint8_t opCode)
    {
        printf("0x%2X : XOR n;\n", opCode);
    }

    void Logger::scf(uint8_t opCode)
    {
        printf("0x%2X : CCF;\n", opCode);
    }

    void Logger::ccf(uint8_t opCode)
    {
        printf("0x%2X : SCF;\n", opCode);
    }

    void Logger::cpl(uint8_t opCode)
    {
        printf("0x%2X : CPL;\n", opCode);
    }

    void Logger::daa(uint8_t opCode)
    {
        printf("0x%2X : DAA;\n", opCode);
    }

    void Logger::add_HL_SP(uint8_t opCode)
    {
        printf("0x%2X : ADD HL SP;\n", opCode);
    }

    void Logger::add_SP_n(uint8_t opCode)
    {
        printf("0x%2X : ADD SP n;\n", opCode);
    }

    void Logger::inc_SP(uint8_t opCode)
    {
        printf("0x%2X : INC SP;\n", opCode);
    }

    void Logger::dec_SP(uint8_t opCode)
    {
        printf("0x%2X : DEC SP;\n", opCode);
    }

    void Logger::rlca(uint8_t opCode)
    {
        printf("0x%2X : RLCA;\n", opCode);
    }

    void Logger::rlc_HL(uint8_t opCode)
    {
        printf("0x%2X : RLC HL;\n", opCode);
    }

    void Logger::rla(uint8_t opCode)
    {
        printf("0x%2X : RLA;\n", opCode);
    }

    void Logger::rl_HL(uint8_t opCode)
    {
        printf("0x%2X : RL HL;\n", opCode);
    }

    void Logger::rrca(uint8_t opCode)
    {
        printf("0x%2X : RRCA;\n", opCode);
    }

    void Logger::rrc_HL(uint8_t opCode)
    {
        printf("0x%2X : RRC HL;\n", opCode);
    }

    void Logger::rra(uint8_t opCode)
    {
        printf("0x%2X : RRA;\n", opCode);
    }

    void Logger::rr_HL(uint8_t opCode)
    {
        printf("0x%2X : RR HL;\n", opCode);
    }

    void Logger::sla_HL(uint8_t opCode)
    {
        printf("0x%2X : SLA HL;\n", opCode);
    }

    void Logger::sra_HL(uint8_t opCode)
    {
        printf("0x%2X : SRA HL;\n", opCode);
    }

    void Logger::swap_HL(uint8_t opCode)
    {
        printf("0x%2X : SWAP HL;\n", opCode);
    }

    void Logger::srl_HL(uint8_t opCode)
    {
        printf("0x%2X : SRL HL;\n", opCode);
    }

    void Logger::jp_nn(uint8_t opCode)
    {
        printf("0x%2X : JP nn;\n", opCode);
    }

    void Logger::jp_HL(uint8_t opCode)
    {
        printf("0x%2X : JP HL;\n", opCode);
    }

    void Logger::jp_nz_nn(uint8_t opCode)
    {
        printf("0x%2X : JP NZ, nn;\n", opCode);
    }

    void Logger::jp_z_nn(uint8_t opCode)
    {
        printf("0x%2X : JP Z, nn;\n", opCode);
    }

    void Logger::jp_nc_nn(uint8_t opCode)
    {
        printf("0x%2X : JP NC, nn;\n", opCode);
    }

    void Logger::jp_c_nn(uint8_t opCode)
    {
        printf("0x%2X : JP C, nn;\n", opCode);
    }

    void Logger::jr_e(uint8_t opCode)
    {
        printf("0x%2X : JR e;\n", opCode);
    }

    void Logger::jr_nz_n(uint8_t opCode)
    {
        printf("0x%2X : JR NZ, nn;\n", opCode);
    }

    void Logger::jr_z_n(uint8_t opCode)
    {
        printf("0x%2X : JR Z, nn;\n", opCode);
    }

    void Logger::jr_nc_n(uint8_t opCode)
    {
        printf("0x%2X : JR NC, nn;\n", opCode);
    }

    void Logger::jr_c_n(uint8_t opCode)
    {
        printf("0x%2X : JR C, nn;\n", opCode);
    }

    void Logger::call_nn(uint8_t opCode)
    {
        printf("0x%2X : CALL nn;\n", opCode);
    }

    void Logger::call_nz_nn(uint8_t opCode)
    {
        printf("0x%2X : CALL NZ, nn;\n", opCode);
    }

    void Logger::call_z_nn(uint8_t opCode)
    {
        printf("0x%2X : CALL Z, nn;\n", opCode);
    }

    void Logger::call_nc_nn(uint8_t opCode)
    {
        printf("0x%2X : CALL NC, nn;\n", opCode);
    }

    void Logger::call_c_nn(uint8_t opCode)
    {
        printf("0x%2X : CALL C, nn;\n", opCode);
    }

    void Logger::ret(uint8_t opCode)
    {
        printf("0x%2X : RET;\n", opCode);
    }

    void Logger::ret_nz(uint8_t opCode)
    {
        printf("0x%2X : RET NZ;\n", opCode);
    }

    void Logger::ret_z(uint8_t opCode)
    {
        printf("0x%2X : RET Z;\n", opCode);
    }

    void Logger::ret_nc(uint8_t opCode)
    {
        printf("0x%2X : RET C;\n", opCode);
    }

    void Logger::ret_c(uint8_t opCode)
    {
        printf("0x%2X : RET NC;\n", opCode);
    }

    void Logger::reti(uint8_t opCode)
    {
        printf("0x%2X : RETI;\n", opCode);
    }
    
    void Logger::di(uint8_t opCode)
    {
        printf("0x%2X : DI;\n", opCode);
    }
    
    void Logger::ei(uint8_t opCode)
    {
        printf("0x%2X : EI;\n", opCode);
    }

    void Logger::halt(uint8_t opCode)
    {
        printf("0x%2X : HALT;\n", opCode);
    }

    void Logger::stop(uint8_t opCode)
    {
        printf("0x%2X : STOP;\n", opCode);
    }

    void Logger::cb(uint8_t)
    {
        printf("CB ");
        
        uint8_t n = m_memory.read8(m_registers.getPC() + 1);
        ((this->*m_cbInstructionSet[n]))(n);
    }
}