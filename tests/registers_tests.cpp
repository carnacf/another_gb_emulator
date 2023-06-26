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
		{ {{flag::carry, true}}, 0x10},
		{ {{flag::half_carry, true}}, 0x20},
		{ {{flag::substract, true}}, 0x40},
		{ {{flag::zero, true}}, 0x80},
		
		{ {{flag::carry, true}, {flag::carry, false}}, 0},
		{ {{flag::half_carry, true}, {flag::half_carry, false}}, 0},
		{ {{flag::substract, true}, {flag::substract, false}}, 0},
		{ {{flag::zero, true}, {flag::zero, false}}, 0},
		
		{ { {flag::carry, true}, {flag::half_carry, true}, 
		{flag::substract, true}, {flag::zero, true}}, 0xF0},

		{ { {flag::carry, true}, {flag::half_carry, true},
		{flag::substract, true}, {flag::carry, false}}, 0x60}
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
		r.enableFlag(flag, enabled);
	}

	EXPECT_EQ(param.reusltingFlag, r.read8<cpu::Registers::F>());
}
}
