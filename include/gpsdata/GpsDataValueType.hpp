#ifndef _X_GPSDATA_TYPES_
#define _X_GPSDATA_TYPES_

#include <cstdint>

namespace gpsdata {
	enum GpsDataValueType : uint_fast8_t {
		NONE_T = 0,
		BOOL_T, // 1-bit value
		S_CHAR_T, // 8-bits signed integer value
		U_CHAR_T, // 8-bits unsigned integer value
		S_SHORT_T, // 16-bits signed integer value
		U_SHORT_T, // 16-bits unsigned integer value
		S_INT_T, // 32-bits signed integer value
		U_INT_T, // 32-bits unsigned integer value
		S_LONG_T, // 64-bits signed integer value
		U_LONG_T, // 64-bits unsigned integer value
		FLOAT_T, // 32-bits floating number
		DOUBLE_T // 8-bits floating number
	};
}

#endif /* _X_GPSDATA_TYPES_ */
