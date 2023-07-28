#include "processor.h"

#include "memory.h"
#include "registery.h"

#include <utility>
#include <limits>
#include <iostream>
#include <chrono>
#include <thread>

namespace cpu
{
    constexpr size_t cpu_frequency = 4'194'304; // Hz
    constexpr size_t DIV_frequency = 16'384; // Hz
    constexpr std::chrono::nanoseconds cycle_duration = std::chrono::nanoseconds(1'000'000'000 / cpu_frequency);

    template<int frequency>
    constexpr int number_of_cycle()
    {
        return  cpu_frequency / frequency;
    }

    Processor::Processor(Registers& regist, Memory& mem): 
        m_tracer(regist, mem),
        m_registers(regist), 
        m_memory(mem)
    {
        fillInstructionSet();
        fillCbInstructionSet();
    }
    
    void Processor::runNextInstruction(bool trace)
    {
        auto start = std::chrono::system_clock::now();
        std::optional<Interrupt> interrupt = pendingInterrupt();
        if (m_IME && interrupt.has_value())
        {
            handleInterrupt(interrupt.value());
        }

        uint8_t opCode = m_memory.read8(m_registers.getPC());
        if (trace)
        {
            std::cout << "PC=" << m_registers.getPC() << " : " << m_tracer(opCode);
        }

        m_registers.incrementPC();
        int numberOfCycles = (this->*m_instructionSet[opCode])();
        updateClocks(numberOfCycles);

        auto end = std::chrono::system_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        auto waitTime = std::max((duration - cycle_duration*numberOfCycles).count(), 0ll);
        //std::this_thread::sleep_for(std::chrono::microseconds(waitTime));
    }

    std::optional<int> Processor::getTIMANbCycles() const
    {
        uint8_t tac_flag = m_memory.read8(0xFF07);
        if (tac_flag & 0x04)
        {
            return std::nullopt;
        }

        switch (tac_flag & 0x03)
        {
        case 0x00:
            return cpu_frequency / 1024;
        case 0x01:
            return cpu_frequency / 16;
        case 0x02:
            return cpu_frequency / 64;
        case 0x03:
            return cpu_frequency / 256;
        }

        return std::nullopt;
    }

    void Processor::handleInterrupt(Interrupt interruptType)
    {
        m_IME = false;
        push(m_registers.getPC());

        uint8_t ifFlag = m_memory.read8(0xFF0F);
        switch (interruptType)
        {
        case Interrupt::VBlank:
            m_memory.write8(0xFF0F, ifFlag & ~0x01);
            m_registers.setPC(0x40);
            break;
        case Interrupt::LCD_STAT:
            m_memory.write8(0xFF0F, ifFlag & ~0x02);
            m_registers.setPC(0x48);
            break;
        case Interrupt::Timer:
            m_memory.write8(0xFF0F, ifFlag & ~0x04);
            m_registers.setPC(0x50);
            break;
        case Interrupt::Serial:
            m_memory.write8(0xFF0F, ifFlag & ~0x08);
            m_registers.setPC(0x58);
            break;
        case Interrupt::Joypad:
            m_memory.write8(0xFF0F, ifFlag & ~0x10);
            m_registers.setPC(0x60);
            break;
        }
    }
    
    void Processor::updateClocks(int ticks)
    {
        m_divCycleCounter += ticks;
        constexpr auto div_nb_cycles = number_of_cycle<DIV_frequency>();
        if (m_divCycleCounter >= div_nb_cycles)
        {
            m_memory.incrementDIV();
            m_divCycleCounter = m_divCycleCounter % div_nb_cycles;
        }

        std::optional<int> timaNbCycles = getTIMANbCycles();
        if (timaNbCycles.has_value())
        {
            m_timaCycleCounter += ticks;
            if (m_timaCycleCounter >= timaNbCycles.value())
            {
                m_timaCycleCounter = m_timaCycleCounter % div_nb_cycles;
                uint8_t tima = m_memory.read8(0xFF05);
                m_memory.write8(0xFF05, tima + 1);
            }
        }
    }

    std::optional<Interrupt> Processor::pendingInterrupt() const
    {
        if (m_IME == false)
        {
            return std::nullopt;
        }
        uint8_t if_flag = m_memory.read8(0xFF0F);
        uint8_t ie_flag = m_memory.read8(0xFFFF);
        if_flag = if_flag & ie_flag;
        if ((if_flag & 0x01) == 0x01)
        {
            return Interrupt::VBlank;
        }
        if ((if_flag & 0x02) == 0x02)
        {
            return Interrupt::LCD_STAT;
        }
        if ((if_flag & 0x04) == 0x04)
        {
            return Interrupt::Timer;
        }
        if ((if_flag & 0x08) == 0x08)
        {
            return Interrupt::Serial;
        }
        if ((if_flag & 0x10) == 0x10)
        {
            return Interrupt::Joypad;
        }

        return std::nullopt;
    }

    void Processor::fillInstructionSet()
    {
        std::fill_n(m_instructionSet, 256, &Processor::unhandled);
        
        m_instructionSet[0x00] = &Processor::nop;
        m_instructionSet[0x01] = &Processor::ld_rr_nn<Registers::BC>;
        m_instructionSet[0x02] = &Processor::ld_r16_A<Registers::BC>;
        m_instructionSet[0x03] = &Processor::inc_rr<Registers::BC>;
        m_instructionSet[0x04] = &Processor::inc_r<Registers::B>;
        m_instructionSet[0x05] = &Processor::dec_r<Registers::B>;
        m_instructionSet[0x06] = &Processor::ld_r_n_8<0x06>;
        m_instructionSet[0x07] = &Processor::rlca;
        m_instructionSet[0x08] = &Processor::ld_nn_SP;
        m_instructionSet[0x09] = &Processor::add_HL_rr<Registers::BC>;
        m_instructionSet[0x0A] = &Processor::ld_A_r16<Registers::BC>;
        m_instructionSet[0x0B] = &Processor::dec_rr<Registers::BC>;
        m_instructionSet[0x0C] = &Processor::inc_r<Registers::C>;
        m_instructionSet[0x0D] = &Processor::dec_r<Registers::C>;
        m_instructionSet[0x0E] = &Processor::ld_r_n_8<0x0E>;
        m_instructionSet[0x0F] = &Processor::rrca;
        m_instructionSet[0x10] = &Processor::stop;
        m_instructionSet[0x11] = &Processor::ld_rr_nn<Registers::DE>;
        m_instructionSet[0x12] = &Processor::ld_r16_A<Registers::DE>;
        m_instructionSet[0x13] = &Processor::inc_rr<Registers::DE>;
        m_instructionSet[0x14] = &Processor::inc_r<Registers::D>;
        m_instructionSet[0x15] = &Processor::dec_r<Registers::D>;
        m_instructionSet[0x16] = &Processor::ld_r_n_8<0x16>;
        m_instructionSet[0x17] = &Processor::rla;
        m_instructionSet[0x18] = &Processor::jr_e;
        m_instructionSet[0x19] = &Processor::add_HL_rr<Registers::DE>;
        m_instructionSet[0x1A] = &Processor::ld_A_r16<Registers::DE>;
        m_instructionSet[0x1B] = &Processor::dec_rr<Registers::DE>;
        m_instructionSet[0x1C] = &Processor::inc_r<Registers::E>;
        m_instructionSet[0x1D] = &Processor::dec_r<Registers::E>;
        m_instructionSet[0x1E] = &Processor::ld_r_n_8<0x1E>;
        m_instructionSet[0x1F] = &Processor::rra;
        m_instructionSet[0x20] = &Processor::jr_ncc_n<Registers::Flag::Z>;
        m_instructionSet[0x21] = &Processor::ld_rr_nn<Registers::HL>;
        m_instructionSet[0x22] = &Processor::ld_HLi_A;
        m_instructionSet[0x23] = &Processor::inc_rr<Registers::HL>;
        m_instructionSet[0x24] = &Processor::inc_r<Registers::H>;
        m_instructionSet[0x25] = &Processor::dec_r<Registers::H>;
        m_instructionSet[0x26] = &Processor::ld_r_n_8<0x26>;
        m_instructionSet[0x27] = &Processor::daa;
        m_instructionSet[0x28] = &Processor::jr_cc_n<Registers::Flag::Z>;
        m_instructionSet[0x29] = &Processor::add_HL_rr<Registers::HL>;
        m_instructionSet[0x2A] = &Processor::ld_A_HLi;
        m_instructionSet[0x2B] = &Processor::dec_rr<Registers::HL>;
        m_instructionSet[0x2C] = &Processor::inc_r<Registers::L>;
        m_instructionSet[0x2D] = &Processor::dec_r<Registers::L>;
        m_instructionSet[0x2E] = &Processor::ld_r_n_8<0x2E>;
        m_instructionSet[0x2F] = &Processor::cpl;
        m_instructionSet[0x30] = &Processor::jr_ncc_n<Registers::Flag::C>;
        m_instructionSet[0x31] = &Processor::ld_SP_nn;
        m_instructionSet[0x32] = &Processor::ld_HLd_A;
        m_instructionSet[0x33] = &Processor::inc_SP;
        m_instructionSet[0x34] = &Processor::inc_HL;
        m_instructionSet[0x35] = &Processor::dec_HL;
        m_instructionSet[0x36] = &Processor::ld_HL_n_8<0x36>;
        m_instructionSet[0x37] = &Processor::scf;
        m_instructionSet[0x38] = &Processor::jr_cc_n<Registers::Flag::C>;
        m_instructionSet[0x39] = &Processor::add_HL_SP;
        m_instructionSet[0x3A] = &Processor::ld_A_HLd;
        m_instructionSet[0x3B] = &Processor::dec_SP;
        m_instructionSet[0x3C] = &Processor::inc_r<Registers::A>;
        m_instructionSet[0x3D] = &Processor::dec_r<Registers::A>;
        m_instructionSet[0x3E] = &Processor::ld_r_n_8<0x3E>;
        m_instructionSet[0x3F] = &Processor::ccf;
        m_instructionSet[0x40] = &Processor::ld_r_r_8<0x40>;
        m_instructionSet[0x41] = &Processor::ld_r_r_8<0x41>;
        m_instructionSet[0x42] = &Processor::ld_r_r_8<0x42>;
        m_instructionSet[0x43] = &Processor::ld_r_r_8<0x43>;
        m_instructionSet[0x44] = &Processor::ld_r_r_8<0x44>;
        m_instructionSet[0x45] = &Processor::ld_r_r_8<0x45>;
        m_instructionSet[0x46] = &Processor::ld_r_HL<0x46>;
        m_instructionSet[0x47] = &Processor::ld_r_r_8<0x47>;
        m_instructionSet[0x48] = &Processor::ld_r_r_8<0x48>;
        m_instructionSet[0x49] = &Processor::ld_r_r_8<0x49>;
        m_instructionSet[0x4A] = &Processor::ld_r_r_8<0x4A>;
        m_instructionSet[0x4B] = &Processor::ld_r_r_8<0x4B>;
        m_instructionSet[0x4C] = &Processor::ld_r_r_8<0x4C>;
        m_instructionSet[0x4D] = &Processor::ld_r_r_8<0x4D>;
        m_instructionSet[0x4E] = &Processor::ld_r_HL<0x4E>;
        m_instructionSet[0x4F] = &Processor::ld_r_r_8<0x4F>;
        m_instructionSet[0x50] = &Processor::ld_r_r_8<0x50>;
        m_instructionSet[0x51] = &Processor::ld_r_r_8<0x51>;
        m_instructionSet[0x52] = &Processor::ld_r_r_8<0x52>;
        m_instructionSet[0x53] = &Processor::ld_r_r_8<0x53>;
        m_instructionSet[0x54] = &Processor::ld_r_r_8<0x54>;
        m_instructionSet[0x55] = &Processor::ld_r_r_8<0x55>;
        m_instructionSet[0x56] = &Processor::ld_r_HL<0x56>;
        m_instructionSet[0x57] = &Processor::ld_r_r_8<0x57>;
        m_instructionSet[0x58] = &Processor::ld_r_r_8<0x58>;
        m_instructionSet[0x59] = &Processor::ld_r_r_8<0x59>;
        m_instructionSet[0x5A] = &Processor::ld_r_r_8<0x5A>;
        m_instructionSet[0x5B] = &Processor::ld_r_r_8<0x5B>;
        m_instructionSet[0x5C] = &Processor::ld_r_r_8<0x5C>;
        m_instructionSet[0x5D] = &Processor::ld_r_r_8<0x5D>;
        m_instructionSet[0x5E] = &Processor::ld_r_HL<0x5E>;
        m_instructionSet[0x5F] = &Processor::ld_r_r_8<0x5F>;
        m_instructionSet[0x60] = &Processor::ld_r_r_8<0x60>;
        m_instructionSet[0x61] = &Processor::ld_r_r_8<0x61>;
        m_instructionSet[0x62] = &Processor::ld_r_r_8<0x62>;
        m_instructionSet[0x63] = &Processor::ld_r_r_8<0x63>;
        m_instructionSet[0x64] = &Processor::ld_r_r_8<0x64>;
        m_instructionSet[0x65] = &Processor::ld_r_r_8<0x65>;
        m_instructionSet[0x66] = &Processor::ld_r_HL<0x66>;
        m_instructionSet[0x67] = &Processor::ld_r_r_8<0x67>;
        m_instructionSet[0x68] = &Processor::ld_r_r_8<0x68>;
        m_instructionSet[0x69] = &Processor::ld_r_r_8<0x69>;
        m_instructionSet[0x6A] = &Processor::ld_r_r_8<0x6A>;
        m_instructionSet[0x6B] = &Processor::ld_r_r_8<0x6B>;
        m_instructionSet[0x6C] = &Processor::ld_r_r_8<0x6C>;
        m_instructionSet[0x6D] = &Processor::ld_r_r_8<0x6D>;
        m_instructionSet[0x6E] = &Processor::ld_r_HL<0x6E>;
        m_instructionSet[0x6F] = &Processor::ld_r_r_8<0x6F>;
        m_instructionSet[0x70] = &Processor::ld_HL_r<0x70>;
        m_instructionSet[0x71] = &Processor::ld_HL_r<0x71>;
        m_instructionSet[0x72] = &Processor::ld_HL_r<0x72>;
        m_instructionSet[0x73] = &Processor::ld_HL_r<0x73>;
        m_instructionSet[0x74] = &Processor::ld_HL_r<0x74>;
        m_instructionSet[0x75] = &Processor::ld_HL_r<0x75>;
        m_instructionSet[0x76] = &Processor::halt;
        m_instructionSet[0x77] = &Processor::ld_HL_r<0x77>;
        m_instructionSet[0x78] = &Processor::ld_r_r_8<0x78>;
        m_instructionSet[0x79] = &Processor::ld_r_r_8<0x79>;
        m_instructionSet[0x7A] = &Processor::ld_r_r_8<0x7A>;
        m_instructionSet[0x7B] = &Processor::ld_r_r_8<0x7B>;
        m_instructionSet[0x7C] = &Processor::ld_r_r_8<0x7C>;
        m_instructionSet[0x7D] = &Processor::ld_r_r_8<0x7D>;
        m_instructionSet[0x7E] = &Processor::ld_r_HL<0x7E>;
        m_instructionSet[0x7F] = &Processor::ld_r_r_8<0x7F>;
        m_instructionSet[0x80] = &Processor::add_r<Registers::B>;
        m_instructionSet[0x81] = &Processor::add_r<Registers::C>;
        m_instructionSet[0x82] = &Processor::add_r<Registers::D>;
        m_instructionSet[0x83] = &Processor::add_r<Registers::E>;
        m_instructionSet[0x84] = &Processor::add_r<Registers::H>;
        m_instructionSet[0x85] = &Processor::add_r<Registers::L>;
        m_instructionSet[0x86] = &Processor::add_HL;
        m_instructionSet[0x87] = &Processor::add_r<Registers::A>;
        m_instructionSet[0x88] = &Processor::adc_r<Registers::B>;
        m_instructionSet[0x89] = &Processor::adc_r<Registers::C>;
        m_instructionSet[0x8A] = &Processor::adc_r<Registers::D>;
        m_instructionSet[0x8B] = &Processor::adc_r<Registers::E>;
        m_instructionSet[0x8C] = &Processor::adc_r<Registers::H>;
        m_instructionSet[0x8D] = &Processor::adc_r<Registers::L>;
        m_instructionSet[0x8E] = &Processor::adc_HL;
        m_instructionSet[0x8F] = &Processor::adc_r<Registers::A>;
        m_instructionSet[0x90] = &Processor::sub_r<Registers::B>;
        m_instructionSet[0x91] = &Processor::sub_r<Registers::C>;
        m_instructionSet[0x92] = &Processor::sub_r<Registers::D>;
        m_instructionSet[0x93] = &Processor::sub_r<Registers::E>;
        m_instructionSet[0x94] = &Processor::sub_r<Registers::H>;
        m_instructionSet[0x95] = &Processor::sub_r<Registers::L>;
        m_instructionSet[0x96] = &Processor::sub_HL;
        m_instructionSet[0x97] = &Processor::sub_r<Registers::A>;
        m_instructionSet[0x98] = &Processor::sbc_r<Registers::B>;
        m_instructionSet[0x99] = &Processor::sbc_r<Registers::C>;
        m_instructionSet[0x9A] = &Processor::sbc_r<Registers::D>;
        m_instructionSet[0x9B] = &Processor::sbc_r<Registers::E>;
        m_instructionSet[0x9C] = &Processor::sbc_r<Registers::H>;
        m_instructionSet[0x9D] = &Processor::sbc_r<Registers::L>;
        m_instructionSet[0x9E] = &Processor::sbc_HL;
        m_instructionSet[0x9F] = &Processor::sbc_r<Registers::A>;
        m_instructionSet[0xA0] = &Processor::and_r<Registers::B>;
        m_instructionSet[0xA1] = &Processor::and_r<Registers::C>;
        m_instructionSet[0xA2] = &Processor::and_r<Registers::D>;
        m_instructionSet[0xA3] = &Processor::and_r<Registers::E>;
        m_instructionSet[0xA4] = &Processor::and_r<Registers::H>;
        m_instructionSet[0xA5] = &Processor::and_r<Registers::L>;
        m_instructionSet[0xA6] = &Processor::and_HL;
        m_instructionSet[0xA7] = &Processor::and_r<Registers::A>;
        m_instructionSet[0xA8] = &Processor::xor_r<Registers::B>;
        m_instructionSet[0xA9] = &Processor::xor_r<Registers::C>;
        m_instructionSet[0xAA] = &Processor::xor_r<Registers::D>;
        m_instructionSet[0xAB] = &Processor::xor_r<Registers::E>;
        m_instructionSet[0xAC] = &Processor::xor_r<Registers::H>;
        m_instructionSet[0xAD] = &Processor::xor_r<Registers::L>;
        m_instructionSet[0xAE] = &Processor::xor_HL;
        m_instructionSet[0xAF] = &Processor::xor_r<Registers::A>;
        m_instructionSet[0xB0] = &Processor::or_r<Registers::B>;
        m_instructionSet[0xB1] = &Processor::or_r<Registers::C>;
        m_instructionSet[0xB2] = &Processor::or_r<Registers::D>;
        m_instructionSet[0xB3] = &Processor::or_r<Registers::E>;
        m_instructionSet[0xB4] = &Processor::or_r<Registers::H>;
        m_instructionSet[0xB5] = &Processor::or_r<Registers::L>;
        m_instructionSet[0xB6] = &Processor::or_HL;
        m_instructionSet[0xB7] = &Processor::or_r<Registers::A>;
        m_instructionSet[0xB8] = &Processor::cp_r<Registers::B>;
        m_instructionSet[0xB9] = &Processor::cp_r<Registers::C>;
        m_instructionSet[0xBA] = &Processor::cp_r<Registers::D>;
        m_instructionSet[0xBB] = &Processor::cp_r<Registers::E>;
        m_instructionSet[0xBC] = &Processor::cp_r<Registers::H>;
        m_instructionSet[0xBD] = &Processor::cp_r<Registers::L>;
        m_instructionSet[0xBE] = &Processor::cp_HL;
        m_instructionSet[0xBF] = &Processor::cp_r<Registers::A>;
        m_instructionSet[0xC0] = &Processor::ret_ncc<Registers::Flag::Z>;
        m_instructionSet[0xC1] = &Processor::pop_rr<Registers::BC>;
        m_instructionSet[0xC2] = &Processor::jp_ncc_nn<Registers::Flag::Z>;
        m_instructionSet[0xC3] = &Processor::jp_nn;
        m_instructionSet[0xC4] = &Processor::call_ncc_nn<Registers::Flag::Z>;
        m_instructionSet[0xC5] = &Processor::push_rr<Registers::BC>;
        m_instructionSet[0xC6] = &Processor::add_n;
        m_instructionSet[0xC7] = &Processor::rst<0x00>;
        m_instructionSet[0xC8] = &Processor::ret_cc<Registers::Flag::Z>;
        m_instructionSet[0xC9] = &Processor::ret;
        m_instructionSet[0xCA] = &Processor::jp_cc_nn<Registers::Flag::Z>;
        m_instructionSet[0xCB] = &Processor::cb;
        m_instructionSet[0xCC] = &Processor::call_cc_nn<Registers::Flag::Z>;
        m_instructionSet[0xCD] = &Processor::call_nn;
        m_instructionSet[0xCE] = &Processor::adc_n;
        m_instructionSet[0xCF] = &Processor::rst<0x08>;
        m_instructionSet[0xD0] = &Processor::ret_ncc<Registers::Flag::C>;
        m_instructionSet[0xD1] = &Processor::pop_rr<Registers::DE>;
        m_instructionSet[0xD2] = &Processor::jp_ncc_nn<Registers::Flag::C>;
        m_instructionSet[0xD4] = &Processor::call_ncc_nn<Registers::Flag::C>;
        m_instructionSet[0xD5] = &Processor::push_rr<Registers::DE>;
        m_instructionSet[0xD6] = &Processor::sub_n;
        m_instructionSet[0xD7] = &Processor::rst<0x10>;
        m_instructionSet[0xD8] = &Processor::ret_cc<Registers::Flag::C>;
        m_instructionSet[0xD9] = &Processor::reti;
        m_instructionSet[0xDA] = &Processor::jp_cc_nn<Registers::Flag::C>;
        m_instructionSet[0xDC] = &Processor::call_cc_nn<Registers::Flag::C>;
        m_instructionSet[0xDE] = &Processor::sbc_n;
        m_instructionSet[0xDF] = &Processor::rst<0x18>;
        m_instructionSet[0xE0] = &Processor::ldh_an_A;
        m_instructionSet[0xE1] = &Processor::pop_rr<Registers::HL>;
        m_instructionSet[0xE2] = &Processor::ldh_aC_A;
        m_instructionSet[0xE5] = &Processor::push_rr<Registers::HL>;
        m_instructionSet[0xE6] = &Processor::and_n;
        m_instructionSet[0xE7] = &Processor::rst<0x20>;
        m_instructionSet[0xE8] = &Processor::add_SP_n;
        m_instructionSet[0xE9] = &Processor::jp_HL;
        m_instructionSet[0xEA] = &Processor::ld_nn_A;
        m_instructionSet[0xEE] = &Processor::xor_n;
        m_instructionSet[0xEF] = &Processor::rst<0x28>;
        m_instructionSet[0xF0] = &Processor::ldh_A_an;
        m_instructionSet[0xF1] = &Processor::pop_rr<Registers::AF>;
        m_instructionSet[0xF2] = &Processor::ldh_A_aC;
        m_instructionSet[0xF3] = &Processor::di;
        m_instructionSet[0xF5] = &Processor::push_rr<Registers::AF>;
        m_instructionSet[0xF6] = &Processor::or_n;
        m_instructionSet[0xF7] = &Processor::rst<0x30>;
        m_instructionSet[0xF8] = &Processor::ld_HL_SP_r8;
        m_instructionSet[0xF9] = &Processor::ld_SP_HL;
        m_instructionSet[0xFA] = &Processor::ld_A_nn;
        m_instructionSet[0xFB] = &Processor::ei;
        m_instructionSet[0xFE] = &Processor::cp_n;
        m_instructionSet[0xFF] = &Processor::rst<0x38>;
    }

    void Processor::fillCbInstructionSet()
    {
        m_cbInstructionSet[0x00] = &Processor::rlc_r<Registers::B>;
        m_cbInstructionSet[0x01] = &Processor::rlc_r<Registers::C>;
        m_cbInstructionSet[0x02] = &Processor::rlc_r<Registers::D>;
        m_cbInstructionSet[0x03] = &Processor::rlc_r<Registers::E>;
        m_cbInstructionSet[0x04] = &Processor::rlc_r<Registers::H>;
        m_cbInstructionSet[0x05] = &Processor::rlc_r<Registers::L>;
        m_cbInstructionSet[0x06] = &Processor::rlc_HL;
        m_cbInstructionSet[0x07] = &Processor::rlc_r<Registers::A>;
        m_cbInstructionSet[0x08] = &Processor::rrc_r<Registers::B>;
        m_cbInstructionSet[0x09] = &Processor::rrc_r<Registers::C>;
        m_cbInstructionSet[0x0A] = &Processor::rrc_r<Registers::D>;
        m_cbInstructionSet[0x0B] = &Processor::rrc_r<Registers::E>;
        m_cbInstructionSet[0x0C] = &Processor::rrc_r<Registers::H>;
        m_cbInstructionSet[0x0D] = &Processor::rrc_r<Registers::L>;
        m_cbInstructionSet[0x0E] = &Processor::rrc_HL;
        m_cbInstructionSet[0x0F] = &Processor::rrc_r<Registers::A>;
        m_cbInstructionSet[0x10] = &Processor::rl_r<Registers::B>;
        m_cbInstructionSet[0x11] = &Processor::rl_r<Registers::C>;
        m_cbInstructionSet[0x12] = &Processor::rl_r<Registers::D>;
        m_cbInstructionSet[0x13] = &Processor::rl_r<Registers::E>;
        m_cbInstructionSet[0x14] = &Processor::rl_r<Registers::H>;
        m_cbInstructionSet[0x15] = &Processor::rl_r<Registers::L>;
        m_cbInstructionSet[0x16] = &Processor::rl_HL;
        m_cbInstructionSet[0x17] = &Processor::rl_r<Registers::A>;
        m_cbInstructionSet[0x18] = &Processor::rr_r<Registers::B>;
        m_cbInstructionSet[0x19] = &Processor::rr_r<Registers::C>;
        m_cbInstructionSet[0x1A] = &Processor::rr_r<Registers::D>;
        m_cbInstructionSet[0x1B] = &Processor::rr_r<Registers::E>;
        m_cbInstructionSet[0x1C] = &Processor::rr_r<Registers::H>;
        m_cbInstructionSet[0x1D] = &Processor::rr_r<Registers::L>;
        m_cbInstructionSet[0x1E] = &Processor::rr_HL;
        m_cbInstructionSet[0x1F] = &Processor::rr_r<Registers::A>;
        m_cbInstructionSet[0x20] = &Processor::sla_r<Registers::B>;
        m_cbInstructionSet[0x21] = &Processor::sla_r<Registers::C>;
        m_cbInstructionSet[0x22] = &Processor::sla_r<Registers::D>;
        m_cbInstructionSet[0x23] = &Processor::sla_r<Registers::E>;
        m_cbInstructionSet[0x24] = &Processor::sla_r<Registers::H>;
        m_cbInstructionSet[0x25] = &Processor::sla_r<Registers::L>;
        m_cbInstructionSet[0x26] = &Processor::sla_HL;
        m_cbInstructionSet[0x27] = &Processor::sla_r<Registers::A>;
        m_cbInstructionSet[0x28] = &Processor::sra_r<Registers::B>;
        m_cbInstructionSet[0x29] = &Processor::sra_r<Registers::C>;
        m_cbInstructionSet[0x2A] = &Processor::sra_r<Registers::D>;
        m_cbInstructionSet[0x2B] = &Processor::sra_r<Registers::E>;
        m_cbInstructionSet[0x2C] = &Processor::sra_r<Registers::H>;
        m_cbInstructionSet[0x2D] = &Processor::sra_r<Registers::L>;
        m_cbInstructionSet[0x2E] = &Processor::sra_HL;
        m_cbInstructionSet[0x2F] = &Processor::sra_r<Registers::A>;
        m_cbInstructionSet[0x2F] = &Processor::swap_r<Registers::A>;
        m_cbInstructionSet[0x30] = &Processor::swap_r<Registers::B>;
        m_cbInstructionSet[0x31] = &Processor::swap_r<Registers::C>;
        m_cbInstructionSet[0x32] = &Processor::swap_r<Registers::D>;
        m_cbInstructionSet[0x33] = &Processor::swap_r<Registers::E>;
        m_cbInstructionSet[0x34] = &Processor::swap_r<Registers::H>;
        m_cbInstructionSet[0x35] = &Processor::swap_r<Registers::L>;
        m_cbInstructionSet[0x36] = &Processor::swap_HL;
        m_cbInstructionSet[0x37] = &Processor::swap_r<Registers::A>;
        m_cbInstructionSet[0x38] = &Processor::srl_r<Registers::B>;
        m_cbInstructionSet[0x39] = &Processor::srl_r<Registers::C>;
        m_cbInstructionSet[0x3A] = &Processor::srl_r<Registers::D>;
        m_cbInstructionSet[0x3B] = &Processor::srl_r<Registers::E>;
        m_cbInstructionSet[0x3C] = &Processor::srl_r<Registers::H>;
        m_cbInstructionSet[0x3D] = &Processor::srl_r<Registers::L>;
        m_cbInstructionSet[0x3E] = &Processor::srl_HL;
        m_cbInstructionSet[0x3F] = &Processor::srl_r<Registers::A>;
        m_cbInstructionSet[0x40] = &Processor::bit_n_r<0, Registers::B>;
        m_cbInstructionSet[0x41] = &Processor::bit_n_r<0, Registers::C>;
        m_cbInstructionSet[0x42] = &Processor::bit_n_r<0, Registers::D>;
        m_cbInstructionSet[0x43] = &Processor::bit_n_r<0, Registers::E>;
        m_cbInstructionSet[0x44] = &Processor::bit_n_r<0, Registers::H>;
        m_cbInstructionSet[0x45] = &Processor::bit_n_r<0, Registers::L>;
        m_cbInstructionSet[0x46] = &Processor::bit_n_HL<0>;
        m_cbInstructionSet[0x47] = &Processor::bit_n_r<0, Registers::A>;
        m_cbInstructionSet[0x48] = &Processor::bit_n_r<1, Registers::B>;
        m_cbInstructionSet[0x49] = &Processor::bit_n_r<1, Registers::C>;
        m_cbInstructionSet[0x4A] = &Processor::bit_n_r<1, Registers::D>;
        m_cbInstructionSet[0x4B] = &Processor::bit_n_r<1, Registers::E>;
        m_cbInstructionSet[0x4C] = &Processor::bit_n_r<1, Registers::H>;
        m_cbInstructionSet[0x4D] = &Processor::bit_n_r<1, Registers::L>;
        m_cbInstructionSet[0x4E] = &Processor::bit_n_HL<1>;
        m_cbInstructionSet[0x4F] = &Processor::bit_n_r<1, Registers::A>;
        m_cbInstructionSet[0x50] = &Processor::bit_n_r<2, Registers::B>;
        m_cbInstructionSet[0x51] = &Processor::bit_n_r<2, Registers::C>;
        m_cbInstructionSet[0x52] = &Processor::bit_n_r<2, Registers::D>;
        m_cbInstructionSet[0x53] = &Processor::bit_n_r<2, Registers::E>;
        m_cbInstructionSet[0x54] = &Processor::bit_n_r<2, Registers::H>;
        m_cbInstructionSet[0x55] = &Processor::bit_n_r<2, Registers::L>;
        m_cbInstructionSet[0x56] = &Processor::bit_n_HL<2>;
        m_cbInstructionSet[0x57] = &Processor::bit_n_r<2, Registers::A>;
        m_cbInstructionSet[0x58] = &Processor::bit_n_r<3, Registers::B>;
        m_cbInstructionSet[0x59] = &Processor::bit_n_r<3, Registers::C>;
        m_cbInstructionSet[0x5A] = &Processor::bit_n_r<3, Registers::D>;
        m_cbInstructionSet[0x5B] = &Processor::bit_n_r<3, Registers::E>;
        m_cbInstructionSet[0x5C] = &Processor::bit_n_r<3, Registers::H>;
        m_cbInstructionSet[0x5D] = &Processor::bit_n_r<3, Registers::L>;
        m_cbInstructionSet[0x5E] = &Processor::bit_n_HL<3>;
        m_cbInstructionSet[0x5F] = &Processor::bit_n_r<3, Registers::A>;
        m_cbInstructionSet[0x60] = &Processor::bit_n_r<4, Registers::B>;
        m_cbInstructionSet[0x61] = &Processor::bit_n_r<4, Registers::C>;
        m_cbInstructionSet[0x62] = &Processor::bit_n_r<4, Registers::D>;
        m_cbInstructionSet[0x63] = &Processor::bit_n_r<4, Registers::E>;
        m_cbInstructionSet[0x64] = &Processor::bit_n_r<4, Registers::H>;
        m_cbInstructionSet[0x65] = &Processor::bit_n_r<4, Registers::L>;
        m_cbInstructionSet[0x66] = &Processor::bit_n_HL<4>;
        m_cbInstructionSet[0x67] = &Processor::bit_n_r<4, Registers::A>;
        m_cbInstructionSet[0x68] = &Processor::bit_n_r<5, Registers::B>;
        m_cbInstructionSet[0x69] = &Processor::bit_n_r<5, Registers::C>;
        m_cbInstructionSet[0x6A] = &Processor::bit_n_r<5, Registers::D>;
        m_cbInstructionSet[0x6B] = &Processor::bit_n_r<5, Registers::E>;
        m_cbInstructionSet[0x6C] = &Processor::bit_n_r<5, Registers::H>;
        m_cbInstructionSet[0x6D] = &Processor::bit_n_r<5, Registers::L>;
        m_cbInstructionSet[0x6E] = &Processor::bit_n_HL<5>;
        m_cbInstructionSet[0x6F] = &Processor::bit_n_r<5, Registers::A>;
        m_cbInstructionSet[0x70] = &Processor::bit_n_r<6, Registers::B>;
        m_cbInstructionSet[0x71] = &Processor::bit_n_r<6, Registers::C>;
        m_cbInstructionSet[0x72] = &Processor::bit_n_r<6, Registers::D>;
        m_cbInstructionSet[0x73] = &Processor::bit_n_r<6, Registers::E>;
        m_cbInstructionSet[0x74] = &Processor::bit_n_r<6, Registers::H>;
        m_cbInstructionSet[0x75] = &Processor::bit_n_r<6, Registers::L>;
        m_cbInstructionSet[0x76] = &Processor::bit_n_HL<6>;
        m_cbInstructionSet[0x77] = &Processor::bit_n_r<6, Registers::A>;
        m_cbInstructionSet[0x78] = &Processor::bit_n_r<7, Registers::B>;
        m_cbInstructionSet[0x79] = &Processor::bit_n_r<7, Registers::C>;
        m_cbInstructionSet[0x7A] = &Processor::bit_n_r<7, Registers::D>;
        m_cbInstructionSet[0x7B] = &Processor::bit_n_r<7, Registers::E>;
        m_cbInstructionSet[0x7C] = &Processor::bit_n_r<7, Registers::H>;
        m_cbInstructionSet[0x7D] = &Processor::bit_n_r<7, Registers::L>;
        m_cbInstructionSet[0x7E] = &Processor::bit_n_HL<7>;
        m_cbInstructionSet[0x7F] = &Processor::bit_n_r<7, Registers::A>;
        m_cbInstructionSet[0x80] = &Processor::res_n_r<0, Registers::B>;
        m_cbInstructionSet[0x81] = &Processor::res_n_r<0, Registers::C>;
        m_cbInstructionSet[0x82] = &Processor::res_n_r<0, Registers::D>;
        m_cbInstructionSet[0x83] = &Processor::res_n_r<0, Registers::E>;
        m_cbInstructionSet[0x84] = &Processor::res_n_r<0, Registers::H>;
        m_cbInstructionSet[0x85] = &Processor::res_n_r<0, Registers::L>;
        m_cbInstructionSet[0x86] = &Processor::res_n_HL<0>;
        m_cbInstructionSet[0x87] = &Processor::res_n_r<0, Registers::A>;
        m_cbInstructionSet[0x88] = &Processor::res_n_r<1, Registers::B>;
        m_cbInstructionSet[0x89] = &Processor::res_n_r<1, Registers::C>;
        m_cbInstructionSet[0x8A] = &Processor::res_n_r<1, Registers::D>;
        m_cbInstructionSet[0x8B] = &Processor::res_n_r<1, Registers::E>;
        m_cbInstructionSet[0x8C] = &Processor::res_n_r<1, Registers::H>;
        m_cbInstructionSet[0x8D] = &Processor::res_n_r<1, Registers::L>;
        m_cbInstructionSet[0x8E] = &Processor::res_n_HL<1>;
        m_cbInstructionSet[0x8F] = &Processor::res_n_r<1, Registers::A>;
        m_cbInstructionSet[0x90] = &Processor::res_n_r<2, Registers::B>;
        m_cbInstructionSet[0x91] = &Processor::res_n_r<2, Registers::C>;
        m_cbInstructionSet[0x92] = &Processor::res_n_r<2, Registers::D>;
        m_cbInstructionSet[0x93] = &Processor::res_n_r<2, Registers::E>;
        m_cbInstructionSet[0x94] = &Processor::res_n_r<2, Registers::H>;
        m_cbInstructionSet[0x95] = &Processor::res_n_r<2, Registers::L>;
        m_cbInstructionSet[0x96] = &Processor::res_n_HL<2>;
        m_cbInstructionSet[0x97] = &Processor::res_n_r<2, Registers::A>;
        m_cbInstructionSet[0x98] = &Processor::res_n_r<3, Registers::B>;
        m_cbInstructionSet[0x99] = &Processor::res_n_r<3, Registers::C>;
        m_cbInstructionSet[0x9A] = &Processor::res_n_r<3, Registers::D>;
        m_cbInstructionSet[0x9B] = &Processor::res_n_r<3, Registers::E>;
        m_cbInstructionSet[0x9C] = &Processor::res_n_r<3, Registers::H>;
        m_cbInstructionSet[0x9D] = &Processor::res_n_r<3, Registers::L>;
        m_cbInstructionSet[0x9E] = &Processor::res_n_HL<3>;
        m_cbInstructionSet[0x9F] = &Processor::res_n_r<3, Registers::A>;
        m_cbInstructionSet[0xA0] = &Processor::res_n_r<4, Registers::B>;
        m_cbInstructionSet[0xA1] = &Processor::res_n_r<4, Registers::C>;
        m_cbInstructionSet[0xA2] = &Processor::res_n_r<4, Registers::D>;
        m_cbInstructionSet[0xA3] = &Processor::res_n_r<4, Registers::E>;
        m_cbInstructionSet[0xA4] = &Processor::res_n_r<4, Registers::H>;
        m_cbInstructionSet[0xA5] = &Processor::res_n_r<4, Registers::L>;
        m_cbInstructionSet[0xA6] = &Processor::res_n_HL<4>;
        m_cbInstructionSet[0xA7] = &Processor::res_n_r<4, Registers::A>;
        m_cbInstructionSet[0xA8] = &Processor::res_n_r<5, Registers::B>;
        m_cbInstructionSet[0xA9] = &Processor::res_n_r<5, Registers::C>;
        m_cbInstructionSet[0xAA] = &Processor::res_n_r<5, Registers::D>;
        m_cbInstructionSet[0xAB] = &Processor::res_n_r<5, Registers::E>;
        m_cbInstructionSet[0xAC] = &Processor::res_n_r<5, Registers::H>;
        m_cbInstructionSet[0xAD] = &Processor::res_n_r<5, Registers::L>;
        m_cbInstructionSet[0xAE] = &Processor::res_n_HL<5>;
        m_cbInstructionSet[0xAF] = &Processor::res_n_r<5, Registers::A>;
        m_cbInstructionSet[0xB0] = &Processor::res_n_r<6, Registers::B>;
        m_cbInstructionSet[0xB1] = &Processor::res_n_r<6, Registers::C>;
        m_cbInstructionSet[0xB2] = &Processor::res_n_r<6, Registers::D>;
        m_cbInstructionSet[0xB3] = &Processor::res_n_r<6, Registers::E>;
        m_cbInstructionSet[0xB4] = &Processor::res_n_r<6, Registers::H>;
        m_cbInstructionSet[0xB5] = &Processor::res_n_r<6, Registers::L>;
        m_cbInstructionSet[0xB6] = &Processor::res_n_HL<6>;
        m_cbInstructionSet[0xB7] = &Processor::res_n_r<6, Registers::A>;
        m_cbInstructionSet[0xB8] = &Processor::res_n_r<7, Registers::B>;
        m_cbInstructionSet[0xB9] = &Processor::res_n_r<7, Registers::C>;
        m_cbInstructionSet[0xBA] = &Processor::res_n_r<7, Registers::D>;
        m_cbInstructionSet[0xBB] = &Processor::res_n_r<7, Registers::E>;
        m_cbInstructionSet[0xBC] = &Processor::res_n_r<7, Registers::H>;
        m_cbInstructionSet[0xBD] = &Processor::res_n_r<7, Registers::L>;
        m_cbInstructionSet[0xBE] = &Processor::res_n_HL<7>;
        m_cbInstructionSet[0xBF] = &Processor::res_n_r<7, Registers::A>;
        m_cbInstructionSet[0xC0] = &Processor::set_n_r<0, Registers::B>;
        m_cbInstructionSet[0xC1] = &Processor::set_n_r<0, Registers::C>;
        m_cbInstructionSet[0xC2] = &Processor::set_n_r<0, Registers::D>;
        m_cbInstructionSet[0xC3] = &Processor::set_n_r<0, Registers::E>;
        m_cbInstructionSet[0xC4] = &Processor::set_n_r<0, Registers::H>;
        m_cbInstructionSet[0xC5] = &Processor::set_n_r<0, Registers::L>;
        m_cbInstructionSet[0xC6] = &Processor::set_n_HL<0>;
        m_cbInstructionSet[0xC7] = &Processor::set_n_r<0, Registers::A>;
        m_cbInstructionSet[0xC8] = &Processor::set_n_r<1, Registers::B>;
        m_cbInstructionSet[0xC9] = &Processor::set_n_r<1, Registers::C>;
        m_cbInstructionSet[0xCA] = &Processor::set_n_r<1, Registers::D>;
        m_cbInstructionSet[0xCB] = &Processor::set_n_r<1, Registers::E>;
        m_cbInstructionSet[0xCC] = &Processor::set_n_r<1, Registers::H>;
        m_cbInstructionSet[0xCD] = &Processor::set_n_r<1, Registers::L>;
        m_cbInstructionSet[0xCE] = &Processor::set_n_HL<1>;
        m_cbInstructionSet[0xCF] = &Processor::set_n_r<1, Registers::A>;
        m_cbInstructionSet[0xD0] = &Processor::set_n_r<2, Registers::B>;
        m_cbInstructionSet[0xD1] = &Processor::set_n_r<2, Registers::C>;
        m_cbInstructionSet[0xD2] = &Processor::set_n_r<2, Registers::D>;
        m_cbInstructionSet[0xD3] = &Processor::set_n_r<2, Registers::E>;
        m_cbInstructionSet[0xD4] = &Processor::set_n_r<2, Registers::H>;
        m_cbInstructionSet[0xD5] = &Processor::set_n_r<2, Registers::L>;
        m_cbInstructionSet[0xD6] = &Processor::set_n_HL<2>;
        m_cbInstructionSet[0xD7] = &Processor::set_n_r<2, Registers::A>;
        m_cbInstructionSet[0xD8] = &Processor::set_n_r<3, Registers::B>;
        m_cbInstructionSet[0xD9] = &Processor::set_n_r<3, Registers::C>;
        m_cbInstructionSet[0xDA] = &Processor::set_n_r<3, Registers::D>;
        m_cbInstructionSet[0xDB] = &Processor::set_n_r<3, Registers::E>;
        m_cbInstructionSet[0xDC] = &Processor::set_n_r<3, Registers::H>;
        m_cbInstructionSet[0xDD] = &Processor::set_n_r<3, Registers::L>;
        m_cbInstructionSet[0xDE] = &Processor::set_n_HL<3>;
        m_cbInstructionSet[0xDF] = &Processor::set_n_r<3, Registers::A>;
        m_cbInstructionSet[0xE0] = &Processor::set_n_r<4, Registers::B>;
        m_cbInstructionSet[0xE1] = &Processor::set_n_r<4, Registers::C>;
        m_cbInstructionSet[0xE2] = &Processor::set_n_r<4, Registers::D>;
        m_cbInstructionSet[0xE3] = &Processor::set_n_r<4, Registers::E>;
        m_cbInstructionSet[0xE4] = &Processor::set_n_r<4, Registers::H>;
        m_cbInstructionSet[0xE5] = &Processor::set_n_r<4, Registers::L>;
        m_cbInstructionSet[0xE6] = &Processor::set_n_HL<4>;
        m_cbInstructionSet[0xE7] = &Processor::set_n_r<4, Registers::A>;
        m_cbInstructionSet[0xE8] = &Processor::set_n_r<5, Registers::B>;
        m_cbInstructionSet[0xE9] = &Processor::set_n_r<5, Registers::C>;
        m_cbInstructionSet[0xEA] = &Processor::set_n_r<5, Registers::D>;
        m_cbInstructionSet[0xEB] = &Processor::set_n_r<5, Registers::E>;
        m_cbInstructionSet[0xEC] = &Processor::set_n_r<5, Registers::H>;
        m_cbInstructionSet[0xED] = &Processor::set_n_r<5, Registers::L>;
        m_cbInstructionSet[0xEE] = &Processor::set_n_HL<5>;
        m_cbInstructionSet[0xEF] = &Processor::set_n_r<5, Registers::A>;
        m_cbInstructionSet[0xF0] = &Processor::set_n_r<6, Registers::B>;
        m_cbInstructionSet[0xF1] = &Processor::set_n_r<6, Registers::C>;
        m_cbInstructionSet[0xF2] = &Processor::set_n_r<6, Registers::D>;
        m_cbInstructionSet[0xF3] = &Processor::set_n_r<6, Registers::E>;
        m_cbInstructionSet[0xF4] = &Processor::set_n_r<6, Registers::H>;
        m_cbInstructionSet[0xF5] = &Processor::set_n_r<6, Registers::L>;
        m_cbInstructionSet[0xF6] = &Processor::set_n_HL<6>;
        m_cbInstructionSet[0xF7] = &Processor::set_n_r<6, Registers::A>;
        m_cbInstructionSet[0xF8] = &Processor::set_n_r<7, Registers::B>;
        m_cbInstructionSet[0xF9] = &Processor::set_n_r<7, Registers::C>;
        m_cbInstructionSet[0xFA] = &Processor::set_n_r<7, Registers::D>;
        m_cbInstructionSet[0xFB] = &Processor::set_n_r<7, Registers::E>;
        m_cbInstructionSet[0xFC] = &Processor::set_n_r<7, Registers::H>;
        m_cbInstructionSet[0xFD] = &Processor::set_n_r<7, Registers::L>;
        m_cbInstructionSet[0xFE] = &Processor::set_n_HL<7>;
        m_cbInstructionSet[0xFF] = &Processor::set_n_r<7, Registers::A>;
    }

    int Processor::unhandled() 
    {
        return 1;
    }

    uint8_t Processor::getImmediate8()
    {
        uint8_t n = m_memory.read8(m_registers.getPC());
        m_registers.incrementPC();

        return n;
    }

    uint16_t Processor::getImmediate16()
    {
        uint16_t nn = m_memory.read16(m_registers.getPC());
        m_registers.incrementPC();
        m_registers.incrementPC();
        
        return nn;
    }
    
    int Processor::nop()
    {
        return 1;
    }

    int Processor::cb()
    {
        uint8_t opCode = getImmediate8();
        return (this->*m_cbInstructionSet[opCode])();
    }

    int Processor::ld_A_nn()
    {
        uint16_t nn = getImmediate16();

        uint8_t val = m_memory.read8(nn);
        m_registers.write8<Registers::A>(val);
        return 4;
    }

    int Processor::ld_nn_A()
    {
        uint16_t nn = getImmediate16();

        uint8_t a = m_registers.read8<Registers::A>();
        m_memory.write8(nn, a);

        return 4;
    }

    int Processor::ldh_A_aC()
    {
        uint8_t c = m_registers.read8<Registers::C>();
        uint16_t addr = utils::to16(0xFF, c);
        
        uint8_t val = m_memory.read8(addr);
        m_registers.write8<Registers::A>(val);

        return 2;
    }

    int Processor::ldh_aC_A()
    {
        uint8_t c = m_registers.read8<Registers::C>();
        uint16_t addr = utils::to16(0xFF, c);

        uint8_t a = m_registers.read8<Registers::A>();

        m_memory.write8(addr, a);

        return 2;
    }

    int Processor::ldh_A_an()
    {
        uint8_t n = getImmediate8();
        uint16_t addr = utils::to16(0xFF, n);

        uint8_t val = m_memory.read8(addr);
        m_registers.write8<Registers::A>(val);

        return 3;
    }

    int Processor::ldh_an_A()
    {
        uint8_t n = getImmediate8();
        uint16_t addr = utils::to16(0xFF, n);

        uint8_t a = m_registers.read8<Registers::A>();

        m_memory.write8(addr, a);
        return 3;
    }

    int Processor::ld_HLd_A()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t a = m_registers.read8<Registers::A>();
        m_memory.write8(hl, a);

        m_registers.write16<Registers::HL>(--hl);

        return 2;
    }
        
    int Processor::ld_A_HLd()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);

        m_registers.write16<Registers::HL>(--hl);
        m_registers.write8<Registers::A>(val);

        return 2;
    }
        
    int Processor::ld_HLi_A()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t a = m_registers.read8<Registers::A>();
        m_memory.write8(hl, a);

        m_registers.write16<Registers::HL>(++hl);

        return 2;
    }
    
    int Processor::ld_A_HLi()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);

        m_registers.write16<Registers::HL>(++hl);
        m_registers.write8<Registers::A>(val);

        return 2;
    }

    int Processor::ld_SP_nn()
    {
        uint16_t val = getImmediate16();

        m_registers.setSP(val);

        return 3;
    }

    int Processor::ld_nn_SP()
    {
        uint16_t addr = getImmediate16();

        m_memory.write16(addr, m_registers.getSP());

        return 5;
    }

    int Processor::ld_SP_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        m_registers.setSP(hl);

        return 2;
    }

    int Processor::ld_HL_SP_r8()
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

    void Processor::push(uint16_t val)
    {
        uint16_t sp = m_registers.getSP();
        sp--;
        sp--;

        m_registers.setSP(sp);
        m_memory.write16(m_registers.getSP(), val);
    }

    uint16_t Processor::pop()
    {
        uint16_t sp = m_registers.getSP();
        uint16_t val = m_memory.read16(sp);

        sp++;
        sp++;
        m_registers.setSP(sp);

        return val;
    }

    void Processor::add(int a, int b)
    {
        int res = a + b;
        int carryBits = a ^ b ^ res;

        m_registers.write8<Registers::A>(res);
        updateFlagsWithCarry8bit(carryBits, res, true, false);
    }

    int Processor::add_HL()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();
        
        uint16_t hl = m_registers.read16<Registers::HL>();
        int b = (int8_t) m_memory.read8(hl);
        add(a, b);

        return 2;
    }

    int Processor::add_n()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();
        int b = (int8_t) getImmediate8();
        add(a, b);

        return 2;
    }
    
    void Processor::adc(int a, int b)
    {
        int carryFlag = m_registers.isSetFlag(Registers::Flag::C) ? 1 : 0;
        int res = a + b + carryFlag;
        int carryBits = a ^ b ^ carryFlag ^ res;

        m_registers.write8<Registers::A>(res);
        updateFlagsWithCarry8bit(carryBits, res, true, false);
    }

    int Processor::adc_HL()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        uint16_t hl = m_registers.read16<Registers::HL>();
        int b = (int8_t)m_memory.read8(hl);
        adc(a, b);

        return 2;
    }

    int Processor::adc_n()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        int b = (int8_t) getImmediate8();
        adc(a, b);

        return 2;
    }

    void Processor::sub(int a, int b)
    {
        int res = a - b;
        int carryBits = a ^ b ^ res;

        m_registers.write8<Registers::A>(res);
        updateFlagsWithCarry8bit(carryBits, res, true, true);
    }

    int Processor::sub_HL()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        uint16_t hl = m_registers.read16<Registers::HL>();
        int b = (int8_t)m_memory.read8(hl);
        sub(a, b);

        return 2;
    }

    int Processor::sub_n()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        int b = (int8_t) getImmediate8();
        sub(a, b);

        return 2;
    }

    void Processor::sbc(int a, int b)
    {
        int carryFlag = m_registers.isSetFlag(Registers::Flag::C) ? 1 : 0;
        int res = a - b - carryFlag;
        int carryBits = a ^ b ^ carryFlag ^ res;

        m_registers.write8<Registers::A>(res);
        updateFlagsWithCarry8bit(carryBits, res, true, true);
    }

    int Processor::sbc_HL()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        uint16_t hl = m_registers.read16<Registers::HL>();
        int b = (int8_t)m_memory.read8(hl);
        sbc(a, b);

        return 2;
    }

    int Processor::sbc_n()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();

        int b = (int8_t) getImmediate8();
        sbc(a, b);

        return 2;
    }

    void Processor::cp(int a, int b)
    {
        int res = a - b;
        int carryBits = a ^ b ^ res;

        updateFlagsWithCarry8bit(carryBits, res, true, true);
    }

    int Processor::cp_HL()
    {
        int a = (int8_t)m_registers.read8<Registers::A>();

        uint16_t hl = m_registers.read16<Registers::HL>();
        int b = (int8_t)m_memory.read8(hl);
        cp(a, b);

        return 2;
    }

    int Processor::cp_n()
    {
        int a = (int8_t) m_registers.read8<Registers::A>();

        int b = (int8_t) getImmediate8();
        cp(a, b);

        return 2;
    }

    int Processor::inc_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        int val = (int8_t) m_memory.read8(hl);

        int8_t r = val + 1;
        m_memory.write8(hl, r);

        int carryBits = val ^ 1 ^ r;
        updateFlagsWithCarry8bit(carryBits, r, false, false);

        return 3;
    }

    int Processor::dec_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        int val = (int8_t)m_memory.read8(hl);

        int8_t r = val - 1;
        m_memory.write8(hl, r);

        int carryBits = val ^ 1 ^ r;
        updateFlagsWithCarry8bit(carryBits, r, false, true);

        return 3;
    }

    int Processor::and_HL()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t b = m_memory.read8(hl);
        uint8_t r = a & b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, true, false);

        return 2;
    }

    int Processor::and_n()
    {
        uint8_t a = m_registers.read8<Registers::A>();

        uint8_t b = getImmediate8();
        uint8_t r = a & b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, true, false);
        
        return 2;
    }

    int Processor::or_HL()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t b = m_memory.read8(hl);
        uint8_t r = a | b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, false, false);

        return 2;
    }

    int Processor::or_n()
    {
        uint8_t a = m_registers.read8<Registers::A>();

        uint8_t b = getImmediate8();
        uint8_t r = a | b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, false, false);

        return 2;
    }

    int Processor::xor_HL()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t b = m_memory.read8(hl);
        uint8_t r = a ^ b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, false, false);

        return 2;
    }

    int Processor::xor_n()
    {
        uint8_t a = m_registers.read8<Registers::A>();

        uint8_t b = getImmediate8();
        uint8_t r = a ^ b;

        m_registers.write8<Registers::A>(r);

        updateFlags(r, false, false, false);

        return 2;
    }

    int Processor::scf()
    {
        m_registers.setFlag(Registers::Flag::N, false);
        m_registers.setFlag(Registers::Flag::H, false);
        m_registers.setFlag(Registers::Flag::C, true);

        return 1;
    }

    int Processor::ccf()
    {
        m_registers.setFlag(Registers::Flag::N, false);
        m_registers.setFlag(Registers::Flag::H, false);

        bool c = m_registers.isSetFlag(Registers::Flag::C);
        m_registers.setFlag(Registers::Flag::C, !c);

        return 1;
    }

    int Processor::cpl()
    {
        uint8_t a = m_registers.read8<Registers::A>();
        m_registers.write8<Registers::A>(~a);

        m_registers.setFlag(Registers::Flag::N, true);
        m_registers.setFlag(Registers::Flag::H, true);

        return 1;
    }

    int Processor::daa()
    {
        int a = m_registers.read8<Registers::A>();
        bool isSub = m_registers.isSetFlag(Registers::Flag::N);

        if (!isSub)
        {
            if ((a & 0x0F) > 9 || m_registers.isSetFlag(Registers::Flag::H))
            {
                a += 0x06;
            }

            if (a  > 0x9F || m_registers.isSetFlag(Registers::Flag::C))
            {
                a += 0x60;
            }
        }
        else
        {
            if (m_registers.isSetFlag(Registers::Flag::H))
            {
                a = (a - 0x06) & 0xFF;
            }

            if (m_registers.isSetFlag(Registers::Flag::C))
            {
                a -= 0x60;
            }
        }


        m_registers.setFlag(Registers::Flag::H, false);
        if ((a & 0x100) == 0x100)
        {
            m_registers.setFlag(Registers::Flag::C, (a & 0x100) == 0x100);
        }

        a &= 0xFF;
        m_registers.setFlag(Registers::Flag::Z, a==0);
        m_registers.write8<Registers::A>(a);

        return 1;
    }

    int Processor::add_HL_SP()
    {
        int hl = m_registers.read16<Registers::HL>();
        int sp = m_registers.getSP();
        int res = hl + sp;

        int carryBits = hl ^ sp ^ res;
        updateFlagsWithCarry16bit(carryBits, true, false);

        m_registers.write16<Registers::HL>((uint16_t)res);

        return 2;
    }

    int Processor::add_SP_n()
    {
        int sp = m_registers.getSP();
        int8_t n = (int8_t) getImmediate8();

        int res = sp + n;

        int carryBits = sp ^ n ^ (res & 0xFFFF);
        m_registers.setFlag(Registers::Flag::Z, false);
        
        bool h = (carryBits & 0x10) == 0x10;
        bool c = ((carryBits & 0x100) == 0x100);

        m_registers.setFlag(Registers::Flag::N, false);
        m_registers.setFlag(Registers::Flag::H, h);
        m_registers.setFlag(Registers::Flag::C, c);

        m_registers.setSP((uint16_t)res);

        return 4;
    }

    int Processor::inc_SP()
    {
        uint16_t rr = m_registers.getSP();
        rr++;
        m_registers.setSP(rr);

        return 2;
    }

    int Processor::dec_SP()
    {
        uint16_t rr = m_registers.getSP();
        rr--;
        m_registers.setSP(rr);

        return 2;
    }

    int Processor::rlca()
    {
        rlc_r<Registers::A>();
        m_registers.setFlag(Registers::Flag::Z, false);
        return 1;
    }

    int Processor::rlc_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);
        bool c = (val & 0x80) == 0x80;

        val <<= 1;
        if (c)
        {
            val |= 1;
        }

        updateFlags(val, false, false, c);

        m_memory.write8(hl, val);

        return 4;
    }

    int Processor::rla()
    {
        rl_r<Registers::A>();
        m_registers.setFlag(Registers::Flag::Z, false);
        return 1;
    }

    int Processor::rl_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);
        bool c = (val & 0x80) == 0x80;
        uint16_t oldCarry = m_registers.isSetFlag(Registers::Flag::C) ? 1 : 0;
        val <<= 1;
        val |= oldCarry;

        updateFlags(val, false, false, c);

        m_memory.write8(hl, val);

        return 4;
    }

    int Processor::rrca()
    {
        rrc_r<Registers::A>();
        m_registers.setFlag(Registers::Flag::Z, false);
        return 1;
    }
    int Processor::rrc_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);
        bool c = (val & 1) == 1;

        val >>= 1;
        if (c)
        {
            val |= 0x80;
        }

        updateFlags(val, false, false, c);

        m_memory.write8(hl, val);

        return 4;
    }


    int Processor::rra()
    {
        rr_r<Registers::A>();
        m_registers.setFlag(Registers::Flag::Z, false);
        return 1;
    }
    
    int Processor::rr_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);
        bool c = (val & 1) == 1;
        uint16_t oldCarry = m_registers.isSetFlag(Registers::Flag::C) ? 0X8000 : 0;
        val >>= 1;
        val |= oldCarry;

        updateFlags(val, false, false, c);

        m_memory.write8(hl, val);

        return 4;
    }

    int Processor::sla_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);
        bool c = (hl & 0x80) == 0x80;
        hl <<= 1;

        updateFlags(val, false, false, c);
        m_memory.write8(hl, val);

        return 4;
    }

    int Processor::sra_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);
        bool c = (val & 1) == 1;
        if ((val & 0x80) != 0)
        {
            val >>= 1;
            val |= 0x80;
        }
        else
        {
            val >>= 1;
        }
        
        updateFlags(val, false, false, c);
        m_memory.write8(hl, val);

        return 4;
    }

    int Processor::srl_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);
        bool c = (val & 1) == 1;
        val >>= 1;

        updateFlags(val, false, false, c);
        m_memory.write8(hl, val);

        return 4;
    }

    int Processor::swap_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        uint8_t val = m_memory.read8(hl);
        val = (val << 4) | (val >> 4);

        updateFlags(val, false, false, false);
        m_memory.write8(hl, val);

        return 4;
    }

    void Processor::bit_n(uint8_t n, uint16_t r)
    {
        uint16_t b = 1 << n;
        bool isSet = (b & r) == 0;

        m_registers.setFlag(Registers::Flag::Z, isSet);
        m_registers.setFlag(Registers::Flag::N, 0);
        m_registers.setFlag(Registers::Flag::H, 1);
    }

    int Processor::jp_nn()
    {
        uint16_t immediate = getImmediate16();
        m_registers.setPC(immediate);

        return 4;
    }

    int Processor::jp_HL()
    {
        uint16_t hl = m_registers.read16<Registers::HL>();
        m_registers.setPC(hl);

        return 1;
    }

    int Processor::jr_e()
    {
        int8_t e = (int8_t) getImmediate8();
        uint16_t pc = m_registers.getPC();
        m_registers.setPC(pc + e);

        return 3;
    }

    int Processor::call_nn()
    {
        uint16_t addr = getImmediate16();
        
        uint16_t pc = m_registers.getPC();
        push(pc);

        m_registers.setPC(addr);
        return 6;
    }

    int Processor::ret()
    {
        uint16_t addr = pop();
        m_registers.setPC(addr);

        return 4;
    }

    int Processor::reti()
    {
        m_IME = true;
        return ret();
    }

    int Processor::di()
    {
        m_IME = false;
        return 1;
    }

    int Processor::ei()
    {
        m_IME = true;
        return 1;
    }

    int Processor::halt()
    {
        m_isHalt = true;
        return 1;
    }

    int Processor::stop()
    {
        getImmediate8();
        m_isStopped = true;
        return 1;
    }
}