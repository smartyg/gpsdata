#ifndef _X_GPSDATA_ZONEDATE_
#define _X_GPSDATA_ZONEDATE_

namespace gpsdata::utils { class ZoneDate; }

#include <date/date.h>
#include <date/tz.h>
#include <zonedetect.h>
#include <memory>

#include <gpsdata/utils/PointDate.hpp>
#include <gpsdata/types/ObjectTime.hpp>

namespace gpsdata::utils {
	class ZoneDate final : std::enable_shared_from_this<ZoneDate> {
		const date::time_zone* _zone;
		std::string _zone_name;
		std::string _country_name;
		std::string _country_code;
		double _latitude;
		double _longitude;

		ZoneDate (const double& lat, const double& lon, const ZoneDetectResult *);
		ZoneDate (const double& lat, const double& lon, const std::string&);
		ZoneDate (const std::string&);

		ZoneDate (void) = delete;
		ZoneDate (const ZoneDate&) = delete;                // copy constructor
		ZoneDate (ZoneDate&&) noexcept = delete;            // move constructor
		ZoneDate& operator= (const ZoneDate&) = delete;     // copy assignment
		ZoneDate& operator= (ZoneDate&&) noexcept = delete; // move assignment

	public:
		~ZoneDate (void);

		const std::string getZoneName (void) const noexcept;
		const std::string getCountryCode (void) const noexcept;
		const std::string getCountryName (void) const noexcept;

		date::local_time<std::chrono::milliseconds> getUtcTime (const date::sys_time<std::chrono::milliseconds>&) const noexcept;
		const ObjectTime getUtcTimeAsObjectTime (const date::sys_time<std::chrono::milliseconds>&) const noexcept;

		inline date::sys_time<std::chrono::milliseconds> getZoneTime (const ObjectTime::internalTimeType& utc_time) const noexcept {
			const ObjectTime o (utc_time);
			return this->getZoneTime (o);
		}

		inline date::sys_time<std::chrono::milliseconds> getZoneTime (const ObjectTime& utc_time) const noexcept {
			std::chrono::milliseconds utc_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(utc_time.getTime ());
			return this->getZoneTime (utc_time_ms);
		}

		inline date::sys_time<std::chrono::milliseconds> getZoneTime (const std::chrono::milliseconds& utc_tp) const noexcept {
			date::local_time<std::chrono::milliseconds> tp{utc_tp};
			return this->getZoneTime (tp);
		}

		inline int getUtcOffset (const ObjectTime::internalTimeType& utc_time) const noexcept {
			const ObjectTime o (utc_time);
			return this->getUtcOffset (o);
		}

		inline int getUtcOffset (const ObjectTime& utc_time) const noexcept {
			std::chrono::milliseconds utc_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(utc_time.getTime ());
			return this->getUtcOffset (utc_time_ms);
		}

		inline int getUtcOffset (const std::chrono::milliseconds& utc_time) const noexcept {
			date::local_time<std::chrono::milliseconds> utc_tp{utc_time};
			return this->getUtcOffset (utc_tp);
		}

		inline const date::sys_info getZoneInfo (const ObjectTime::internalTimeType& utc_time) const noexcept {
			const ObjectTime o (utc_time);
			return this->getZoneInfo (o);
		}

		inline const date::sys_info getZoneInfo (const ObjectTime& utc_time) const noexcept {
			std::chrono::milliseconds utc_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(utc_time.getTime ());
			return this->getZoneInfo (utc_time_ms);
		}

		inline const date::sys_info getZoneInfo (const std::chrono::milliseconds& utc_time) const noexcept {
			date::local_time<std::chrono::milliseconds> utc_tp{utc_time};
			return this->getZoneInfo (utc_tp);
		}

		inline const date::time_zone* getZonePtr (void) const noexcept {
			return this->_zone;
		}

		[[nodiscard]] static std::shared_ptr<ZoneDate> create (const std::string& zone_name) {
			return std::shared_ptr<ZoneDate>(new ZoneDate (zone_name));
		}

		[[nodiscard]] static std::shared_ptr<ZoneDate> create (const double& lat, const double& lon, const std::string& zone_name) {
			return std::shared_ptr<ZoneDate>(new ZoneDate (lat, lon, zone_name));
		}

		[[nodiscard]] static std::shared_ptr<ZoneDate> create (const double& lat, const double& lon, const ZoneDetectResult *zone) {
			return std::shared_ptr<ZoneDate>(new ZoneDate (lat, lon, zone));
		}

	private:
		date::sys_time<std::chrono::milliseconds> getZoneTime (const date::local_time<std::chrono::milliseconds>& utc_tp) const noexcept;
		const date::sys_info getZoneInfo (const date::local_time<std::chrono::milliseconds>& utc_tp) const noexcept;
		int getUtcOffset (const date::local_time<std::chrono::milliseconds>&) const noexcept;
	};
}

#endif /* _X_GPSDATA_ZONEDATE_ */