#include "config.h"

#include "gpsdata/utils/PointDate.hpp"

#include <date/date.h>
#include <zonedetect.h>
#include <chrono>
#include <sstream>
#include <cstring>
#include <memory>

#include "gpsdata/utils/Logger.hpp"
#include "gpsdata/utils/ZoneDate.hpp"
#include "gpsdata/types/ObjectTime.hpp"

using gpsdata::utils::PointDate;
using gpsdata::utils::ZoneDate;
using gpsdata::ObjectTime;

const ZoneDetect *PointDate::_zone_database = nullptr;
std::string PointDate::_db_path = {};

bool PointDate::init (const std::string& path = {}) {
	if (PointDate::_zone_database == nullptr && !path.empty ()) {
		PointDate::_db_path = path;
		ZDSetErrorHandler([](int err_zd, int err_native){
			(void) err_native;
			throw std::runtime_error(ZDGetErrorString(err_zd));
		});

		const ZoneDetect *db = ZDOpenDatabase (PointDate::_db_path.c_str ());
		if(!db) throw std::runtime_error("Error opening timezone database.");
		PointDate::_zone_database = db;
		return true;
	} else if (PointDate::_zone_database == nullptr && path.empty ()) {
		throw std::runtime_error("no timezone database path was given.");
	}
	return false;
}

void PointDate::destroy (void) noexcept {
	if (PointDate::_zone_database != nullptr)
		ZDCloseDatabase(const_cast<ZoneDetect *>(PointDate::_zone_database));
	PointDate::_db_path.clear ();
	PointDate::_zone_database = nullptr;
}

bool PointDate::isValid (void) noexcept {
	return (PointDate::_zone_database != nullptr);
}

const std::shared_ptr<ZoneDate> PointDate::getTimeZone (const double& lat, const double& lon) noexcept {
	if (!PointDate::isValid ()) return nullptr;
	ZoneDetectResult *result = ZDLookup (PointDate::_zone_database, static_cast<float>(lat), static_cast<float>(lon), nullptr);
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


