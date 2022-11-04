#ifndef _GPSDATA_UTILS_POINTDATE_
#define _GPSDATA_UTILS_POINTDATE_

namespace gpsdata::utils { class PointDate; }

#include <date/date.h>
#include <zonedetect.h>
#include <chrono>
#include <sstream>
#include <memory>

#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/traits/GpsSegment.hpp>
#include <gpsdata/traits/GpsRoute.hpp>
#include <gpsdata/utils/ZoneDate.hpp>
#include <gpsdata/types/ObjectTime.hpp>

namespace gpsdata::utils {
	class PointDate {
		PointDate (void);
		~PointDate (void);

		PointDate (const PointDate&)                = delete; // copy constructor
		PointDate (PointDate&&) noexcept            = delete; // move constructor
		PointDate& operator= (const PointDate&)     = delete; // copy assignment
		PointDate& operator= (PointDate&&) noexcept = delete; // move assignment

	public:
		static const std::shared_ptr<ZoneDate> getTimeZone (const double& lat, const double& lon) noexcept;
		static const std::shared_ptr<ZoneDate> getTimeZone (const std::string&) noexcept;
		static const ObjectTime parseTime (const std::string&, const std::vector<std::string>&);

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
			}
			return {};
		}

		template<GpsRouteTrait Route>
		inline static const date::zoned_time<ObjectTime::timeType> getTimeInZone (const std::shared_ptr<Route> route) {
			if (route) {
				const std::shared_ptr<typename Route::Segment>& segment = *(route->cbegin ());
				if (segment) {
					const std::shared_ptr<typename Route::Point>& point = *(segment->cbegin ());
					if (point) {
						return PointDate::getZonedTime (point);
					}
				}
			}
			return {};
		}

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
		ZoneDetect *_zone_database;

		static const ZoneDetect* getZoneDatabasePtr (void);
		static const date::local_time<std::chrono::milliseconds> parseTimeInt (const std::string&, const std::vector<std::string>&);
		static const date::zoned_time<ObjectTime::timeType> makeZonedTime (const double& lat, const double& lon, const ObjectTime& time);

		inline static time_t convertToTimeT (const date::local_time<std::chrono::milliseconds>& tp) noexcept {
			std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
			return static_cast<time_t>(s.count());
		}
	};
}

#endif /* _GPSDATA_UTILS_POINTDATE_ */