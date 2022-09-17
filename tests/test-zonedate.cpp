#include <gtest/gtest.h>

#include "config.h"

#include "gpsdata/utils/ZoneDate.hpp"

#include <cstring>

using gpsdata::utils::ZoneDate;

TEST(ZoneDateTest, InitTest)
{
	std::shared_ptr<ZoneDate> zd;
	EXPECT_NO_THROW((zd = gpsdata::utils::ZoneDate::create ("Europe/Amsterdam")));

	EXPECT_STREQ(zd->getZoneName().c_str (), "Europe/Amsterdam");
	EXPECT_TRUE(zd->getCountryCode().empty ());
	EXPECT_TRUE(zd->getCountryName().empty ());
}

TEST(ZoneDateTest, InitFailTest)
{
	std::shared_ptr<ZoneDate> zd;
	EXPECT_THROW((zd = ZoneDate::create ("Disney/Mickey_Mouse")), std::runtime_error);
}

TEST(ZoneDateTest, getZoneTime1)
{
	std::shared_ptr<ZoneDate> zd;
	ASSERT_NO_THROW((zd = ZoneDate::create ("Europe/London")));

	auto zone_time = zd->getZoneTime (1634419175000);
	EXPECT_EQ(zone_time.time_since_epoch ().count (), 1634415575000);
}

TEST(ZoneDateTest, getZoneTime2)
{
	std::shared_ptr<ZoneDate> zd;
	ASSERT_NO_THROW((zd = ZoneDate::create ("Europe/Amsterdam")));

	std::chrono::milliseconds t{1634419175000};

	auto zone_time = zd->getZoneTime(t);
	EXPECT_EQ(zone_time.time_since_epoch().count(), 1634411975000);
}

TEST(ZoneDateTest, getUtcTime1)
{
	std::shared_ptr<ZoneDate> zd;
	ASSERT_NO_THROW((zd = ZoneDate::create ("Europe/Amsterdam")));

	auto zone_time = zd->getZoneTime(1634419175000);
	auto utc_time = zd->getUtcTime(zone_time);
	EXPECT_EQ(utc_time.time_since_epoch().count(), 1634419175000);
}

TEST(ZoneDateTest, getUtcTime2)
{
	std::shared_ptr<ZoneDate> zd;
	ASSERT_NO_THROW((zd = ZoneDate::create ("Asia/Bangkok")));

	auto zone_time = zd->getZoneTime(1634419175000);
	auto utc_time = zd->getUtcTime(zone_time);
	EXPECT_EQ(utc_time.time_since_epoch().count(), 1634419175000);
}

TEST(ZoneDateTest, getUtcTime3)
{
	std::shared_ptr<ZoneDate> zd;
	ASSERT_NO_THROW((zd = ZoneDate::create ("Asia/Jakarta")));

	auto zone_time = zd->getZoneTime(1634419175000);
	EXPECT_EQ(zd->getUtcTimeAsObjectTime(zone_time).get (), 1634419175000);
}

TEST(ZoneDateTest, getUtcOffset1)
{
	std::shared_ptr<ZoneDate> zd;
	ASSERT_NO_THROW((zd = ZoneDate::create ("Europe/Amsterdam")));

	EXPECT_EQ(zd->getUtcOffset(1634419175000), 7200);
}

TEST(ZoneDateTest, getUtcOffset2)
{
	std::shared_ptr<ZoneDate> zd;
	ASSERT_NO_THROW((zd = ZoneDate::create ("Europe/Amsterdam")));

	std::chrono::milliseconds utc_time{1637093975000};
	EXPECT_EQ(zd->getUtcOffset(utc_time), 3600);
}

TEST(ZoneDateTest, getZoneInfo)
{
	std::shared_ptr<ZoneDate> zd;
	ASSERT_NO_THROW((zd = ZoneDate::create ("Europe/Amsterdam")));

	date::sys_info info = zd->getZoneInfo(1637093975000);
	EXPECT_EQ(info.begin.time_since_epoch().count(), 1635642000);
	EXPECT_EQ(info.end.time_since_epoch().count(), 1648342800);
	EXPECT_EQ(info.offset.count(), 3600);
	EXPECT_EQ(info.save.count(), 0);
	EXPECT_STREQ(info.abbrev.c_str(), "CET");
}
