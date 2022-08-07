#ifndef _X_GPSDATA_TRAITS_GPSROUTE_
#define _X_GPSDATA_TRAITS_GPSROUTE_

#include <gpsdata/traits/common.hpp>
#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/traits/GpsSegment.hpp>

namespace gpsdata {
	// Forward declaration of the GpsRoute type.
	template<GpsFactoryTrait F, GpsSegmentTrait S>
	class GpsRoute;

	inline namespace traits {
		template<class T>
		concept CONCEPT_RETURN GpsRouteTrait = requires(const T t) {
			typename T::GpsFactory;
			typename T::Segment;
			typename T::Point;
			typename T::DataType;
			typename T::ActivityType;
			std::is_base_of<GpsRoute<typename T::GpsFactory, typename T::Segment>, T>::value;
		};
	}
}

#endif /* _X_GPSDATA_TRAITS_GPSROUTE_ */