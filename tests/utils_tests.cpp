#include <gtest/gtest.h>

#include "utils/utils.h"

namespace
{
struct test
{
	uint16_t input;
	uint8_t low;
	uint8_t high;
};

std::vector<test> getTestCases()
{
	return {
		{0b1100'1100'0101'0101, 0b0101'0101, 0b1100'1100},
		{0b1100'1100'1111'0101, 0b1111'0101, 0b1100'1100},
		{0b1100'1100'0101'1111, 0b0101'1111, 0b1100'1100},
		{0b1111'0101'1100'1100, 0b1100'1100, 0b1111'0101},
		{0b0101'1111'1100'1100, 0b1100'1100, 0b0101'1111}
	};
}

class UtilsTests : public testing::TestWithParam<test>
{};

INSTANTIATE_TEST_CASE_P(UtilsTests, UtilsTests, testing::ValuesIn(getTestCases()));

TEST_P(UtilsTests, low)
{
	auto param = GetParam();
	uint16_t in = param.input;
	uint8_t expected = param.low;
	uint8_t res = utils::low(in);
	EXPECT_EQ(expected, res);
}

TEST_P(UtilsTests, high)
{
	auto param = GetParam();
	uint16_t in = param.input;
	uint8_t expected = param.high;
	uint8_t res = utils::high(in);
	EXPECT_EQ(expected, res);
}

TEST_P(UtilsTests, to16)
{
	auto param = GetParam();
	uint8_t msb = param.high;
	uint8_t lsb = param.low;
	uint16_t expected = param.input;
	uint16_t res = utils::to16(msb, lsb);
	EXPECT_EQ(expected, res);
}
}
