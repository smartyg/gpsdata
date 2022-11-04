#include "config.h"

#include <gtest/gtest.h>

#include "gpsdata.hpp"
#include "gpsdata/utils/PointDate.hpp"
#include "gpsdata/utils/ZoneDate.hpp"
#include "gpsdata/utils/GpsDataFactoryBasic.hpp"

using gpsdata::utils::PointDate;
using gpsdata::utils::ZoneDate;
using gpsdata::GpsPoint;
using gpsdata::utils::GpsDataFactoryBasic;

TEST(PointDateTest, InitTest1)
{
	const std::shared_ptr<ZoneDate> zd = PointDate::getTimeZone ("Europe/Amsterdam");

	ASSERT_NE(zd, nullptr);
	EXPECT_FALSE(zd->getZoneName ().empty ());
	EXPECT_STREQ(zd->getZoneName ().c_str (), "Europe/Amsterdam");
	ASSERT_TRUE(zd->getCountryCode ().empty ());
	ASSERT_TRUE(zd->getCountryName ().empty ());
}

TEST(PointDateTest, InitTest2)
{
	const std::shared_ptr<ZoneDate> zd = PointDate::getTimeZone(52.0, 4.0);

	ASSERT_NE(zd, nullptr);
	EXPECT_FALSE(zd->getZoneName().empty ());
	EXPECT_STREQ(zd->getZoneName().c_str (), "Europe/Amsterdam");
	EXPECT_FALSE(zd->getCountryCode().empty ());
	EXPECT_STREQ(zd->getCountryCode().c_str (), "NL");
	EXPECT_FALSE(zd->getCountryName().empty ());
	EXPECT_STREQ(zd->getCountryName().c_str (), "Netherlands");
}

TEST(PointDateTest, InitTest3)
{
	const std::shared_ptr<ZoneDate> zd = PointDate::getTimeZone(52.0, 13.0);

	EXPECT_STREQ(zd->getZoneName().c_str (), "Europe/Berlin");
	EXPECT_STREQ(zd->getCountryCode().c_str (), "DE");
	EXPECT_STREQ(zd->getCountryName().c_str (), "Germany");
}

TEST(PointDateTest, parseTime)
{
	EXPECT_EQ(PointDate::parseTime("12-06-2020 05:20:56", { "%d-%m-%Y %T" }).get (), 1591939256000);
	EXPECT_EQ(PointDate::parseTime("12-06-2020 05:20:56", { "%T", "%d-%m-%Y", "%d-%m-%Y %T" }).get (), 1591939256000);
	EXPECT_EQ(PointDate::parseTime("12-06-2020 05:20:56", { "%d-%m-%Y %T", "%d-%m-%Y", "%T" }).get (), 1591939256000);
	EXPECT_EQ(PointDate::parseTime("12-06-2020 05:20:56.98", { "%T", "%d-%m-%Y", "%d-%m-%Y %T" }).get (), 1591939256980);
	EXPECT_EQ(PointDate::parseTime("12-06-2020 05:20:56.981", { "%T", "%d-%m-%Y", "%d-%m-%Y %T" }).get (), 1591939256981);
	EXPECT_EQ(PointDate::parseTime("12-06-2020 05:20:56.980", { "%T", "%d-%m-%Y", "%d-%m-%Y %T" }).get (), 1591939256980);
	EXPECT_THROW(PointDate::parseTime("12-06-2020 05:20:56", { "%T", "%d-%m-%Y" }), std::runtime_error);
	EXPECT_THROW(PointDate::parseTime("12-14-2020 05:20:56", { "%T", "%d-%m-%Y", "%d-%m-%Y %T" }), std::runtime_error);
}

TEST(PointDateTest, convertToTimeT1)
{
	EXPECT_EQ(PointDate::convertToTimeT(1591939256000), 1591939256);
	EXPECT_EQ(PointDate::convertToTimeT(1591939256001), 1591939256);
	EXPECT_EQ(PointDate::convertToTimeT(1591939256999), 1591939256);
	EXPECT_EQ(PointDate::convertToTimeT(1591939257000), 1591939257);
}

TEST(PointDateTest, convertToTimeT2)
{
	std::chrono::milliseconds t{1591939256000};
	EXPECT_EQ(PointDate::convertToTimeT(t), 1591939256);
}

TEST(PointDateTest, getPointLocalTime)
{
	gpsdata::ObjectTime t1 (2015, 3, 15, 14, 31, 23.012);
	const auto point = GpsPoint<GpsDataFactoryBasic>::template create<GpsPoint<GpsDataFactoryBasic>>(t1, GpsDataFactoryBasic::create ());
	point->addData ("LON", 1300000000, true);
	point->addData ("LAT", 5200000800, true);
	const auto zoned_time = PointDate::getZonedTime (point);
	EXPECT_EQ(zoned_time.get_sys_time ().time_since_epoch ().count (), t1.getTime ().count ());
	gpsdata::ObjectTime t_ref (2015, 3, 15, 15, 31, 23.012);
	EXPECT_EQ(zoned_time.get_local_time ().time_since_epoch ().count (), t_ref.getTime ().count ());
	EXPECT_STREQ(date::format("%F %T %z", zoned_time).c_str (), "2015-03-15 15:31:23.012 +0100");
}
