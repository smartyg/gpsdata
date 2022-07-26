#ifndef _GPSDATA_BITSERY_OBJECTID_
#define _GPSDATA_BITSERY_OBJECTID_

#include <bitsery/bitsery.h>

#include <gpsdata/types/ObjectId.hpp>

namespace gpsdata {
	inline namespace types {
		template<typename B>
		void serialize (B& s, ObjectId& o) {
			s.value4b (o._id);
		};
	}
}

#endif /* _GPSDATA_BITSERY_OBJECTID_ */
