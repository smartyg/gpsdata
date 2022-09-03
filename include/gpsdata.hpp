#ifndef _X_GPSDATA_
#define _X_GPSDATA_

#ifndef DEBUG_MSG
#define DEBUG_MSG(format, ...) (gpsdata::utils::empty_func())
#endif
#ifndef CRITICAL_MSG
#define CRITICAL_MSG(format, ...) (gpsdata::utils::empty_func())
#endif
#ifndef ERROR_MSG
#define ERROR_MSG(format, ...) (gpsdata::utils::empty_func())
#endif
#ifndef WARNING_MSG
#define WARNING_MSG(format, ...) (gpsdata::utils::empty_func())
#endif
#ifndef NOTICE_MSG
#define NOTICE_MSG(format, ...) (gpsdata::utils::empty_func())
#endif
#ifndef INFO_MSG
#define INFO_MSG(format, ...) (gpsdata::utils::empty_func())
#endif
#ifndef EMPTY_FUNC_DEFINED
#define EMPTY_FUNC_DEFINED
namespace gpsdata::utils {
	inline const void empty_func (void) { return; };
}
#endif /* EMPTY_FUNC_DEFINED */

#define GPSVALUEVECTOR_MIN_SIZE (static_cast<uint8_t>(12))

#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/traits/GpsPoint.hpp>
#include <gpsdata/traits/GpsStatistics.hpp>
#include <gpsdata/traits/GpsSegment.hpp>
#include <gpsdata/traits/GpsRoute.hpp>

#include <gpsdata/types/ObjectId.hpp>
#include <gpsdata/types/ObjectTime.hpp>
#include <gpsdata/types/Latitude.hpp>
#include <gpsdata/types/Longitude.hpp>

#include <gpsdata/GpsValue.hpp>
#include <gpsdata/GpsStatistics.hpp>
#include <gpsdata/GpsPoint.hpp>
#include <gpsdata/GpsSegment.hpp>
#include <gpsdata/GpsRoute.hpp>

#endif /* _X_GPSDATA_ */
