#include <features.h>

#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include <bitsery/bitsery.h>
#include <bitsery/adapter/buffer.h>
#include <bitsery/traits/vector.h>

// Include this file before gpsdata.h to enable debug messages, if _RELEASE is defined also DEBUG_MSG is enabled.
//#include <gpsdata-utils.hpp>
#include <gpsdata/utils/Logger.hpp>
#include <gpsdata/utils/GpsDataFactoryBasic.hpp>
#include <gpsdata.hpp>
#include <gpsdata/bitsery/bitsery.hpp>
#include <gpsdata/comparision.hpp>

#include "PrintGpsRoute.hpp"

using Buffer = std::vector<uint8_t>;
using OutputAdapter = bitsery::OutputBufferAdapter<Buffer>;
using InputAdapter = bitsery::InputBufferAdapter<Buffer>;

using GpsFactoryType = gpsdata::utils::GpsDataFactoryBasic;
using GpsPointType = gpsdata::GpsPoint<GpsFactoryType>;
using GpsSegmentType = gpsdata::GpsSegment<GpsFactoryType, GpsPointType>;
using GpsRouteType = gpsdata::GpsRoute<GpsFactoryType, GpsSegmentType>;

int main(void) {
	gpsdata::ObjectId id = 1;
	auto factory = GpsFactoryType::create ();
	auto route = GpsRouteType::create<GpsRouteType> (id, factory);

	route->addSegment (0);

	gpsdata::ObjectTime t1 (static_cast<gpsdata::ObjectTime::internalTimeType>(10));
	route->addPointData (0, t1, "SPEED", 10, true);
	route->addPointData (0, t1, "ALT", 1000, true);
	route->addPointData (0, t1, "CADANCE", 98, true);
	gpsdata::ObjectTime t2 (static_cast<gpsdata::ObjectTime::internalTimeType>(20));
	route->addPointData (0, t2, "SPEED", 20, true);
	route->addPointData (0, t2, "CADANCE", 90, true);
	gpsdata::ObjectTime t3 (static_cast<gpsdata::ObjectTime::internalTimeType>(30));
	route->addPointData (0, t3, "CADANCE", 101, true);
	route->setTitle ("test ride");
	route->setSummary ("long test ride");
	route->setDetails ("A very nice ling test ride.");
	route->setActivityType ("CYCLING");
	route->setTimezoneOffset (3600);

	auto res = GpsRouteType::create<GpsRouteType> (id, factory);

	//create buffer to store data
	Buffer buffer;
	//use quick serialization function,
	//it will use default configuration to setup all the nesessary steps
	//and serialize data to container
	auto writtenSize = bitsery::quickSerialization<bitsery::OutputBufferAdapter<Buffer>>(buffer, route);

	std::cout << "quickSerialization returned: " << std::to_string (writtenSize) << std::endl;
	buffer.resize (writtenSize);
	std::cout << "buffer capacity: " << std::to_string (buffer.capacity ()) << std::endl;
	std::cout << "buffer size: " << std::to_string (buffer.size ()) << std::endl;
	for (const uint8_t& c : buffer) {
		//std::cout << std::to_string(c) << " (" << std::string(static_cast<char>(c)) << "),";
		std::cout << std::to_string (static_cast<unsigned>(c)) << ",";
	}
	std::cout << std::endl;

	//same as serialization, but returns deserialization state as a pair
	//first = error code, second = is buffer was successfully read from begin to the end.
	auto state = bitsery::quickDeserialization<InputAdapter>({buffer.begin(), writtenSize}, res);

	assert(state.first == bitsery::ReaderError::NoError && state.second);

	//printGpsRoute (route);

	//std::cout << std::endl;

	//printGpsRoute (res);

	assert(*route == *res);

	return 0;
}