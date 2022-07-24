#include <features.h>

#include <memory>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <tuple>

//#define DEBUG(format, ...) (empty_func())
//#define DEBUG(format, ...) (printf(format, __VA_ARGS__))
//inline const void empty_func (void) { return; };

#include "gpsdata/utils/Logger.hpp"

#include "gpsdata.hpp"
#include "gpsdata/utils/GpsDataFactoryBasic.hpp"

int main(void) {
	gpsdata::ObjectId id = 1;
	auto factory = gpsdata::utils::GpsDataFactoryBasic::create ();
	auto route = gpsdata::GpsRoute<gpsdata::utils::GpsDataFactoryBasic>::create (id, factory);
	route->addSegment (0);
	gpsdata::ObjectTime t1 = static_cast<uint64_t>(10);
	if (route->addPointData (0, t1, "SPEED", 10, true)) std::cout << "success" << std::endl;
	else std::cout << "failed" << std::endl;
	gpsdata::ObjectTime t2 = static_cast<uint64_t>(20);
	if (route->addPointData (0, t2, "SPEED", 20, true)) std::cout << "success" << std::endl;
	else std::cout << "failed" << std::endl;

	return 0;
}