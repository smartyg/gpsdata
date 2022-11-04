#ifndef _GPSDATA_BITSERY_GPSSTATISTICS_
#define _GPSDATA_BITSERY_GPSSTATISTICS_

#include <type_traits>

#include <bitsery/bitsery.h>

#include <gpsdata/traits/is_instance.hpp>
#include <gpsdata/traits/GpsStatistics.hpp>
#include <gpsdata/GpsStatistics.hpp>

#include <gpsdata/bitsery/GpsValue.hpp>

namespace gpsdata {
	template<typename B, GpsStatisticsTrait S>
	void serialize (B& s, S& statistics) requires(std::is_base_of<GpsStatistics<typename S::GpsFactory>, S>::value) {
		if constexpr (is_instance<B, bitsery::Serializer>{}) {
			serializeGpsValueVector (s, statistics->_statistics, statistics->getFactory ());
		}

		if constexpr (is_instance<B, bitsery::Deserializer>{}) {
			deserializeGpsValueVector (s, statistics->_statistics, statistics->getFactory ());
		}
	};
}

#endif /* _GPSDATA_BITSERY_GPSSTATISTICS_ */
