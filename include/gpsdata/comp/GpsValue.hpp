#ifndef _GPSDATA_COMP_GPSVALUE_
#define _GPSDATA_COMP_GPSVALUE_

#include <Logger.hpp>

#include <gpsdata/GpsValue.hpp>

namespace gpsdata {
	template<typename T>
	bool operator== (const GpsValue<T>& a, const GpsValue<T>& b) {
		DEBUG_MSG ("{:s}: compare GpsValue\n", __func__);
		if (a.type != b.type) return false;
		if (a.value._raw != b.value._raw) return false;
		return true;
	}
}

#endif /* _GPSDATA_COMP_GPSVALUE_ */
