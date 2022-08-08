#ifndef _X_GPSDATA_BITSERY_GPSPOINT_
#define _X_GPSDATA_BITSERY_GPSPOINT_

#include <type_traits>
#include <memory>

#include <bitsery/bitsery.h>

#include <gpsdata/traits/is_instance.hpp>
#include <gpsdata/traits/GpsPoint.hpp>
#include <gpsdata/GpsPoint.hpp>

#include <gpsdata/bitsery/ObjectTime.hpp>
#include <gpsdata/bitsery/GpsValue.hpp>

namespace gpsdata {
	template<typename B, GpsPointTrait P>
	void serialize (B& s, std::shared_ptr<P>& point) requires(std::is_base_of<GpsPoint<typename P::GpsFactory>, P>::value) {

		if constexpr (is_instance<B, bitsery::Serializer>{}) {
			s.object (point->_time);
			serializeGpsValueVector (s, point->_data, point->getFactory ());
		}

		if constexpr (is_instance<B, bitsery::Deserializer>{}) {
			s.object (point->_time);
			deserializeGpsValueVector (s, point->_data, point->getFactory ());
		}
	};
}

#endif /* _X_GPSDATA_BITSERY_GPSPOINT_ */
