#pragma once

#include "utils/global.h"

#include "registery.h"

#include <string>

class Memory;

namespace cpu
{
    class Registers;

    class Logger
    {
    public:
        using Disassembly = std::string(Logger::*)(uint8_t);

        Logger() = delete;
        Logger(Registers& regist, Memory& mem);

        std::string operator()(uint8_t opCode)
        {
            return ((this->*m_instructionSet[opCode]))(opCode);
        }
    private:
        void fillInstructionSet();
        void fillCbInstructionSet();

        std::string unhandled(uint8_t opcode);

        // 8bit Loads
        template<uint8_t opcode>
        std::string ld_r_r_8(uint8_t opCode);
        template<uint8_t opcode>
        std::string ld_r_n_8(uint8_t opCode);
        template<uint8_t opcode>
        std::string ld_HL_n_8(uint8_t opCode);
        template<uint8_t opcode>
        std::string ld_r_HL(uint8_t opCode);
        template<uint8_t opcode>
        std::string ld_HL_r(uint8_t opCode);
        template<Registers::Paired NAME>
        std::string ld_A_r16(uint8_t opCode);
        template<Registers::Paired NAME>
        std::string ld_r16_A(uint8_t opCode);
        std::string ld_A_nn(uint8_t opCode);
        std::string ld_nn_A(uint8_t opCode);
        std::string ldh_A_aC(uint8_t opCode);
        std::string ldh_aC_A(uint8_t opCode);
        std::string ldh_A_an(uint8_t opCode);
        std::string ldh_an_A(uint8_t opCode);
        std::string ld_HLd_A(uint8_t opCode);
        std::string ld_A_HLd(uint8_t opCode);
        std::string ld_HLi_A(uint8_t opCode);
        std::string ld_A_HLi(uint8_t opCode);

        // 16bits loads
        template<Registers::Paired NAME>
        std::string ld_rr_nn(uint8_t opCode);
        std::string ld_SP_rr(uint8_t opCode);
        std::string ld_nn_SP(uint8_t opCode);
        std::string ld_SP_HL(uint8_t opCode);
        std::string ld_HL_SP_r8(uint8_t opCode);

        template<Registers::Paired NAME>
        std::string push(uint8_t opCode);
        template<Registers::Paired NAME>
        std::string pop(uint8_t opCode);

        // 8-bit arithmetic and logical instructions
        template<Registers::Names NAME>
        std::string add_r(uint8_t opCode);
        std::string add_HL(uint8_t opCode);
        std::string add_n(uint8_t opCode);

        template<Registers::Names NAME>
        std::string adc_r(uint8_t opCode);
        std::string adc_HL(uint8_t opCode);
        std::string adc_n(uint8_t opCode);

        template<Registers::Names NAME>
        std::string sub_r(uint8_t opCode);
        std::string sub_HL(uint8_t opCode);
        std::string sub_n(uint8_t opCode);
        
        template<Registers::Names NAME>
        std::string sbc_r(uint8_t opCode);
        std::string sbc_HL(uint8_t opCode);
        std::string sbc_n(uint8_t opCode);

        template<Registers::Names NAME>
        std::string cp_r(uint8_t opCode);
        std::string cp_HL(uint8_t opCode);
        std::string cp_n(uint8_t opCode);

        template<Registers::Names NAME>
        std::string inc_r(uint8_t opCode);
        std::string inc_HL(uint8_t opCode);

        template<Registers::Names NAME>
        std::string dec_r(uint8_t opCode);
        std::string dec_HL(uint8_t opCode);

        template<Registers::Names NAME>
        std::string and_r(uint8_t opCode);
        std::string and_HL(uint8_t opCode);
        std::string and_n(uint8_t opCode);

        template<Registers::Names NAME>
        std::string or_r(uint8_t opCode);
        std::string or_HL(uint8_t opCode);
        std::string or_n(uint8_t opCode);

        template<Registers::Names NAME>
        std::string xor_r(uint8_t opCode);
        std::string xor_HL(uint8_t opCode);
        std::string xor_n(uint8_t opCode);

        std::string scf(uint8_t opCode);
        std::string ccf(uint8_t opCode);
        std::string cpl(uint8_t opCode);
        std::string daa(uint8_t opCode);

        // 16-bit arithmetic
        template<Registers::Paired NAME>
        std::string add_HL_rr(uint8_t opCode);
        std::string add_HL_SP(uint8_t opCode);
        std::string add_SP_n(uint8_t opCode);
        template<Registers::Paired NAME>
        std::string inc_rr(uint8_t opCode);
        std::string inc_SP(uint8_t opCode);
        template<Registers::Paired NAME>
        std::string dec_rr(uint8_t opCode);
        std::string dec_SP(uint8_t opCode);

        // Rotates and shifts
        std::string rlca(uint8_t opCode);
        std::string rla(uint8_t opCode);
        std::string rrca(uint8_t opCode);
        std::string rra(uint8_t opCode);

        // Control flow instructions
        std::string jp_nn(uint8_t opCode);
        std::string jp_HL(uint8_t opCode);
        std::string jp_nz_nn(uint8_t opCode);
        std::string jp_z_nn(uint8_t opCode);
        std::string jp_nc_nn(uint8_t opCode);
        std::string jp_c_nn(uint8_t opCode);
    private:
        Registers& m_registers;
        //Memory& m_memory;

        Disassembly m_instructionSet[255];
        Disassembly m_cbInstructionSet[255];
    };
}

#include "logger-impl.hpp"
