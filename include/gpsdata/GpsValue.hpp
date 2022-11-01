#ifndef _GPSDATA_GPSVALUE_
#define _GPSDATA_GPSVALUE_

#include <cstdint>
#include <type_traits>

namespace gpsdata {
	template<typename T>
	struct GpsValue {
		T type;
		union {
			uintptr_t _pointer;
			bool _bool;
			int_fast8_t _s_char;
			uint_fast8_t _u_char;
			int_fast16_t _s_short;
			uint_fast16_t _u_short;
			int_fast32_t _s_int;
			uint_fast32_t _u_int;
			int_fast64_t _s_long;
			uint_fast64_t _u_long;
			float _float;
			double _double;
			uint_fast64_t _raw;
		} value;
	};
}

#endif /* _GPSDATA_GPSVALUE_ */
