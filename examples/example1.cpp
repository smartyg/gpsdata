#include <features.h>

#include <memory>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <tuple>

// Include this file before gpsdata.h to enable debug messages, if _RELEASE is defined also DEBUG_MSG is enabled.
#include <gpsdata-utils.hpp>
#include <gpsdata.hpp>

using GpsFactoryType = gpsdata::utils::GpsDataFactoryBasic;
using GpsPointType = gpsdata::GpsPoint<GpsFactoryType>;
using GpsSegmentType = gpsdata::GpsSegment<GpsFactoryType, GpsPointType>;
using GpsRouteType = gpsdata::GpsRoute<GpsFactoryType, GpsSegmentType>;

int main(void) {
	gpsdata::ObjectId id = 1;
	auto factory = GpsFactoryType::create ();
	auto route = GpsRouteType::create<GpsRouteType> (id, factory);

	route->addSegment (0);
	gpsdata::ObjectTime t1 = static_cast<gpsdata::ObjectTime::internalTimeType>(10);
	if (route->addPointData (0, t1, "SPEED", 10, true)) std::cout << "success" << std::endl;
	else std::cout << "failed" << std::endl;
	gpsdata::ObjectTime t2 = static_cast<gpsdata::ObjectTime::internalTimeType>(20);
	if (route->addPointData (0, t2, "SPEED", 20, true)) std::cout << "success" << std::endl;
	else std::cout << "failed" << std::endl;

	return 0;
}