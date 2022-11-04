#ifndef _GPSDATA_TRAIT_ISINSTANCE_
#define _GPSDATA_TRAIT_ISINSTANCE_

#include <type_traits>

namespace gpsdata {
	namespace
	{
		template <typename, template <typename...> typename>
		struct is_instance_impl : public std::false_type {};

		template <template <typename...> typename U, typename...Ts>
		struct is_instance_impl<U<Ts...>, U> : public std::true_type {};
	}

	template <typename T, template <typename ...> typename U>
	using is_instance = is_instance_impl<std::decay_t<T>, U>;
}

#endif /* _GPSDATA_TRAIT_ISINSTANCE_ */