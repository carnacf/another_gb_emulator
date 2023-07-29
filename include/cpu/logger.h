#pragma once

#include "utils/global.h"

#include "registery.h"

class Memory;

namespace cpu
{
    class Registers;

    class Logger
    {
    public:
        using Disassembly = void (Logger::*)(uint8_t);

        Logger() = delete;
        Logger(Registers& regist, Memory& mem);

        void operator()(uint8_t opCode)
        {
            return ((this->*m_instructionSet[opCode]))(opCode);
        }
    private:
        void fillInstructionSet();
        void fillCbInstructionSet();

        void unhandled(uint8_t opcode);

        void nop(uint8_t opcode);
        void cb(uint8_t opcode);
        // 8bit Loads
        template<uint8_t opcode>
        void ld_r_r_8(uint8_t opCode);
        template<uint8_t opcode>
        void ld_r_n_8(uint8_t opCode);
        void ld_HL_n_8(uint8_t opCode);
        template<uint8_t opcode>
        void ld_r_HL(uint8_t opCode);
        template<uint8_t opcode>
        void ld_HL_r(uint8_t opCode);
        template<Registers::Paired NAME>
        void ld_A_r16(uint8_t opCode);
        template<Registers::Paired NAME>
        void ld_r16_A(uint8_t opCode);
        void ld_A_nn(uint8_t opCode);
        void ld_nn_A(uint8_t opCode);
        void ldh_A_aC(uint8_t opCode);
        void ldh_aC_A(uint8_t opCode);
        void ldh_A_an(uint8_t opCode);
        void ldh_an_A(uint8_t opCode);
        void ld_HLd_A(uint8_t opCode);
        void ld_A_HLd(uint8_t opCode);
        void ld_HLi_A(uint8_t opCode);
        void ld_A_HLi(uint8_t opCode);

        // 16bits loads
        template<Registers::Paired NAME>
        void ld_rr_nn(uint8_t opCode);
        void ld_SP_nn(uint8_t opCode);
        void ld_nn_SP(uint8_t opCode);
        void ld_SP_HL(uint8_t opCode);
        void ld_HL_SP_r8(uint8_t opCode);

        template<Registers::Paired NAME>
        void push(uint8_t opCode);
        template<Registers::Paired NAME>
        void pop(uint8_t opCode);

        // 8-bit arithmetic and logical instructions
        template<Registers::Names NAME>
        void add_r(uint8_t opCode);
        void add_HL(uint8_t opCode);
        void add_n(uint8_t opCode);

        template<Registers::Names NAME>
        void adc_r(uint8_t opCode);
        void adc_HL(uint8_t opCode);
        void adc_n(uint8_t opCode);

        template<Registers::Names NAME>
        void sub_r(uint8_t opCode);
        void sub_HL(uint8_t opCode);
        void sub_n(uint8_t opCode);
        
        template<Registers::Names NAME>
        void sbc_r(uint8_t opCode);
        void sbc_HL(uint8_t opCode);
        void sbc_n(uint8_t opCode);

        template<Registers::Names NAME>
        void cp_r(uint8_t opCode);
        void cp_HL(uint8_t opCode);
        void cp_n(uint8_t opCode);

        template<Registers::Names NAME>
        void inc_r(uint8_t opCode);
        void inc_HL(uint8_t opCode);

        template<Registers::Names NAME>
        void dec_r(uint8_t opCode);
        void dec_HL(uint8_t opCode);

        template<Registers::Names NAME>
        void and_r(uint8_t opCode);
        void and_HL(uint8_t opCode);
        void and_n(uint8_t opCode);

        template<Registers::Names NAME>
        void or_r(uint8_t opCode);
        void or_HL(uint8_t opCode);
        void or_n(uint8_t opCode);

        template<Registers::Names NAME>
        void xor_r(uint8_t opCode);
        void xor_HL(uint8_t opCode);
        void xor_n(uint8_t opCode);

        void scf(uint8_t opCode);
        void ccf(uint8_t opCode);
        void cpl(uint8_t opCode);
        void daa(uint8_t opCode);

        // 16-bit arithmetic
        template<Registers::Paired NAME>
        void add_HL_rr(uint8_t opCode);
        void add_HL_SP(uint8_t opCode);
        void add_SP_n(uint8_t opCode);
        template<Registers::Paired NAME>
        void inc_rr(uint8_t opCode);
        void inc_SP(uint8_t opCode);
        template<Registers::Paired NAME>
        void dec_rr(uint8_t opCode);
        void dec_SP(uint8_t opCode);

        // Rotates and shifts
        void rlca(uint8_t opCode);
        template<Registers::Names NAME>
        void rlc_r(uint8_t opCode);
        void rlc_HL(uint8_t opCode);

        void rla(uint8_t opCode);
        template<Registers::Names NAME>
        void rl_r(uint8_t opCode);
        void rl_HL(uint8_t opCode);

        void rrca(uint8_t opCode);
        template<Registers::Names NAME>
        void rrc_r(uint8_t opCode);
        void rrc_HL(uint8_t opCode);

        void rra(uint8_t opCode);
        template<Registers::Names NAME>
        void rr_r(uint8_t opCode);
        void rr_HL(uint8_t opCode);

        template<Registers::Names NAME>
        void sla_r(uint8_t opCode);
        void sla_HL(uint8_t opCode);

        template<Registers::Names NAME>
        void sra_r(uint8_t opCode);
        void sra_HL(uint8_t opCode);

        template<Registers::Names NAME>
        void swap_r(uint8_t opCode);
        void swap_HL(uint8_t opCode);

        template<Registers::Names NAME>
        void srl_r(uint8_t opCode);
        void srl_HL(uint8_t opCode);

        // Single - bit Operation instructions
        template<uint8_t n, Registers::Names NAME>
        void bit_n_r(uint8_t opCode);
        template<uint8_t n>
        void bit_n_HL(uint8_t opCode);

        template<uint8_t n, Registers::Names NAME>
        void res_n_r(uint8_t opCode);
        template<uint8_t n>
        void res_n_HL(uint8_t opCode);

        template<uint8_t n, Registers::Names NAME>
        void set_n_r(uint8_t opCode);
        template<uint8_t n>
        void set_n_HL(uint8_t opCode);

        // Control flow instructions
        void jp_nn(uint8_t opCode);
        void jp_HL(uint8_t opCode);
        void jp_nz_nn(uint8_t opCode);
        void jp_z_nn(uint8_t opCode);
        void jp_nc_nn(uint8_t opCode);
        void jp_c_nn(uint8_t opCode);
        
        void jr_e(uint8_t opCode);
        void jr_nz_n(uint8_t opCode);
        void jr_z_n(uint8_t opCode);
        void jr_nc_n(uint8_t opCode);
        void jr_c_n(uint8_t opCode);

        void call_nn(uint8_t opCode);
        void call_nz_nn(uint8_t opCode);
        void call_z_nn(uint8_t opCode);
        void call_nc_nn(uint8_t opCode);
        void call_c_nn(uint8_t opCode);

        void ret(uint8_t opCode);
        void ret_nz(uint8_t opCode);
        void ret_z(uint8_t opCode);
        void ret_nc(uint8_t opCode);
        void ret_c(uint8_t opCode);
        void reti(uint8_t opCode);

        template<uint8_t n>
        void rst(uint8_t opCode);

        void di(uint8_t opCode);
        void ei(uint8_t opCode);
        void halt(uint8_t opCode);
        void stop(uint8_t opCode);
    private:
        Registers& m_registers;
        Memory& m_memory;

        Disassembly m_instructionSet[256];
        Disassembly m_cbInstructionSet[256];
    };
}

#include "logger-impl.hpp"
