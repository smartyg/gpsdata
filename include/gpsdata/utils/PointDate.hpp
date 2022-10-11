#ifndef _X_GPSDATA_POINTDATE_
#define _X_GPSDATA_POINTDATE_

namespace gpsdata::utils { class PointDate; }

#include <date/date.h>
#include <zonedetect.h>
#include <chrono>
#include <sstream>
#include <memory>

#include <gpsdata/utils/Logger.hpp>
#include <gpsdata.hpp>
#include <gpsdata/utils/ZoneDate.hpp>
#include <gpsdata/types/ObjectTime.hpp>

namespace gpsdata::utils {
	class PointDate {
	public:
		static bool init (const std::string&);
		static void destroy (void) noexcept;

		static bool isValid (void) noexcept;

		static const std::shared_ptr<ZoneDate> getTimeZone (const double& lat, const double& lon) noexcept;
		static const std::shared_ptr<ZoneDate> getTimeZone (const std::string&) noexcept;

		template<GpsPointTrait Point>
		static const date::zoned_time<ObjectTime::timeType> getZonedTime (const std::shared_ptr<Point> point) {
			std::shared_ptr<const typename Point::GpsFactory> f;
			typename Point::GpsFactory::DataType f_t_lat, f_t_lon;
			double lat, lon;

			if (point) {
				f = point->getFactory ();
				f_t_lat = f->getDataType ("LAT");
				f_t_lon = f->getDataType ("LON");

				const auto lat_value = point->getData (f_t_lat);
				const auto lon_value = point->getData (f_t_lon);

				f->getValue (lat_value, lat, true);
				f->getValue (lon_value, lon, true);

				return PointDate::makeZonedTime (lat / 100000000, lon / 100000000, point->getTime ());
				//const auto zd = PointDate::getTimeZone (lat / 100000000, lon / 100000000);
				//const date::zoned_time ret(zd->getZonePtr (), zd->getZoneTime (point->getTime ()));
				//return ret;
			}
			return {};
		}

		static const ObjectTime parseTime (const std::string&, const std::vector<std::string>&);

		static inline time_t convertToTimeT (const ObjectTime::internalTimeType& t) noexcept  {
			const ObjectTime o (t);
			return PointDate::convertToTimeT(o);
		}

		static inline time_t convertToTimeT (const ObjectTime& t) noexcept {
			const std::chrono::milliseconds tp = std::chrono::duration_cast<std::chrono::milliseconds>(t.getTime ());
			return PointDate::convertToTimeT(tp);
		}

		static inline time_t convertToTimeT (const std::chrono::milliseconds& t) noexcept {
			date::local_time<std::chrono::milliseconds> tp{t};
			return PointDate::convertToTimeT(tp);
		}

	private:
		static const ZoneDetect *_zone_database;
		static std::string _db_path;

		static time_t convertToTimeT (const date::local_time<std::chrono::milliseconds>& tp) noexcept {
			std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
			return static_cast<time_t>(s.count());
		}

		static const date::local_time<std::chrono::milliseconds> parseTimeInt (const std::string&, const std::vector<std::string>&);

		static const date::zoned_time<ObjectTime::timeType> makeZonedTime (const double& lat, const double& lon, const ObjectTime& time);
	};
}

#endif /* _X_GPSDATA_POINTDATE_ */