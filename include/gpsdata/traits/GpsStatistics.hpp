#ifndef _GPSDATA_TRAITS_GPSSTATISTICS_
#define _GPSDATA_TRAITS_GPSSTATISTICS_

#include <gpsdata/traits/common.hpp>
#include <gpsdata/traits/GpsFactory.hpp>

namespace gpsdata {
	// Forward declaration of the GpsStatistics type.
	template<GpsFactoryTrait F>
	class GpsStatistics;

	inline namespace traits {
		template<class T>
		concept CONCEPT_RETURN GpsStatisticsTrait = requires(const T t) {
			typename T::GpsFactory;
			typename T::DataType;
			std::is_base_of<GpsStatistics<typename T::GpsFactory>, T>::value;
		};
	}
}

#endif /* _GPSDATA_TRAITS_GPSSTATISTICS_ */