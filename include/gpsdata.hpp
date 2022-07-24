#ifndef _X_GPSDATA_
#define _X_GPSDATA_

#ifndef DEBUG_MSG
#define DEBUG_MSG(format, ...) (gpsdata::empty_func())
namespace gpsdata {
	inline void empty_func (void) { return; };
}
#endif

#include "gpsdata/types/ObjectId.hpp"
#include "gpsdata/types/ObjectTime.hpp"
#include "gpsdata/types/Latitude.hpp"
#include "gpsdata/types/Longitude.hpp"
#include "gpsdata/GpsValue.hpp"
#include "gpsdata/GpsStatistics.hpp"
#include "gpsdata/GpsPoint.hpp"
#include "gpsdata/GpsSegment.hpp"
#include "gpsdata/GpsRoute.hpp"

#endif /* _X_GPSDATA_ */
