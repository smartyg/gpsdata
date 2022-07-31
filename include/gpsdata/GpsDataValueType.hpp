#ifndef _X_GPSDATA_TYPES_
#define _X_GPSDATA_TYPES_

#include <cstdint>

namespace gpsdata {
	enum GpsDataValueType : uint_fast8_t {
		NONE_T = 0,
		BOOL_T,
		S_CHAR_T,
		U_CHAR_T,
		S_SHORT_T,
		U_SHORT_T,
		S_INT_T,
		U_INT_T,
		S_LONG_T,
		U_LONG_T,
		FLOAT_T,
		DOUBLE_T
	};
}

#endif /* _X_GPSDATA_TYPES_ */
