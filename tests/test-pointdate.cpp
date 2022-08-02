#include "config.h"

#include <gtest/gtest.h>

#include "gpsdata/utils/PointDate.hpp"
#include "gpsdata/utils/ZoneDate.hpp"

using gpsdata::utils::PointDate;
using gpsdata::utils::ZoneDate;

class PointDateTest : public testing::Test {
public:
	// Per-test-suite set-up.
	// Called before the first test in this test suite.
	// Can be omitted if not needed.
	static void SetUpTestSuite() {
		// Avoid reallocating static objects if called in subclasses of FooTest.
		PointDate::init("./out_v1/timezone21.bin");
	}

	// Per-test-suite tear-down.
	// Called after the last test in this test suite.
	// Can be omitted if not needed.
	static void TearDownTestSuite() {
		PointDate::destroy ();
	}
};

TEST_F(PointDateTest, Valid)
{
	EXPECT_TRUE(PointDate::isValid());
}

TEST_F(PointDateTest, InitTest1)
{
	ASSERT_TRUE(PointDate::isValid());

	const std::shared_ptr<ZoneDate> zd = PointDate::getTimeZone ("Europe/Amsterdam");

	ASSERT_NE(zd, nullptr);
	EXPECT_FALSE(zd->getZoneName ().empty ());
	EXPECT_STREQ(zd->getZoneName ().c_str (), "Europe/Amsterdam");
	ASSERT_TRUE(zd->getCountryCode ().empty ());
	ASSERT_TRUE(zd->getCountryName ().empty ());
}

TEST_F(PointDateTest, InitTest2)
{
	ASSERT_TRUE(PointDate::isValid());

	const std::shared_ptr<ZoneDate> zd = PointDate::getTimeZone(52.0, 4.0);

	ASSERT_NE(zd, nullptr);
	EXPECT_FALSE(zd->getZoneName().empty ());
	EXPECT_STREQ(zd->getZoneName().c_str (), "Europe/Amsterdam");
	EXPECT_FALSE(zd->getCountryCode().empty ());
	EXPECT_STREQ(zd->getCountryCode().c_str (), "NL");
	EXPECT_FALSE(zd->getCountryName().empty ());
	EXPECT_STREQ(zd->getCountryName().c_str (), "Netherlands");
}

TEST_F(PointDateTest, InitTest3)
{
	ASSERT_TRUE(PointDate::isValid());

	const std::shared_ptr<ZoneDate> zd = PointDate::getTimeZone(52.0, 13.0);

	EXPECT_STREQ(zd->getZoneName().c_str (), "Europe/Berlin");
	EXPECT_STREQ(zd->getCountryCode().c_str (), "DE");
	EXPECT_STREQ(zd->getCountryName().c_str (), "Germany");
}

TEST_F(PointDateTest, parseTime)
{
	EXPECT_EQ(PointDate::parseTime("12-06-2020 05:20:56", { "%d-%m-%Y %T" }).getTime (), 1591939256000);
	EXPECT_EQ(PointDate::parseTime("12-06-2020 05:20:56", { "%T", "%d-%m-%Y", "%d-%m-%Y %T" }).getTime (), 1591939256000);
	EXPECT_EQ(PointDate::parseTime("12-06-2020 05:20:56", { "%d-%m-%Y %T", "%d-%m-%Y", "%T" }).getTime (), 1591939256000);
	EXPECT_EQ(PointDate::parseTime("12-06-2020 05:20:56.98", { "%T", "%d-%m-%Y", "%d-%m-%Y %T" }).getTime (), 1591939256980);
	EXPECT_EQ(PointDate::parseTime("12-06-2020 05:20:56.981", { "%T", "%d-%m-%Y", "%d-%m-%Y %T" }).getTime (), 1591939256981);
	EXPECT_EQ(PointDate::parseTime("12-06-2020 05:20:56.980", { "%T", "%d-%m-%Y", "%d-%m-%Y %T" }).getTime (), 1591939256980);
	EXPECT_THROW(PointDate::parseTime("12-06-2020 05:20:56", { "%T", "%d-%m-%Y" }), std::runtime_error);
	EXPECT_THROW(PointDate::parseTime("12-14-2020 05:20:56", { "%T", "%d-%m-%Y", "%d-%m-%Y %T" }), std::runtime_error);
}

TEST_F(PointDateTest, convertToTimeT1)
{
	EXPECT_EQ(PointDate::convertToTimeT(1591939256000), 1591939256);
	EXPECT_EQ(PointDate::convertToTimeT(1591939256001), 1591939256);
	EXPECT_EQ(PointDate::convertToTimeT(1591939256999), 1591939256);
	EXPECT_EQ(PointDate::convertToTimeT(1591939257000), 1591939257);
}

TEST_F(PointDateTest, convertToTimeT2)
{
	std::chrono::milliseconds t{1591939256000};
	EXPECT_EQ(PointDate::convertToTimeT(t), 1591939256);
}