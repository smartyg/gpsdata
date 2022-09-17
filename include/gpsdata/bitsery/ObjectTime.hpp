#ifndef _X_GPSDATA_BITSERY_OBJECTTIME_
#define _X_GPSDATA_BITSERY_OBJECTTIME_

#include <bitsery/bitsery.h>

#include <gpsdata/types/ObjectTime.hpp>

namespace gpsdata {
	inline namespace types {
		template<typename B>
		void serialize (B& s, ObjectTime& o) {
			if constexpr (is_instance<B, bitsery::Serializer>{}) {
				// First cast to ObjectTime::internalTimeType (which is a 64 bit value).
				ObjectTime::internalTimeType value = o.get ();
				s.value8b (value);
			}

			if constexpr (is_instance<B, bitsery::Deserializer>{}) {
				ObjectTime::internalTimeType value;
				s.value8b (value);
				// Cast the 64 bit value to a time type.
				o = value;
			}
		};

		template<typename B>
		void serialize (B& s, const ObjectTime& o) {
			static_assert (is_instance<B, bitsery::Serializer>{});
			ObjectTime::internalTimeType value = o.getTime ();
			s.value8b (value);
		};
	}
}

#endif /* _X_GPSDATA_BITSERY_OBJECTTIME_ */
