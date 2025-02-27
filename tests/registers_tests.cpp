#include <gtest/gtest.h>

#include "cpu/registery.h"

namespace
{ 

TEST(RegistersTests, default)
{
	cpu::Registers r;
	EXPECT_EQ(r.read8<cpu::Registers::A>(), 0);
	EXPECT_EQ(r.read8<cpu::Registers::F>(), 0);
	EXPECT_EQ(r.read8<cpu::Registers::B>(), 0);
	EXPECT_EQ(r.read8<cpu::Registers::C>(), 0);
	EXPECT_EQ(r.read8<cpu::Registers::D>(), 0);
	EXPECT_EQ(r.read8<cpu::Registers::E>(), 0);
	EXPECT_EQ(r.read8<cpu::Registers::H>(), 0);
	EXPECT_EQ(r.read8<cpu::Registers::L>(), 0);
}

TEST(RegistersTests, writeAndRead8)
{
	cpu::Registers registers;

	// Arbitrary value
	uint8_t val = 0xD3;

	registers.write8<cpu::Registers::A>(val);
	registers.write8<cpu::Registers::F>(val);
	registers.write8<cpu::Registers::B>(val); 
	registers.write8<cpu::Registers::C>(val);
	registers.write8<cpu::Registers::D>(val); 
	registers.write8<cpu::Registers::E>(val);
	registers.write8<cpu::Registers::H>(val); 
	registers.write8<cpu::Registers::L>(val);

	EXPECT_EQ(registers.read8<cpu::Registers::A>(), val);
	EXPECT_EQ(registers.read8<cpu::Registers::F>(), val);
	EXPECT_EQ(registers.read8<cpu::Registers::B>(), val);
	EXPECT_EQ(registers.read8<cpu::Registers::C>(), val);
	EXPECT_EQ(registers.read8<cpu::Registers::D>(), val);
	EXPECT_EQ(registers.read8<cpu::Registers::E>(), val);
	EXPECT_EQ(registers.read8<cpu::Registers::H>(), val);
	EXPECT_EQ(registers.read8<cpu::Registers::L>(), val);
}

TEST(RegistersTests, writeAndRead16)
{
	cpu::Registers registers;

	// Arbitrary value
	uint16_t val = 0xD3'6C;

	registers.write16<cpu::Registers::AF>(val);
	registers.write16<cpu::Registers::BC>(val);
	registers.write16<cpu::Registers::DE>(val);
	registers.write16<cpu::Registers::HL>(val);

	EXPECT_EQ(registers.read16<cpu::Registers::AF>(), val);
	EXPECT_EQ(registers.read16<cpu::Registers::BC>(), val);
	EXPECT_EQ(registers.read16<cpu::Registers::DE>(), val);
	EXPECT_EQ(registers.read16<cpu::Registers::HL>(), val);
}

struct test
{
	std::vector<std::pair<cpu::Registers::Flag, bool>> operations;
	uint8_t reusltingFlag;
};

std::vector<test> getTestCases()
{
	using flag = cpu::Registers::Flag;
	return 
	{
		{ {{flag::C, true}}, 0x10},
		{ {{flag::H, true}}, 0x20},
		{ {{flag::N, true}}, 0x40},
		{ {{flag::Z, true}}, 0x80},
		
		{ {{flag::C, true}, {flag::C, false}}, 0},
		{ {{flag::H, true}, {flag::H, false}}, 0},
		{ {{flag::N, true}, {flag::N, false}}, 0},
		{ {{flag::Z, true}, {flag::Z, false}}, 0},
		
		{ { {flag::C, true}, {flag::H, true}, 
		{flag::N, true}, {flag::Z, true}}, 0xF0},

		{ { {flag::C, true}, {flag::H, true},
		{flag::N, true}, {flag::C, false}}, 0x60}
	};
}

class FlagsTests : public testing::TestWithParam<test>
{};
INSTANTIATE_TEST_CASE_P(FlagsTests, FlagsTests, testing::ValuesIn(getTestCases()));

TEST_P(FlagsTests, editFlags)
{
	auto param = GetParam();

	cpu::Registers r;
	for (auto [flag, enabled] : param.operations)
	{
		r.setFlag(flag, enabled);
	}

	EXPECT_EQ(param.reusltingFlag, r.read8<cpu::Registers::F>());
}
}
