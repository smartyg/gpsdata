#ifndef _X_GPSDATA_TRAITS_GPSSEGMENT_
#define _X_GPSDATA_TRAITS_GPSSEGMENT_

#include <gpsdata/traits/common.hpp>
#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/traits/GpsPoint.hpp>

namespace gpsdata {
	// Forward declaration of the GpsSegment type.
	template<GpsFactoryTrait F, GpsPointTrait P>
	class GpsSegment;

	inline namespace traits {
		template<class T>
		concept CONCEPT_RETURN GpsSegmentTrait = requires(const T t) {
			typename T::GpsFactory;
			typename T::Point;
			typename T::DataType;
			std::is_base_of<GpsSegment<typename T::GpsFactory, typename T::Point>, T>::value;
		};
	}
}

#endif /* _X_GPSDATA_TRAITS_GPSSEGMENT_ */