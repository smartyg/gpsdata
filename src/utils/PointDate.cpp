#include "config.h"

#include "gpsdata/utils/PointDate.hpp"

#include <memory>
#include <sstream>
#include <string>
#include <chrono>
#include <cstring>
#include <date/date.h>
#include <zonedetect.h>

#include "gpsdata/utils/ZoneDate.hpp"
#include "gpsdata/types/ObjectTime.hpp"

using gpsdata::utils::PointDate;
using gpsdata::utils::ZoneDate;
using gpsdata::ObjectTime;

PointDate::PointDate (void) {
		ZDSetErrorHandler ([](int err_zd, int err_native) {
			(void) err_native;
			throw std::runtime_error (ZDGetErrorString (err_zd));
		});

		this->_zone_database = ZDOpenDatabase (TIMEZONE_FILE);
		//this->_zone_database = ZDOpenDatabase ("./timezone21.bin");
		if (!this->_zone_database) throw std::runtime_error ("Error opening timezone database.");
}

PointDate::~PointDate (void) noexcept {
	if (this->_zone_database)
		ZDCloseDatabase (this->_zone_database);
}

const ZoneDetect* PointDate::getZoneDatabasePtr (void) {
	static PointDate instance;
	return instance._zone_database;
}

const std::shared_ptr<ZoneDate> PointDate::getTimeZone (const double& lat, const double& lon) noexcept {
	ZoneDetectResult *result = ZDLookup (PointDate::getZoneDatabasePtr (), static_cast<float>(lat), static_cast<float>(lon), nullptr);
	const std::shared_ptr<ZoneDate> zone_date = ZoneDate::create (lat, lon, result);
	ZDFreeResults (result);
	return zone_date;
}

const std::shared_ptr<ZoneDate> PointDate::getTimeZone (const std::string& zone_name) noexcept {

	const std::shared_ptr<ZoneDate> zone_date = ZoneDate::create (zone_name);
	return zone_date;
}

const date::local_time<std::chrono::milliseconds> PointDate::parseTimeInt (const std::string& time_string, const std::vector<std::string>& formats) {
	date::local_time<std::chrono::milliseconds> tp;
	unsigned int i = 0;
	std::istringstream in;
	in.imbue (std::locale::classic ());

	// Loop over the formats and try till one succeeds.
	do {
		// Clear any error bits.
		in.clear ();
		// Construct a new stream object with time_string a content.
		in.str (time_string);
		// try matching the string against the next format in the list.
		date::from_stream (in, formats[i].c_str (), tp);

		++i;
	} while ((in.fail () || !(in.eof () || in.peek() == std::char_traits<char>::eof())) && i < formats.size ());
	// if in.fail() is true, we ran out of formats to try and none matches.
	if (!in.eof () || in.fail ()) throw std::runtime_error("no matching format found.");
	// (at least) one format matched.
	return tp;
}

const ObjectTime PointDate::parseTime (const std::string& time_string, const std::vector<std::string>& formats) {
	const date::local_time<std::chrono::milliseconds> tp = PointDate::parseTimeInt (time_string, formats);
	return ObjectTime (tp.time_since_epoch());
}

const date::zoned_time<ObjectTime::timeType> PointDate::makeZonedTime (const double& lat, const double& lon, const ObjectTime& time) {
	//const auto zd = PointDate::getTimeZone (lat, lon);
	const std::chrono::time_point<std::chrono::system_clock, ObjectTime::Representation> t(time.getTime ());
	const date::zoned_time ret(PointDate::getTimeZone (lat, lon)->getZonePtr (), t);
	return ret;
}
