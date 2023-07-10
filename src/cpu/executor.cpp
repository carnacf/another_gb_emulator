#include "executor.h"

#include "memory.h"
#include "registery.h"

#include <utility>
#include <limits>

namespace cpu
{
    Executor::Executor(Registers& regist, Memory& mem): 
        m_tracer(regist, mem),
        m_registers(regist), 
        m_memory(mem)
    {
        fillInstructionSet();
        fillCbInstructionSet();
    }
    
    void Executor::fillInstructionSet()
    {
        std::fill_n(m_instructionSet, 256, &Executor::unhandled);
        
        m_instructionSet[0x00] = &Executor::nop;
        m_instructionSet[0x01] = &Executor::ld_rr_nn<Registers::BC>;
        m_instructionSet[0x02] = &Executor::ld_r16_A<Registers::BC>;
        m_instructionSet[0x03] = &Executor::inc_rr<Registers::BC>;
        m_instructionSet[0x04] = &Executor::inc_r<Registers::B>;
        m_instructionSet[0x05] = &Executor::dec_r<Registers::B>;
        m_instructionSet[0x06] = &Executor::ld_r_n_8<0x06>;
        m_instructionSet[0x07] = &Executor::rlca;
        m_instructionSet[0x08] = &Executor::ld_nn_SP;
        m_instructionSet[0x09] = &Executor::add_HL_rr<Registers::BC>;
        m_instructionSet[0x0A] = &Executor::ld_A_r16<Registers::BC>;
        m_instructionSet[0x0B] = &Executor::dec_rr<Registers::BC>;
        m_instructionSet[0x0C] = &Executor::inc_r<Registers::C>;
        m_instructionSet[0x0D] = &Executor::dec_r<Registers::C>;
        m_instructionSet[0x0E] = &Executor::ld_r_n_8<0x0E>;
        m_instructionSet[0x0F] = &Executor::rrca;
        m_instructionSet[0x10] = &Executor::stop;
        m_instructionSet[0x11] = &Executor::ld_rr_nn<Registers::DE>;
        m_instructionSet[0x12] = &Executor::ld_r16_A<Registers::DE>;
        m_instructionSet[0x13] = &Executor::inc_rr<Registers::DE>;
        m_instructionSet[0x14] = &Executor::inc_r<Registers::D>;
        m_instructionSet[0x15] = &Executor::dec_r<Registers::D>;
        m_instructionSet[0x16] = &Executor::ld_r_n_8<0x16>;
        m_instructionSet[0x17] = &Executor::rla;
        m_instructionSet[0x18] = &Executor::jr_e;
        m_instructionSet[0x19] = &Executor::add_HL_rr<Registers::DE>;
        m_instructionSet[0x1A] = &Executor::ld_A_r16<Registers::DE>;
        m_instructionSet[0x1B] = &Executor::dec_rr<Registers::DE>;
        m_instructionSet[0x1C] = &Executor::inc_r<Registers::E>;
        m_instructionSet[0x1D] = &Executor::dec_r<Registers::E>;
        m_instructionSet[0x1E] = &Executor::ld_r_n_8<0x1E>;
        m_instructionSet[0x1F] = &Executor::rra;
        m_instructionSet[0x20] = &Executor::jr_ncc_n<Registers::Flag::Z>;
        m_instructionSet[0x21] = &Executor::ld_rr_nn<Registers::HL>;
        m_instructionSet[0x22] = &Executor::ld_HLi_A;
        m_instructionSet[0x23] = &Executor::inc_rr<Registers::HL>;
        m_instructionSet[0x24] = &Executor::inc_r<Registers::H>;
        m_instructionSet[0x25] = &Executor::dec_r<Registers::H>;
        m_instructionSet[0x26] = &Executor::ld_r_n_8<0x26>;
        m_instructionSet[0x27] = &Executor::daa;
        m_instructionSet[0x28] = &Executor::jr_cc_n<Registers::Flag::Z>;
        m_instructionSet[0x29] = &Executor::add_HL_rr<Registers::HL>;
        m_instructionSet[0x2A] = &Executor::ld_A_HLi;
        m_instructionSet[0x2B] = &Executor::inc_rr<Registers::HL>;
        m_instructionSet[0x2C] = &Executor::inc_r<Registers::L>;
        m_instructionSet[0x2D] = &Executor::dec_r<Registers::L>;
        m_instructionSet[0x2E] = &Executor::ld_r_n_8<0x2E>;
        m_instructionSet[0x2F] = &Executor::cpl;
        m_instructionSet[0x30] = &Executor::jr_ncc_n<Registers::Flag::C>;
        m_instructionSet[0x31] = &Executor::ld_SP_rr;
        m_instructionSet[0x32] = &Executor::ld_HLd_A;
        m_instructionSet[0x33] = &Executor::inc_SP;
        m_instructionSet[0x34] = &Executor::inc_HL;
        m_instructionSet[0x35] = &Executor::dec_HL;
        m_instructionSet[0x36] = &Executor::ld_HL_n_8<0x36>;
        m_instructionSet[0x37] = &Executor::scf;
        m_instructionSet[0x38] = &Executor::jr_cc_n<Registers::Flag::C>;
        m_instructionSet[0x39] = &Executor::add_HL_SP;
        m_instructionSet[0x3A] = &Executor::ld_A_HLd;
        m_instructionSet[0x3B] = &Executor::dec_SP;
        m_instructionSet[0x3C] = &Executor::inc_r<Registers::A>;
        m_instructionSet[0x3D] = &Executor::dec_r<Registers::A>;
        m_instructionSet[0x3E] = &Executor::ld_r_n_8<0x3E>;
        m_instructionSet[0x3F] = &Executor::ccf;
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
        m_instructionSet[0x76] = &Executor::halt;
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
        m_instructionSet[0x90] = &Executor::sub_r<Registers::B>;
        m_instructionSet[0x91] = &Executor::sub_r<Registers::C>;
        m_instructionSet[0x92] = &Executor::sub_r<Registers::D>;
        m_instructionSet[0x93] = &Executor::sub_r<Registers::E>;
        m_instructionSet[0x94] = &Executor::sub_r<Registers::H>;
        m_instructionSet[0x95] = &Executor::sub_r<Registers::L>;
        m_instructionSet[0x96] = &Executor::sub_HL;
        m_instructionSet[0x97] = &Executor::sub_r<Registers::A>;
        m_instructionSet[0x98] = &Executor::sbc_r<Registers::B>;
        m_instructionSet[0x99] = &Executor::sbc_r<Registers::C>;
        m_instructionSet[0x9A] = &Executor::sbc_r<Registers::D>;
        m_instructionSet[0x9B] = &Executor::sbc_r<Registers::E>;
        m_instructionSet[0x9C] = &Executor::sbc_r<Registers::H>;
        m_instructionSet[0x9D] = &Executor::sbc_r<Registers::L>;
        m_instructionSet[0x9E] = &Executor::sbc_HL;
        m_instructionSet[0x9F] = &Executor::sbc_r<Registers::A>;
        m_instructionSet[0xA0] = &Executor::and_r<Registers::B>;
        m_instructionSet[0xA1] = &Executor::and_r<Registers::C>;
        m_instructionSet[0xA2] = &Executor::and_r<Registers::D>;
        m_instructionSet[0xA3] = &Executor::and_r<Registers::E>;
        m_instructionSet[0xA4] = &Executor::and_r<Registers::H>;
        m_instructionSet[0xA5] = &Executor::and_r<Registers::L>;
        m_instructionSet[0xA6] = &Executor::and_HL;
        m_instructionSet[0xA7] = &Executor::and_r<Registers::A>;
        m_instructionSet[0xA8] = &Executor::xor_r<Registers::B>;
        m_instructionSet[0xA9] = &Executor::xor_r<Registers::C>;
        m_instructionSet[0xAA] = &Executor::xor_r<Registers::D>;
        m_instructionSet[0xAB] = &Executor::xor_r<Registers::E>;
        m_instructionSet[0xAC] = &Executor::xor_r<Registers::H>;
        m_instructionSet[0xAD] = &Executor::xor_r<Registers::L>;
        m_instructionSet[0xAE] = &Executor::xor_HL;
        m_instructionSet[0xAF] = &Executor::xor_r<Registers::A>;
        m_instructionSet[0xB0] = &Executor::or_r<Registers::B>;
        m_instructionSet[0xB1] = &Executor::or_r<Registers::C>;
        m_instructionSet[0xB2] = &Executor::or_r<Registers::D>;
        m_instructionSet[0xB3] = &Executor::or_r<Registers::E>;
        m_instructionSet[0xB4] = &Executor::or_r<Registers::H>;
        m_instructionSet[0xB5] = &Executor::or_r<Registers::L>;
        m_instructionSet[0xB6] = &Executor::or_HL;
        m_instructionSet[0xA7] = &Executor::or_r<Registers::A>;
        m_instructionSet[0xB8] = &Executor::cp_r<Registers::B>;
        m_instructionSet[0xB9] = &Executor::cp_r<Registers::C>;
        m_instructionSet[0xBA] = &Executor::cp_r<Registers::D>;
        m_instructionSet[0xBB] = &Executor::cp_r<Registers::E>;
        m_instructionSet[0xBC] = &Executor::cp_r<Registers::H>;
        m_instructionSet[0xBD] = &Executor::cp_r<Registers::L>;
        m_instructionSet[0xBE] = &Executor::cp_HL;
        m_instructionSet[0xBF] = &Executor::cp_r<Registers::A>;
        m_instructionSet[0xC0] = &Executor::ret_ncc<Registers::Flag::Z>;
        m_instructionSet[0xC1] = &Executor::pop_rr<Registers::BC>;
        m_instructionSet[0xC2] = &Executor::jp_ncc_nn<Registers::Flag::Z>;
        m_instructionSet[0xC3] = &Executor::jp_nn;
        m_instructionSet[0xC4] = &Executor::call_ncc_nn<Registers::Flag::Z>;
        m_instructionSet[0xC5] = &Executor::push_rr<Registers::BC>;
        m_instructionSet[0xC6] = &Executor::add_n;
        m_instructionSet[0xC7] = &Executor::rst<0x00>;
        m_instructionSet[0xC8] = &Executor::ret_cc<Registers::Flag::Z>;
        m_instructionSet[0xC9] = &Executor::ret;
        m_instructionSet[0xCA] = &Executor::jp_cc_nn<Registers::Flag::Z>;
        m_instructionSet[0xCB] = &Executor::cb;
        m_instructionSet[0xCC] = &Executor::call_cc_nn<Registers::Flag::Z>;
        m_instructionSet[0xCD] = &Executor::call_nn;
        m_instructionSet[0xCE] = &Executor::adc_n;
        m_instructionSet[0xCF] = &Executor::rst<0x08>;
        m_instructionSet[0xD0] = &Executor::ret_ncc<Registers::Flag::C>;
        m_instructionSet[0xD1] = &Executor::pop_rr<Registers::DE>;
        m_instructionSet[0xD2] = &Executor::jp_ncc_nn<Registers::Flag::C>;
        m_instructionSet[0xD4] = &Executor::call_ncc_nn<Registers::Flag::C>;
        m_instructionSet[0xD5] = &Executor::push_rr<Registers::DE>;
        m_instructionSet[0xD6] = &Executor::sub_n;
        m_instructionSet[0xD7] = &Executor::rst<0x10>;
        m_instructionSet[0xD8] = &Executor::ret_cc<Registers::Flag::C>;
        m_instructionSet[0xD8] = &Executor::reti;
        m_instructionSet[0xDA] = &Executor::jp_cc_nn<Registers::Flag::C>;
        m_instructionSet[0xDC] = &Executor::call_cc_nn<Registers::Flag::C>;
        m_instructionSet[0xDE] = &Executor::sbc_n;
        m_instructionSet[0xDF] = &Executor::rst<0x18>;
        m_instructionSet[0xE0] = &Executor::ldh_an_A;
        m_instructionSet[0xE1] = &Executor::pop_rr<Registers::HL>;
        m_instructionSet[0xE2] = &Executor::ldh_aC_A;
        m_instructionSet[0xE5] = &Executor::push_rr<Registers::HL>;
        m_instructionSet[0xE6] = &Executor::and_n;
        m_instructionSet[0xE7] = &Executor::rst<0x20>;
        m_instructionSet[0xE8] = &Executor::add_SP_n;
        m_instructionSet[0xE9] = &Executor::jp_HL;
        m_instructionSet[0xEA] = &Executor::ld_nn_A;
        m_instructionSet[0xEE] = &Executor::xor_n;
        m_instructionSet[0xEF] = &Executor::rst<0x28>;
        m_instructionSet[0xF0] = &Executor::ldh_A_an;
        m_instructionSet[0xF1] = &Executor::pop_rr<Registers::AF>;
        m_instructionSet[0xF2] = &Executor::ldh_A_aC;
        m_instructionSet[0xF3] = &Executor::di;
        m_instructionSet[0xF5] = &Executor::push_rr<Registers::AF>;
        m_instructionSet[0xF6] = &Executor::or_n;
        m_instructionSet[0xF7] = &Executor::rst<0x30>;
        m_instructionSet[0xF8] = &Executor::ld_HL_SP_r8;
        m_instructionSet[0xF9] = &Executor::ld_SP_HL;
        m_instructionSet[0xFA] = &Executor::ld_A_nn;
        m_instructionSet[0xFB] = &Executor::ei;
        m_instructionSet[0xFE] = &Executor::cp_n;
        m_instructionSet[0xFF] = &Executor::rst<0x38>;
    }

    void Executor::fillCbInstructionSet()
    {
        m_cbInstructionSet[0x00] = &Executor::rlc_r<Registers::B>;
        m_cbInstructionSet[0x01] = &Executor::rlc_r<Registers::C>;
        m_cbInstructionSet[0x02] = &Executor::rlc_r<Registers::D>;
        m_cbInstructionSet[0x03] = &Executor::rlc_r<Registers::E>;
        m_cbInstructionSet[0x04] = &Executor::rlc_r<Registers::H>;
        m_cbInstructionSet[0x05] = &Executor::rlc_r<Registers::L>;
        m_cbInstructionSet[0x06] = &Executor::rlc_HL;
        m_cbInstructionSet[0x07] = &Executor::rlc_r<Registers::A>;
        m_cbInstructionSet[0x08] = &Executor::rrc_r<Registers::B>;
        m_cbInstructionSet[0x09] = &Executor::rrc_r<Registers::C>;
        m_cbInstructionSet[0x0A] = &Executor::rrc_r<Registers::D>;
        m_cbInstructionSet[0x0B] = &Executor::rrc_r<Registers::E>;
        m_cbInstructionSet[0x0C] = &Executor::rrc_r<Registers::H>;
        m_cbInstructionSet[0x0D] = &Executor::rrc_r<Registers::L>;
        m_cbInstructionSet[0x0E] = &Executor::rrc_HL;
        m_cbInstructionSet[0x0F] = &Executor::rrc_r<Registers::A>;
        m_cbInstructionSet[0x10] = &Executor::rl_r<Registers::B>;
        m_cbInstructionSet[0x11] = &Executor::rl_r<Registers::C>;
        m_cbInstructionSet[0x12] = &Executor::rl_r<Registers::D>;
        m_cbInstructionSet[0x13] = &Executor::rl_r<Registers::E>;
        m_cbInstructionSet[0x14] = &Executor::rl_r<Registers::H>;
        m_cbInstructionSet[0x15] = &Executor::rl_r<Registers::L>;
        m_cbInstructionSet[0x16] = &Executor::rl_HL;
        m_cbInstructionSet[0x17] = &Executor::rl_r<Registers::A>;
        m_cbInstructionSet[0x18] = &Executor::rr_r<Registers::B>;
        m_cbInstructionSet[0x19] = &Executor::rr_r<Registers::C>;
        m_cbInstructionSet[0x1A] = &Executor::rr_r<Registers::D>;
        m_cbInstructionSet[0x1B] = &Executor::rr_r<Registers::E>;
        m_cbInstructionSet[0x1C] = &Executor::rr_r<Registers::H>;
        m_cbInstructionSet[0x1D] = &Executor::rr_r<Registers::L>;
        m_cbInstructionSet[0x1E] = &Executor::rr_HL;
        m_cbInstructionSet[0x1F] = &Executor::rr_r<Registers::A>;
        m_cbInstructionSet[0x20] = &Executor::sla_r<Registers::B>;
        m_cbInstructionSet[0x21] = &Executor::sla_r<Registers::C>;
        m_cbInstructionSet[0x22] = &Executor::sla_r<Registers::D>;
        m_cbInstructionSet[0x23] = &Executor::sla_r<Registers::E>;
        m_cbInstructionSet[0x24] = &Executor::sla_r<Registers::H>;
        m_cbInstructionSet[0x25] = &Executor::sla_r<Registers::L>;
        m_cbInstructionSet[0x26] = &Executor::sla_HL;
        m_cbInstructionSet[0x27] = &Executor::sla_r<Registers::A>;
        m_cbInstructionSet[0x28] = &Executor::sra_r<Registers::B>;
        m_cbInstructionSet[0x29] = &Executor::sra_r<Registers::C>;
        m_cbInstructionSet[0x2A] = &Executor::sra_r<Registers::D>;
        m_cbInstructionSet[0x2B] = &Executor::sra_r<Registers::E>;
        m_cbInstructionSet[0x2C] = &Executor::sra_r<Registers::H>;
        m_cbInstructionSet[0x2D] = &Executor::sra_r<Registers::L>;
        m_cbInstructionSet[0x2E] = &Executor::sra_HL;
        m_cbInstructionSet[0x2F] = &Executor::sra_r<Registers::A>;
        m_cbInstructionSet[0x2F] = &Executor::swap_r<Registers::A>;
        m_cbInstructionSet[0x30] = &Executor::swap_r<Registers::B>;
        m_cbInstructionSet[0x31] = &Executor::swap_r<Registers::C>;
        m_cbInstructionSet[0x32] = &Executor::swap_r<Registers::D>;
        m_cbInstructionSet[0x33] = &Executor::swap_r<Registers::E>;
        m_cbInstructionSet[0x34] = &Executor::swap_r<Registers::H>;
        m_cbInstructionSet[0x35] = &Executor::swap_r<Registers::L>;
        m_cbInstructionSet[0x36] = &Executor::swap_HL;
        m_cbInstructionSet[0x37] = &Executor::swap_r<Registers::A>;
        m_cbInstructionSet[0x38] = &Executor::srl_r<Registers::B>;
        m_cbInstructionSet[0x39] = &Executor::srl_r<Registers::C>;
        m_cbInstructionSet[0x3A] = &Executor::srl_r<Registers::D>;
        m_cbInstructionSet[0x3B] = &Executor::srl_r<Registers::E>;
        m_cbInstructionSet[0x3C] = &Executor::srl_r<Registers::H>;
        m_cbInstructionSet[0x3D] = &Executor::srl_r<Registers::L>;
        m_cbInstructionSet[0x3E] = &Executor::srl_HL;
        m_cbInstructionSet[0x3F] = &Executor::srl_r<Registers::A>;
        m_cbInstructionSet[0x40] = &Executor::bit_n_r<0, Registers::B>;
        m_cbInstructionSet[0x41] = &Executor::bit_n_r<0, Registers::C>;
        m_cbInstructionSet[0x42] = &Executor::bit_n_r<0, Registers::D>;
        m_cbInstructionSet[0x43] = &Executor::bit_n_r<0, Registers::E>;
        m_cbInstructionSet[0x44] = &Executor::bit_n_r<0, Registers::H>;
        m_cbInstructionSet[0x45] = &Executor::bit_n_r<0, Registers::L>;
        m_cbInstructionSet[0x46] = &Executor::bit_n_HL<0>;
        m_cbInstructionSet[0x47] = &Executor::bit_n_r<0, Registers::A>;
        m_cbInstructionSet[0x48] = &Executor::bit_n_r<1, Registers::B>;
        m_cbInstructionSet[0x49] = &Executor::bit_n_r<1, Registers::C>;
        m_cbInstructionSet[0x4A] = &Executor::bit_n_r<1, Registers::D>;
        m_cbInstructionSet[0x4B] = &Executor::bit_n_r<1, Registers::E>;
        m_cbInstructionSet[0x4C] = &Executor::bit_n_r<1, Registers::H>;
        m_cbInstructionSet[0x4D] = &Executor::bit_n_r<1, Registers::L>;
        m_cbInstructionSet[0x4E] = &Executor::bit_n_HL<1>;
        m_cbInstructionSet[0x4F] = &Executor::bit_n_r<1, Registers::A>;
        m_cbInstructionSet[0x50] = &Executor::bit_n_r<2, Registers::B>;
        m_cbInstructionSet[0x51] = &Executor::bit_n_r<2, Registers::C>;
        m_cbInstructionSet[0x52] = &Executor::bit_n_r<2, Registers::D>;
        m_cbInstructionSet[0x53] = &Executor::bit_n_r<2, Registers::E>;
        m_cbInstructionSet[0x54] = &Executor::bit_n_r<2, Registers::H>;
        m_cbInstructionSet[0x55] = &Executor::bit_n_r<2, Registers::L>;
        m_cbInstructionSet[0x56] = &Executor::bit_n_HL<2>;
        m_cbInstructionSet[0x57] = &Executor::bit_n_r<2, Registers::A>;
        m_cbInstructionSet[0x58] = &Executor::bit_n_r<3, Registers::B>;
        m_cbInstructionSet[0x59] = &Executor::bit_n_r<3, Registers::C>;
        m_cbInstructionSet[0x5A] = &Executor::bit_n_r<3, Registers::D>;
        m_cbInstructionSet[0x5B] = &Executor::bit_n_r<3, Registers::E>;
        m_cbInstructionSet[0x5C] = &Executor::bit_n_r<3, Registers::H>;
        m_cbInstructionSet[0x5D] = &Executor::bit_n_r<3, Registers::L>;
        m_cbInstructionSet[0x5E] = &Executor::bit_n_HL<3>;
        m_cbInstructionSet[0x5F] = &Executor::bit_n_r<3, Registers::A>;
        m_cbInstructionSet[0x60] = &Executor::bit_n_r<4, Registers::B>;
        m_cbInstructionSet[0x61] = &Executor::bit_n_r<4, Registers::C>;
        m_cbInstructionSet[0x62] = &Executor::bit_n_r<4, Registers::D>;
        m_cbInstructionSet[0x63] = &Executor::bit_n_r<4, Registers::E>;
        m_cbInstructionSet[0x64] = &Executor::bit_n_r<4, Registers::H>;
        m_cbInstructionSet[0x65] = &Executor::bit_n_r<4, Registers::L>;
        m_cbInstructionSet[0x66] = &Executor::bit_n_HL<4>;
        m_cbInstructionSet[0x67] = &Executor::bit_n_r<4, Registers::A>;
        m_cbInstructionSet[0x68] = &Executor::bit_n_r<5, Registers::B>;
        m_cbInstructionSet[0x69] = &Executor::bit_n_r<5, Registers::C>;
        m_cbInstructionSet[0x6A] = &Executor::bit_n_r<5, Registers::D>;
        m_cbInstructionSet[0x6B] = &Executor::bit_n_r<5, Registers::E>;
        m_cbInstructionSet[0x6C] = &Executor::bit_n_r<5, Registers::H>;
        m_cbInstructionSet[0x6D] = &Executor::bit_n_r<5, Registers::L>;
        m_cbInstructionSet[0x6E] = &Executor::bit_n_HL<5>;
        m_cbInstructionSet[0x6F] = &Executor::bit_n_r<6, Registers::A>;
        m_cbInstructionSet[0x70] = &Executor::bit_n_r<6, Registers::B>;
        m_cbInstructionSet[0x71] = &Executor::bit_n_r<6, Registers::C>;
        m_cbInstructionSet[0x72] = &Executor::bit_n_r<6, Registers::D>;
        m_cbInstructionSet[0x73] = &Executor::bit_n_r<6, Registers::E>;
        m_cbInstructionSet[0x74] = &Executor::bit_n_r<6, Registers::H>;
        m_cbInstructionSet[0x75] = &Executor::bit_n_r<6, Registers::L>;
        m_cbInstructionSet[0x76] = &Executor::bit_n_HL<6>;
        m_cbInstructionSet[0x77] = &Executor::bit_n_r<6, Registers::A>;
        m_cbInstructionSet[0x78] = &Executor::bit_n_r<7, Registers::B>;
        m_cbInstructionSet[0x79] = &Executor::bit_n_r<7, Registers::C>;
        m_cbInstructionSet[0x7A] = &Executor::bit_n_r<7, Registers::D>;
        m_cbInstructionSet[0x7B] = &Executor::bit_n_r<7, Registers::E>;
        m_cbInstructionSet[0x7C] = &Executor::bit_n_r<7, Registers::H>;
        m_cbInstructionSet[0x7D] = &Executor::bit_n_r<7, Registers::L>;
        m_cbInstructionSet[0x7E] = &Executor::bit_n_HL<7>;
        m_cbInstructionSet[0x7F] = &Executor::bit_n_r<7, Registers::A>;
        m_cbInstructionSet[0x80] = &Executor::res_n_r<0, Registers::B>;
        m_cbInstructionSet[0x81] = &Executor::res_n_r<0, Registers::C>;
        m_cbInstructionSet[0x82] = &Executor::res_n_r<0, Registers::D>;
        m_cbInstructionSet[0x83] = &Executor::res_n_r<0, Registers::E>;
        m_cbInstructionSet[0x84] = &Executor::res_n_r<0, Registers::H>;
        m_cbInstructionSet[0x85] = &Executor::res_n_r<0, Registers::L>;
        m_cbInstructionSet[0x86] = &Executor::res_n_HL<0>;
        m_cbInstructionSet[0x87] = &Executor::res_n_r<0, Registers::A>;
        m_cbInstructionSet[0x88] = &Executor::res_n_r<1, Registers::B>;
        m_cbInstructionSet[0x89] = &Executor::res_n_r<1, Registers::C>;
        m_cbInstructionSet[0x8A] = &Executor::res_n_r<1, Registers::D>;
        m_cbInstructionSet[0x8B] = &Executor::res_n_r<1, Registers::E>;
        m_cbInstructionSet[0x8C] = &Executor::res_n_r<1, Registers::H>;
        m_cbInstructionSet[0x8D] = &Executor::res_n_r<1, Registers::L>;
        m_cbInstructionSet[0x8E] = &Executor::res_n_HL<1>;
        m_cbInstructionSet[0x8F] = &Executor::res_n_r<1, Registers::A>;
        m_cbInstructionSet[0x90] = &Executor::res_n_r<2, Registers::B>;
        m_cbInstructionSet[0x91] = &Executor::res_n_r<2, Registers::C>;
        m_cbInstructionSet[0x92] = &Executor::res_n_r<2, Registers::D>;
        m_cbInstructionSet[0x93] = &Executor::res_n_r<2, Registers::E>;
        m_cbInstructionSet[0x94] = &Executor::res_n_r<2, Registers::H>;
        m_cbInstructionSet[0x95] = &Executor::res_n_r<2, Registers::L>;
        m_cbInstructionSet[0x96] = &Executor::res_n_HL<2>;
        m_cbInstructionSet[0x97] = &Executor::res_n_r<2, Registers::A>;
        m_cbInstructionSet[0x98] = &Executor::res_n_r<3, Registers::B>;
        m_cbInstructionSet[0x99] = &Executor::res_n_r<3, Registers::C>;
        m_cbInstructionSet[0x9A] = &Executor::res_n_r<3, Registers::D>;
        m_cbInstructionSet[0x9B] = &Executor::res_n_r<3, Registers::E>;
        m_cbInstructionSet[0x9C] = &Executor::res_n_r<3, Registers::H>;
        m_cbInstructionSet[0x9D] = &Executor::res_n_r<3, Registers::L>;
        m_cbInstructionSet[0x9E] = &Executor::res_n_HL<3>;
        m_cbInstructionSet[0x9F] = &Executor::res_n_r<3, Registers::A>;
        m_cbInstructionSet[0xA0] = &Executor::res_n_r<4, Registers::B>;
        m_cbInstructionSet[0xA1] = &Executor::res_n_r<4, Registers::C>;
        m_cbInstructionSet[0xA2] = &Executor::res_n_r<4, Registers::D>;
        m_cbInstructionSet[0xA3] = &Executor::res_n_r<4, Registers::E>;
        m_cbInstructionSet[0xA4] = &Executor::res_n_r<4, Registers::H>;
        m_cbInstructionSet[0xA5] = &Executor::res_n_r<4, Registers::L>;
        m_cbInstructionSet[0xA6] = &Executor::res_n_HL<4>;
        m_cbInstructionSet[0xA7] = &Executor::res_n_r<4, Registers::A>;
        m_cbInstructionSet[0xA8] = &Executor::res_n_r<5, Registers::B>;
        m_cbInstructionSet[0xA9] = &Executor::res_n_r<5, Registers::C>;
        m_cbInstructionSet[0xAA] = &Executor::res_n_r<5, Registers::D>;
        m_cbInstructionSet[0xAB] = &Executor::res_n_r<5, Registers::E>;
        m_cbInstructionSet[0xAC] = &Executor::res_n_r<5, Registers::H>;
        m_cbInstructionSet[0xAD] = &Executor::res_n_r<5, Registers::L>;
        m_cbInstructionSet[0xAE] = &Executor::res_n_HL<5>;
        m_cbInstructionSet[0xAF] = &Executor::res_n_r<6, Registers::A>;
        m_cbInstructionSet[0xB0] = &Executor::res_n_r<6, Registers::B>;
        m_cbInstructionSet[0xB1] = &Executor::res_n_r<6, Registers::C>;
        m_cbInstructionSet[0xB2] = &Executor::res_n_r<6, Registers::D>;
        m_cbInstructionSet[0xB3] = &Executor::res_n_r<6, Registers::E>;
        m_cbInstructionSet[0xB4] = &Executor::res_n_r<6, Registers::H>;
        m_cbInstructionSet[0xB5] = &Executor::res_n_r<6, Registers::L>;
        m_cbInstructionSet[0xB6] = &Executor::res_n_HL<6>;
        m_cbInstructionSet[0xB7] = &Executor::res_n_r<6, Registers::A>;
        m_cbInstructionSet[0xB8] = &Executor::res_n_r<7, Registers::B>;
        m_cbInstructionSet[0xB9] = &Executor::res_n_r<7, Registers::C>;
        m_cbInstructionSet[0xBA] = &Executor::res_n_r<7, Registers::D>;
        m_cbInstructionSet[0xBB] = &Executor::res_n_r<7, Registers::E>;
        m_cbInstructionSet[0xBC] = &Executor::res_n_r<7, Registers::H>;
        m_cbInstructionSet[0xBD] = &Executor::res_n_r<7, Registers::L>;
        m_cbInstructionSet[0xBE] = &Executor::res_n_HL<7>;
        m_cbInstructionSet[0xBF] = &Executor::res_n_r<7, Registers::A>;
        m_cbInstructionSet[0xC0] = &Executor::set_n_r<0, Registers::B>;
        m_cbInstructionSet[0xC1] = &Executor::set_n_r<0, Registers::C>;
        m_cbInstructionSet[0xC2] = &Executor::set_n_r<0, Registers::D>;
        m_cbInstructionSet[0xC3] = &Executor::set_n_r<0, Registers::E>;
        m_cbInstructionSet[0xC4] = &Executor::set_n_r<0, Registers::H>;
        m_cbInstructionSet[0xC5] = &Executor::set_n_r<0, Registers::L>;
        m_cbInstructionSet[0xC6] = &Executor::set_n_HL<0>;
        m_cbInstructionSet[0xC7] = &Executor::set_n_r<0, Registers::A>;
        m_cbInstructionSet[0xC8] = &Executor::set_n_r<1, Registers::B>;
        m_cbInstructionSet[0xC9] = &Executor::set_n_r<1, Registers::C>;
        m_cbInstructionSet[0xCA] = &Executor::set_n_r<1, Registers::D>;
        m_cbInstructionSet[0xCB] = &Executor::set_n_r<1, Registers::E>;
        m_cbInstructionSet[0xCC] = &Executor::set_n_r<1, Registers::H>;
        m_cbInstructionSet[0xCD] = &Executor::set_n_r<1, Registers::L>;
        m_cbInstructionSet[0xCE] = &Executor::set_n_HL<1>;
        m_cbInstructionSet[0xCF] = &Executor::set_n_r<1, Registers::A>;
        m_cbInstructionSet[0xD0] = &Executor::set_n_r<2, Registers::B>;
        m_cbInstructionSet[0xD1] = &Executor::set_n_r<2, Registers::C>;
        m_cbInstructionSet[0xD2] = &Executor::set_n_r<2, Registers::D>;
        m_cbInstructionSet[0xD3] = &Executor::set_n_r<2, Registers::E>;
        m_cbInstructionSet[0xD4] = &Executor::set_n_r<2, Registers::H>;
        m_cbInstructionSet[0xD5] = &Executor::set_n_r<2, Registers::L>;
        m_cbInstructionSet[0xD6] = &Executor::set_n_HL<2>;
        m_cbInstructionSet[0xD7] = &Executor::set_n_r<2, Registers::A>;
        m_cbInstructionSet[0xD8] = &Executor::set_n_r<3, Registers::B>;
        m_cbInstructionSet[0xD9] = &Executor::set_n_r<3, Registers::C>;
        m_cbInstructionSet[0xDA] = &Executor::set_n_r<3, Registers::D>;
        m_cbInstructionSet[0xDB] = &Executor::set_n_r<3, Registers::E>;
        m_cbInstructionSet[0xDC] = &Executor::set_n_r<3, Registers::H>;
        m_cbInstructionSet[0xDD] = &Executor::set_n_r<3, Registers::L>;
        m_cbInstructionSet[0xDE] = &Executor::set_n_HL<3>;
        m_cbInstructionSet[0xDF] = &Executor::set_n_r<3, Registers::A>;
        m_cbInstructionSet[0xE0] = &Executor::set_n_r<4, Registers::B>;
        m_cbInstructionSet[0xE1] = &Executor::set_n_r<4, Registers::C>;
        m_cbInstructionSet[0xE2] = &Executor::set_n_r<4, Registers::D>;
        m_cbInstructionSet[0xE3] = &Executor::set_n_r<4, Registers::E>;
        m_cbInstructionSet[0xE4] = &Executor::set_n_r<4, Registers::H>;
        m_cbInstructionSet[0xE5] = &Executor::set_n_r<4, Registers::L>;
        m_cbInstructionSet[0xE6] = &Executor::set_n_HL<4>;
        m_cbInstructionSet[0xE7] = &Executor::set_n_r<4, Registers::A>;
        m_cbInstructionSet[0xE8] = &Executor::set_n_r<5, Registers::B>;
        m_cbInstructionSet[0xE9] = &Executor::set_n_r<5, Registers::C>;
        m_cbInstructionSet[0xEA] = &Executor::set_n_r<5, Registers::D>;
        m_cbInstructionSet[0xEB] = &Executor::set_n_r<5, Registers::E>;
        m_cbInstructionSet[0xEC] = &Executor::set_n_r<5, Registers::H>;
        m_cbInstructionSet[0xED] = &Executor::set_n_r<5, Registers::L>;
        m_cbInstructionSet[0xEE] = &Executor::set_n_HL<5>;
        m_cbInstructionSet[0xEF] = &Executor::set_n_r<6, Registers::A>;
        m_cbInstructionSet[0xF0] = &Executor::set_n_r<6, Registers::B>;
        m_cbInstructionSet[0xF1] = &Executor::set_n_r<6, Registers::C>;
        m_cbInstructionSet[0xF2] = &Executor::set_n_r<6, Registers::D>;
        m_cbInstructionSet[0xF3] = &Executor::set_n_r<6, Registers::E>;
        m_cbInstructionSet[0xF4] = &Executor::set_n_r<6, Registers::H>;
        m_cbInstructionSet[0xF5] = &Executor::set_n_r<6, Registers::L>;
        m_cbInstructionSet[0xF6] = &Executor::set_n_HL<6>;
        m_cbInstructionSet[0xF7] = &Executor::set_n_r<6, Registers::A>;
        m_cbInstructionSet[0xF8] = &Executor::set_n_r<7, Registers::B>;
        m_cbInstructionSet[0xF9] = &Executor::set_n_r<7, Registers::C>;
        m_cbInstructionSet[0xFA] = &Executor::set_n_r<7, Registers::D>;
        m_cbInstructionSet[0xFB] = &Executor::set_n_r<7, Registers::E>;
        m_cbInstructionSet[0xFC] = &Executor::set_n_r<7, Registers::H>;
        m_cbInstructionSet[0xFD] = &Executor::set_n_r<7, Registers::L>;
        m_cbInstructionSet[0xFE] = &Executor::set_n_HL<7>;
        m_cbInstructionSet[0xFF] = &Executor::set_n_r<7, Registers::A>;
    }

    int Executor::unhandled() 
    {
        return 1;
    }

    uint8_t Executor::getImmediate8()
    {
        uint8_t n = m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();

        return n;
    }

    uint16_t Executor::getImmediate16()
    {
        uint16_t nn = m_memory.read16(m_registers.getPC());
        m_registers.incrementPC();
        m_registers.incrementPC();
        
        return nn;
    }
    
    int Executor::nop()
    {
        return 1;
    }

    int Executor::cb()
    {
        uint8_t opCode = getImmediate8();
        return (this->*m_cbInstructionSet[opCode])();
    }

    int Executor::ld_A_nn()
    {
        uint16_t nn = getImmediate16();

        uint8_t val = m_memory.read8(nn);
        m_registers.write8<Registers::A>(val);
        return 4;
    }

    int Executor::ld_nn_A()
    {
        uint16_t nn = getImmediate16();

        uint8_t a = m_registers.read8<Registers::A>();
        m_memory.write8(nn, a);

        return 4;
    }

    int Executor::ldh_A_aC()
    {
        uint8_t c = m_registers.read8<Registers::C>();
        uint16_t addr = utils::to16(0xFF, c);
        
        uint8_t val = m_memory.read8(addr);
        m_registers.write8<Registers::A>(val);

        return 2;
    }

    int Executor::ldh_aC_A()
    {
        uint8_t c = m_registers.read8<Registers::C>();
        uint16_t addr = utils::to16(0xFF, c);

        uint8_t a = m_registers.read8<Registers::A>();

        m_memory.write8(addr, a);

        return 2;
    }

    int Executor::ldh_A_an()
    {
        uint8_t n = getImmediate8();
        uint16_t addr = utils::to16(0xFF, n);

        uint8_t val = m_memory.read8(addr);
        m_registers.write8<Registers::A>(val);

        return 3;
    }

    int Executor::ldh_an_A()
    {
        uint8_t n = getImmediate8();
        uint16_t addr = utils::to16(0xFF, n);

        uint8_t a = m_registers.read8<Registers::A>();

        m_memory.write8(addr, a);
        return 3;
    }

    int Executor::ld_HLd_A()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);

        m_registers.write16<Registers::HL>(--hl);
        m_registers.write8<Registers::A>(val);

        return 2;

    }
        
    int Executor::ld_A_HLd()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t a = m_registers.read8<Registers::A>();
        m_memory.write8(hl, a);

        m_registers.write16<Registers::HL>(--hl);

        return 2;
    }
        
    int Executor::ld_HLi_A()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);

        m_registers.write16<Registers::HL>(++hl);
        m_registers.write8<Registers::A>(val);

        return 2;
    }
    
    int Executor::ld_A_HLi()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t a = m_registers.read8<Registers::A>();
        m_memory.write8(hl, a);

        m_registers.write16<Registers::HL>(++hl);

        return 2;
    }

    int Executor::ld_SP_rr()
    {
        uint16_t val = getImmediate16();

        m_registers.setSP(val);

        return 3;
    }

    int Executor::ld_nn_SP()
    {
        uint16_t addr = getImmediate16();

        m_memory.write16(addr, m_registers.getSP());

        return 5;
    }

    int Executor::ld_SP_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        m_registers.setSP(hl);

        return 2;
    }

    int Executor::ld_HL_SP_r8()
    {
        int8_t val = (int8_t) getImmediate8();
        
        uint16_t result = m_registers.getSP() + val;

        m_registers.write16<Registers::HL>((uint16_t) result);

        m_registers.resetFlags();

        bool hc = ((val ^ m_registers.getSP() ^ result) & 0x10) == 0x10;
        bool c = ((val ^ m_registers.getSP() ^ result) & 0x100) == 0x100;
        m_registers.setFlag(Registers::Flag::H, hc);
        m_registers.setFlag(Registers::Flag::C, c);

        return 3;
    }

    void Executor::push(uint16_t val)
    {
        uint16_t sp = m_registers.getSP();
        sp--;
        sp--;

        m_registers.setSP(sp);
        m_memory.write16(m_registers.getSP(), val);
    }

    uint16_t Executor::pop()
    {
        uint16_t sp = m_registers.getSP();
        uint16_t val = m_memory.read16(sp);

        sp++;
        sp++;
        m_registers.setSP(sp);

        return val;
    }

    void Executor::add(int a, int b)
    {
        int res = a + b;
        int carryBits = a ^ b ^ res;

        m_registers.write8<Registers::A>(res);
        updateFlagsWithCarry8bit(carryBits, res, true, false);
    }

    int Executor::add_HL()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();
        
        uint16_t hl = m_registers.read16<Registers::HL>();
        int b = (int8_t) m_memory.read8(hl);
        add(a, b);

        return 2;
    }

    int Executor::add_n()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();
        int b = (int8_t) getImmediate8();
        add(a, b);

        return 2;
    }
    
    void Executor::adc(int a, int b)
    {
        int carryFlag = m_registers.isSetFlag(Registers::Flag::C) ? 1 : 0;
        int res = a + b + carryFlag;
        int carryBits = a ^ b ^ carryFlag ^ res;

        m_registers.write8<Registers::A>(res);
        updateFlagsWithCarry8bit(carryBits, res, true, false);
    }

    int Executor::adc_HL()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        uint16_t hl = m_registers.read16<Registers::HL>();
        int b = (int8_t)m_memory.read8(hl);
        adc(a, b);

        return 2;
    }

    int Executor::adc_n()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        int b = (int8_t) getImmediate8();
        adc(a, b);

        return 2;
    }

    void Executor::sub(int a, int b)
    {
        int res = a - b;
        int carryBits = a ^ b ^ res;

        m_registers.write8<Registers::A>(res);
        updateFlagsWithCarry8bit(carryBits, res, true, true);
    }

    int Executor::sub_HL()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        uint16_t hl = m_registers.read16<Registers::HL>();
        int b = (int8_t)m_memory.read8(hl);
        sub(a, b);

        return 2;
    }

    int Executor::sub_n()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        int b = (int8_t) getImmediate8();
        sub(a, b);

        return 2;
    }

    void Executor::sbc(int a, int b)
    {
        int carryFlag = m_registers.isSetFlag(Registers::Flag::C) ? 1 : 0;
        int res = a - b - carryFlag;
        int carryBits = a ^ b ^ carryFlag ^ res;

        m_registers.write8<Registers::A>(res);
        updateFlagsWithCarry8bit(carryBits, res, true, true);
    }

    int Executor::sbc_HL()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        uint16_t hl = m_registers.read16<Registers::HL>();
        int b = (int8_t)m_memory.read8(hl);
        sbc(a, b);

        return 2;
    }

    int Executor::sbc_n()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();

        int b = (int8_t) getImmediate8();
        sbc(a, b);

        return 2;
    }

    void Executor::cp(int a, int b)
    {
        int res = a - b;
        int carryBits = a ^ b ^ res;

        updateFlagsWithCarry8bit(carryBits, res, true, true);
    }

    int Executor::cp_HL()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        uint16_t hl = m_registers.read16<Registers::HL>();
        int b = (int8_t)m_memory.read8(hl);
        cp(a, b);

        return 2;
    }

    int Executor::cp_n()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();

        int b = (int8_t) getImmediate8();
        cp(a, b);

        return 2;
    }

    int Executor::inc_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        int val = (int8_t) m_memory.read8(hl);

        int8_t r = val + 1;
        m_memory.write8(hl, r);

        int carryBits = val ^ 1 ^ r;
        updateFlagsWithCarry8bit(carryBits, r, false, false);

        return 3;
    }

    int Executor::dec_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        int val = (int8_t)m_memory.read8(hl);

        int8_t r = val - 1;
        m_memory.write8(hl, r);

        int carryBits = val ^ 1 ^ r;
        updateFlagsWithCarry8bit(carryBits, r, false, false);

        return 3;
    }

    int Executor::and_HL()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t b = m_memory.read8(hl);
        uint8_t r = a & b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, true, false);

        return 2;
    }

    int Executor::and_n()
    {
        uint8_t a = m_registers.read8<Registers::A>();

        uint8_t b = getImmediate8();
        uint8_t r = a & b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, true, false);
        
        return 2;
    }

    int Executor::or_HL()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t b = m_memory.read8(hl);
        uint8_t r = a | b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, false, false);

        return 2;
    }

    int Executor::or_n()
    {
        uint8_t a = m_registers.read8<Registers::A>();

        uint8_t b = getImmediate8();
        uint8_t r = a | b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, false, false);

        return 2;
    }

    int Executor::xor_HL()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t b = m_memory.read8(hl);
        uint8_t r = a ^ b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, false, false);

        return 2;
    }

    int Executor::xor_n()
    {
        uint8_t a = m_registers.read8<Registers::A>();

        uint8_t b = getImmediate8();
        uint8_t r = a ^ b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, false, false);

        return 2;
    }

    int Executor::scf()
    {
        m_registers.setFlag(Registers::Flag::N, false);
        m_registers.setFlag(Registers::Flag::H, false);
        m_registers.setFlag(Registers::Flag::C, true);

        return 1;
    }

    int Executor::ccf()
    {
        m_registers.setFlag(Registers::Flag::N, false);
        m_registers.setFlag(Registers::Flag::H, false);

        bool c = m_registers.isSetFlag(Registers::Flag::C);
        m_registers.setFlag(Registers::Flag::C, !c);

        return 1;
    }

    int Executor::cpl()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        m_registers.write8<Registers::A>(~a);

        m_registers.setFlag(Registers::Flag::N, true);
        m_registers.setFlag(Registers::Flag::H, true);

        return 1;
    }

    int Executor::daa()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        bool isSub = m_registers.isSetFlag(Registers::Flag::N);

        if ( (a&0x0F) > 9 || m_registers.isSetFlag(Registers::Flag::H) )
        {
            isSub ? a -= 0x06 : a += 0x06;
        }

        if ((a & 0xF0) > 0x90 || m_registers.isSetFlag(Registers::Flag::C))
        {
            isSub ? a -= 0x60 : a += 0x60;
            m_registers.setFlag(Registers::Flag::C, true);
        }

        if (a == 0)
        {
            m_registers.setFlag(Registers::Flag::Z, true);
        }
        m_registers.setFlag(Registers::Flag::H, false);

        m_registers.write8<Registers::A>(a);

        return 1;
    }

    int Executor::add_HL_SP()
    {
        int hl = m_registers.read16<Registers::HL>();
        int sp = m_registers.getSP();
        int res = hl + sp;

        int carryBits = hl ^ sp ^ res;
        updateFlagsWithCarry16bit(carryBits, true, false);

        m_registers.write16<Registers::HL>((uint16_t)res);

        return 2;
    }

    int Executor::add_SP_n()
    {
        int sp = m_registers.getSP();
        int8_t n = (int8_t) getImmediate8();

        int res = sp + n;

        int carryBits = sp ^ n ^ res;
        m_registers.setFlag(Registers::Flag::Z, false);
        updateFlagsWithCarry16bit(carryBits, true, false);

        m_registers.setSP((uint16_t)res);

        return 4;
    }

    int Executor::inc_SP()
    {
        uint16_t rr = m_registers.getSP();
        rr++;
        m_registers.setSP(rr);

        return 2;
    }

    int Executor::dec_SP()
    {
        uint16_t rr = m_registers.getSP();
        rr--;
        m_registers.setSP(rr);

        return 2;
    }

    int Executor::rlca()
    {
        rlc_r<Registers::A>();

        return 1;
    }

    int Executor::rlc_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        bool c = (hl & 0x8000) == 0x8000;

        hl <<= 1;
        if (c)
        {
            hl |= 1;
        }

        updateFlags(hl, false, false, c);

        m_registers.write16<Registers::HL>(hl);

        return 4;
    }

    int Executor::rla()
    {
        rl_r<Registers::A>();

        return 1;
    }

    int Executor::rl_HL()
    {
        uint16_t r = m_registers.read16<Registers::HL>();
        bool c = (r & 0x8000) == 0x8000;
        uint16_t oldCarry = m_registers.isSetFlag(Registers::Flag::C) ? 1 : 0;
        r <<= 1;
        r |= oldCarry;

        updateFlags(r, false, false, c);

        m_registers.write16<Registers::HL>(r);

        return 4;
    }

    int Executor::rrca()
    {
        rrc_r<Registers::A>();

        return 1;
    }
    int Executor::rrc_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        bool c = (hl & 1) == 1;

        hl >>= 1;
        if (c)
        {
            hl |= 0x8000;
        }

        updateFlags(hl, false, false, c);

        m_registers.write16<Registers::HL>(hl);

        return 4;
    }


    int Executor::rra()
    {
        rr_r<Registers::A>();

        return 1;
    }
    
    int Executor::rr_HL()
    {
        uint16_t r = m_registers.read16<Registers::HL>();
        bool c = (r & 1) == 1;
        uint16_t oldCarry = m_registers.isSetFlag(Registers::Flag::C) ? 0X8000 : 0;
        r >>= 1;
        r |= oldCarry;

        updateFlags(r, false, false, c);

        m_registers.write16<Registers::HL>(r);

        return 4;
    }

    int Executor::sla_HL()
    {
        uint16_t r = m_registers.read16<Registers::HL>();
        bool c = (r & 0x8000) == 0x8000;
        r <<= 1;

        updateFlags(r, false, false, c);
        m_registers.write16<Registers::HL>(r);

        return 4;
    }

    int Executor::sra_HL()
    {
        uint16_t r = m_registers.read16<Registers::HL>();
        bool c = (r & 1) == 1;
        if ((r & 0x8000) != 0)
        {
            r >>= 1;
            r |= 0x80;
        }
        else
        {
            r >>= 1;
        }
        
        updateFlags(r, false, false, c);
        m_registers.write16<Registers::HL>(r);

        return 4;
    }

    int Executor::srl_HL()
    {
        uint16_t r = m_registers.read16<Registers::HL>();
        bool c = (r & 1) == 1;
        r >>= 1;

        updateFlags(r, false, false, c);
        m_registers.write16<Registers::HL>(r);

        return 4;
    }

    int Executor::swap_HL()
    {
        uint16_t r = m_registers.read16<Registers::HL>();
        r = (r << 8) | (r >> 8);

        updateFlags(r, false, false, false);
        m_registers.write16<Registers::HL>(r);

        return 4;
    }

    void Executor::bit_n(uint8_t n, uint16_t r)
    {
        uint16_t b = 1 << n;
        bool isSet = (b | r) == b;

        m_registers.setFlag(Registers::Flag::Z, isSet);
        m_registers.setFlag(Registers::Flag::N, 0);
        m_registers.setFlag(Registers::Flag::H, 1);
    }

    int Executor::jp_nn()
    {
        uint16_t immediate = getImmediate16();
        m_registers.setPC(immediate);

        return 4;
    }

    int Executor::jp_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        m_registers.setPC(hl);

        return 1;
    }

    int Executor::jr_e()
    {
        int8_t e = (int8_t) getImmediate8();
        uint16_t pc = m_registers.getPC();
        m_registers.setPC(pc + e);

        return 3;
    }

    int Executor::call_nn()
    {
        uint16_t addr = getImmediate16();
        
        uint16_t pc = m_registers.getPC();
        push(pc);

        m_registers.setPC(addr);
        return 6;
    }

    int Executor::ret()
    {
        uint16_t addr = pop();
        m_registers.setPC(addr);

        return 4;
    }

    int Executor::reti()
    {
        m_IME = true;
        return ret();
    }

    int Executor::di()
    {
        m_IME = false;
        return 1;
    }

    int Executor::ei()
    {
        m_IME = true;
        return 1;
    }

    int Executor::halt()
    {
        m_isHalt = true;
        return 1;
    }

    int Executor::stop()
    {
        getImmediate8();
        m_isStopped = true;
        return 1;
    }
}