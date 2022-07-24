#ifndef _X_GPSDATA_POINTDATE_
#define _X_GPSDATA_POINTDATE_

namespace gpsdata::utils { class PointDate; }

#include <date/date.h>
#include <zonedetect.h>
#include <chrono>
#include <sstream>

#include <gpsdata/utils/ZoneDate.hpp>
#include <gpsdata/types/ObjectTime.hpp>

namespace gpsdata::utils {
	class PointDate {
	public:
		static bool init (const std::string&);
		static void destroy (void) noexcept;

		static bool isValid (void) noexcept;

		static const ZoneDate *getTimeZone (float lat, float lon) noexcept;
		static const ZoneDate *getTimeZone (const std::string&) noexcept;

		static const ObjectTime parseTime (const std::string&, const std::vector<std::string>&);
		static time_t convertToTimeT (const ObjectTime&) noexcept;
		static time_t convertToTimeT (std::chrono::milliseconds) noexcept;

	private:
		static const ZoneDetect *_zone_database;
		static std::string _db_path;

		static inline time_t convertToTimeT (date::local_time<std::chrono::milliseconds>) noexcept;
		static const date::local_time<std::chrono::milliseconds> parseTimeInt (const std::string&, const std::vector<std::string>&);
	};
}

#endif /* _X_GPSDATA_POINTDATE_ */