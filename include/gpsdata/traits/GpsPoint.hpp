#ifndef _X_GPSDATA_TRAITS_GPSPOINT_
#define _X_GPSDATA_TRAITS_GPSPOINT_

#include <gpsdata/traits/common.hpp>
#include <gpsdata/traits/GpsFactory.hpp>

namespace gpsdata {
	// Forward declaration of the GpsPoint type.
	template<GpsFactoryTrait F>
	class GpsPoint;

	inline namespace traits {
		template<class T>
		concept CONCEPT_RETURN GpsPointTrait = requires(const T t) {
			typename T::GpsFactory;
			typename T::DataType;
			std::is_base_of<GpsPoint<typename T::GpsFactory>, T>::value;
		};
	}
}

#endif /* _X_GPSDATA_TRAITS_GPSPOINT_ */