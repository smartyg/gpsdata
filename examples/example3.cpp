#include <features.h>

#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include <bitsery/bitsery.h>
#include <bitsery/adapter/buffer.h>
#include <bitsery/traits/vector.h>

// Include this file before gpsdata.h to enable debug messages, if _RELEASE is defined also DEBUG_MSG is enabled.
#include <gpsdata/utils/Logger.hpp>
#include <gpsdata/utils/GpsDataFactoryBasic.hpp>
#include <gpsdata.hpp>
#include <gpsdata/bitsery/bitsery.hpp>
#include <gpsdata/comparision.hpp>

#include "PrintGpsRoute.hpp"

namespace custom {
	struct latlon {
		float lat;
		float lon;
		void *user_data;
	};

	struct GSList {
		void *data;
		GSList *next;
	};

	template<gpsdata::GpsFactoryTrait F>
	class GpsPointExt : virtual public gpsdata::GpsPoint<F> {
	public:
		GpsPointExt (const gpsdata::ObjectTime& time, const std::shared_ptr<const F>& factory) : gpsdata::internal::GpsFactoryUserBase<F> (factory), gpsdata::GpsPoint<F> (time, factory) { }
		GpsPointExt (const std::shared_ptr<const F>& factory) : gpsdata::internal::GpsFactoryUserBase<F> (factory), gpsdata::GpsPoint<F> (factory) { }

	protected:
		GSList _list_entry;
		latlon _latlon;

	public:
		GSList *initExt (GSList *ptr) {
			auto t_lat = this->getFactory ()->getDataType ("LAT");
			auto t_lon = this->getFactory ()->getDataType ("LON");
			if (!(this->hasDataType (t_lat) && this->hasDataType (t_lon))) return ptr;

			this->_list_entry.next = ptr;
			this->_list_entry.data = &this->_latlon;
			const auto lat_value = this->getData (t_lat);
			const auto lon_value = this->getData (t_lon);
			float lat = NAN;
			this->getFactory ()->getValue (lat_value, lat, true);
			this->_latlon.lat = lat / 100000000;
			float lon = NAN;
			this->getFactory ()->getValue (lon_value, lon, true);
			this->_latlon.lon = lon / 100000000;
			this->_latlon.user_data = &*this;
			return &this->_list_entry;
		}
	};

	template<gpsdata::GpsFactoryTrait F>
	class GpsSegmentExt : virtual public gpsdata::GpsSegment<F, GpsPointExt<F>> {
	public:
		GpsSegmentExt (const std::shared_ptr<const F>& factory) : gpsdata::internal::GpsFactoryUserBase<F> (factory), gpsdata::GpsStatistics<F> (factory), gpsdata::GpsSegment<F, GpsPointExt<F>> (factory) { }
		GpsSegmentExt (const int& n, const std::shared_ptr<const F>& factory) : gpsdata::internal::GpsFactoryUserBase<F> (factory), gpsdata::GpsStatistics<F> (factory), gpsdata::GpsSegment<F, GpsPointExt<F>> (n, factory) { }

	protected:
		GSList *_first;

		const GSList* getGSList (void) {
			return this->_first;
		}

	public:
		GSList *initExt (GSList *ptr) {
			auto it = this->end ();
			--it;
			while (it != this->begin ()) {
				ptr = (*it)->initExt (ptr);
				--it;
			}
			if (it == this->begin ()) ptr = (*it)->initExt (ptr);
			this->_first = ptr;
			return ptr;
		}
	};

	template<gpsdata::GpsFactoryTrait F>
	class GpsRouteExt : virtual public gpsdata::GpsRoute<F, GpsSegmentExt<F>> {
	public:
		GpsRouteExt (const gpsdata::ObjectId& id, const std::shared_ptr<const F>& factory) : gpsdata::internal::GpsFactoryUserBase<F> (factory), gpsdata::GpsStatistics<F> (factory), gpsdata::GpsRoute<F, GpsSegmentExt<F>> (id, factory) { }

	protected:
		GSList *_first;

	public:
		const GSList* getGSList (void) {
			return this->_first;
		}

		void initExt (void) {
			GSList *ptr = nullptr;
			auto it = this->end ();
			--it;
			while (it != this->begin ()) {
				ptr = (*it)->initExt (ptr);
				--it;
			}
			if (it == this->begin ()) ptr = (*it)->initExt (ptr);
			this->_first = ptr;
		}
	};
}

using Buffer = std::vector<uint8_t>;
using OutputAdapter = bitsery::OutputBufferAdapter<Buffer>;
using InputAdapter = bitsery::InputBufferAdapter<Buffer>;

using GpsFactoryType = gpsdata::utils::GpsDataFactoryBasic;
using GpsPointType = custom::GpsPointExt<GpsFactoryType>;
using GpsSegmentType = custom::GpsSegmentExt<GpsFactoryType>;
using GpsRouteType = custom::GpsRouteExt<GpsFactoryType>;

void printGSList (const custom::GSList *);

void printGSList (const custom::GSList *ptr) {
	if (ptr == nullptr) return;
	std::cout << "--- point ---" << std::endl;
	custom::latlon *latlon = static_cast<custom::latlon*>(ptr->data);
	std::cout << "  latitude: " << std::to_string (latlon->lat) << std::endl;
	std::cout << "  longitude: " << std::to_string (latlon->lon) << std::endl;

	if (ptr->next != nullptr) printGSList (ptr->next);
}

int main (void) {
	gpsdata::ObjectId id = 1;
	auto factory = GpsFactoryType::create ();
	auto route = GpsRouteType::create<GpsRouteType> (id, factory);
	route->addSegment (0);

	gpsdata::ObjectTime t1 (static_cast<gpsdata::ObjectTime::internalTimeType>(10));
	route->addPointData (0, t1, "SPEED", 10, true);
	route->addPointData (0, t1, "ALT", 1000, true);
	route->addPointData (0, t1, "CADANCE", 98, true);
	//Amsterdam Central Station, the Netherlands
	route->addPointData (0, t1, "LAT", 490031600, true);
	route->addPointData (0, t1, "LON", 5237898800, true);

	gpsdata::ObjectTime t2 (static_cast<gpsdata::ObjectTime::internalTimeType>(20));
	route->addPointData (0, t2, "SPEED", 20, true);
	route->addPointData (0, t2, "CADANCE", 90, true);
	// Amsterdam Dam Square, the Netherlands
	route->addPointData (0, t2, "LAT", 489306200, true);
	route->addPointData (0, t2, "LON", 5237267200, true);

	gpsdata::ObjectTime t3 (static_cast<gpsdata::ObjectTime::internalTimeType>(30));
	route->addPointData (0, t3, "CADANCE", 101, true);
	// Amsterdam Dam Palace, the Netherlands
	// 52.373178, 4.891856
	route->addPointData (0, t3, "LAT", 489185600, true);
	route->addPointData (0, t3, "LON", 5237317800, true);

	gpsdata::ObjectTime t4 (static_cast<gpsdata::ObjectTime::internalTimeType>(40));
	// Anne Frank House, Amsterdam, the Netherlands
	// 52.375226, 4.883885
	route->addPointData (0, t4, "LAT", 488388500, true);
	route->addPointData (0, t4, "LON", 5237522600, true);

	gpsdata::ObjectTime t5 (static_cast<gpsdata::ObjectTime::internalTimeType>(50));
	// Vondel Park entrance, Amsterdam, the Netherlands
	// 52.361735, 4.881794
	route->addPointData (0, t5, "LAT", 488179400, true);
	route->addPointData (0, t5, "LON", 5236173500, true);

	gpsdata::ObjectTime t6 (static_cast<gpsdata::ObjectTime::internalTimeType>(60));
	// Van Gogh Museum, Amsterdam, the Netherlands
	// 52.358520, 4.880948
	route->addPointData (0, t6, "LAT", 488094800, true);
	route->addPointData (0, t6, "LON", 5235852000, true);

	gpsdata::ObjectTime t7 (static_cast<gpsdata::ObjectTime::internalTimeType>(80));
	// Rijksmuseum, Amsterdam, the Netherlands
	// 52.360272, 4.885635
	route->addPointData (0, t7, "LAT", 488563500, true);
	route->addPointData (0, t7, "LON", 5236027200, true);

	gpsdata::ObjectTime t8 (static_cast<gpsdata::ObjectTime::internalTimeType>(90));
	// Rembrandt House Museum, Amsterdam, the Netherlands
	// 52.369423, 4.901314
	route->addPointData (0, t8, "LAT", 490131400, true);
	route->addPointData (0, t8, "LON", 5236942300, true);

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
		std::cout << std::to_string (static_cast<unsigned>(c)) << ",";
	}
	std::cout << std::endl;

	//same as serialization, but returns deserialization state as a pair
	//first = error code, second = is buffer was successfully read from begin to the end.
	auto state = bitsery::quickDeserialization<InputAdapter>({buffer.begin(), writtenSize}, res);

	assert(state.first == bitsery::ReaderError::NoError && state.second);

	printGpsRoute (route);

	std::cout << std::endl;

	printGpsRoute (res);

	assert(*route == *res);

	route.reset ();

	res->initExt ();
	printGSList (res->getGSList ());

	return 0;
}