#ifndef _X_GPSDATA_ZONEDATE_
#define _X_GPSDATA_ZONEDATE_

namespace gpsdata::utils { class ZoneDate; }

#include <date/date.h>
#include <date/tz.h>
#include <zonedetect.h>

#include <gpsdata/utils/PointDate.hpp>
#include <gpsdata/types/ObjectTime.hpp>

namespace gpsdata::utils {
	class ZoneDate final {
		friend class PointDate;

	protected:
		ZoneDate (double lat, double lon, const ZoneDetectResult *);

	public:
		ZoneDate (double lat, double lon, const std::string&);
		ZoneDate (const std::string&);
		~ZoneDate (void);

		const std::string getZoneName (void) const noexcept;
		const std::string getCountryCode (void) const noexcept;
		const std::string getCountryName (void) const noexcept;
		date::sys_time<std::chrono::milliseconds> getZoneTime (const ObjectTime& utc_time) const noexcept;
		date::sys_time<std::chrono::milliseconds> getZoneTime (std::chrono::milliseconds utc_tp) const noexcept;
		date::local_time<std::chrono::milliseconds> getUtcTime (date::sys_time<std::chrono::milliseconds> tp) const noexcept;
		const ObjectTime getUtcTimeAsObjectTime (date::sys_time<std::chrono::milliseconds> tp) const noexcept;
		int getUtcOffset (const ObjectTime& utc_time) const noexcept;
		int getUtcOffset (std::chrono::milliseconds utc_time) const noexcept;
		const date::sys_info getZoneInfo (const ObjectTime& utc_time) const noexcept;
		const date::sys_info getZoneInfo (std::chrono::milliseconds utc_time) const noexcept;

	private:
		const date::time_zone *_zone;
		std::string _zone_name;
		std::string _country_name;
		std::string _country_code;
		double _latitude;
		double _longitude;

		date::sys_time<std::chrono::milliseconds> getZoneTime (date::local_time<std::chrono::milliseconds> utc_tp) const noexcept;
		const date::sys_info getZoneInfo (date::local_time<std::chrono::milliseconds> utc_tp) const noexcept;
		inline int getUtcOffset (date::local_time<std::chrono::milliseconds> utc_tp) const noexcept;
	};

	const ZoneDate *create_zonedate (const std::string&);
}

#endif /* _X_GPSDATA_ZONEDATE_ */