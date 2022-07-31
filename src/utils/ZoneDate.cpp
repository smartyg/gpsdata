#include "config.h"

#include "gpsdata/utils/ZoneDate.hpp"

#include <date/date.h>
#include <date/tz.h>
#include <zonedetect.h>
#include <cstring>
#include <memory>

#include "gpsdata/types/ObjectTime.hpp"

using gpsdata::utils::ZoneDate;
using gpsdata::ObjectTime;

ZoneDate::ZoneDate (const double& latitude, const double& longitude, const ZoneDetectResult *zone) {
	this->_latitude = latitude;
	this->_longitude = longitude;

	unsigned int index = 0;
	char *tmp_timezone_prefix = nullptr;
	char *tmp_timezone_id = nullptr;
	char *tmp_country_code = nullptr;
	char *tmp_country_name = nullptr;
	while (zone[index].lookupResult != ZD_LOOKUP_END) {
		if (zone[index].data) {
			for (unsigned int i = 0; i < zone[index].numFields; i++) {
				if (strcmp(zone[index].fieldNames[i], "TimezoneIdPrefix") == 0) {
					tmp_timezone_prefix = zone[index].data[i];
				} else if (strcmp(zone[index].fieldNames[i], "TimezoneId") == 0) {
					tmp_timezone_id = zone[index].data[i];
				} else if (strcmp(zone[index].fieldNames[i], "CountryAlpha2") == 0) {
					tmp_country_code = zone[index].data[i];
				} else if (strcmp(zone[index].fieldNames[i], "CountryName") == 0) {
					tmp_country_name = zone[index].data[i];
				}
			}
		}

		index++;
	}

	this->_zone_name = std::string (tmp_timezone_prefix) + std::string (tmp_timezone_id);
	this->_country_name = std::string (tmp_country_name);
	this->_country_code = std::string (tmp_country_code);
	this->_zone = date::locate_zone (this->_zone_name.c_str ());
}

ZoneDate::ZoneDate (const double& latitude, const double& longitude, const std::string& zone_name) {
	this->_latitude = latitude;
	this->_longitude = longitude;
	this->_country_name.clear ();
	this->_country_code.clear ();
	this->_zone_name = std::string (zone_name);
	this->_zone = date::locate_zone (this->_zone_name.c_str ());
}

ZoneDate::ZoneDate (const std::string& zone_name) {
	this->_latitude = nan ("");
	this->_longitude = nan ("");
	this->_country_name.clear ();
	this->_country_code.clear ();
	this->_zone_name = std::string (zone_name);
	this->_zone = date::locate_zone (this->_zone_name.c_str ());
}

ZoneDate::~ZoneDate (void) {
	this->_country_name.clear ();
	this->_country_code.clear ();
	this->_zone_name.clear ();
	this->_zone = nullptr;
}

const std::string ZoneDate::getZoneName (void) const noexcept {
	return this->_zone->name ();
}

const std::string ZoneDate::getCountryCode (void) const noexcept {
	return this->_country_code;
}

const std::string ZoneDate::getCountryName (void) const noexcept {
	return this->_country_name;
}

date::local_time<std::chrono::milliseconds> ZoneDate::getUtcTime (const date::sys_time<std::chrono::milliseconds>& tp) const noexcept {
	return this->_zone->to_local (tp);
}

const ObjectTime ZoneDate::getUtcTimeAsObjectTime (const date::sys_time<std::chrono::milliseconds>& tp) const noexcept {
	date::local_time<std::chrono::milliseconds> local_tp = this->getUtcTime (tp);
	return local_tp.time_since_epoch ();
}

date::sys_time<std::chrono::milliseconds> ZoneDate::getZoneTime (const date::local_time<std::chrono::milliseconds>& utc_tp) const noexcept {
	return this->_zone->to_sys (utc_tp, date::choose::latest);
}

const date::sys_info ZoneDate::getZoneInfo (const date::local_time<std::chrono::milliseconds>& utc_tp) const noexcept {
	const date::local_info li = this->_zone->get_info (utc_tp);
	return li.first;
}

int ZoneDate::getUtcOffset (const date::local_time<std::chrono::milliseconds>& utc_tp) const noexcept {
	const date::sys_info si = this->getZoneInfo (utc_tp);
	return si.offset.count();
}
