#ifndef _X_GPSDATA_BITSERY_GPSVALUE_
#define _X_GPSDATA_BITSERY_GPSVALUE_

#include <memory>
#include <vector>
#include <string>
#include <type_traits>

#include <bitsery/bitsery.h>
#include <bitsery/traits/string.h>

#include <gpsdata/traits/is_instance.hpp>
#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/GpsValue.hpp>

namespace gpsdata {
	template <typename B, GpsFactoryTrait F>
	void serializeGpsValueVector (B& s, std::vector<GpsValue<typename F::DataType>>& v, const std::shared_ptr<const F>& factory) {
		static_assert (is_instance<B, bitsery::Serializer>{});
		assert (v.size () < 256);
		uint8_t n_elements = v.size ();
		s.value1b (n_elements);
		auto it = v.cbegin ();
		for (int i = 0; i < n_elements; ++i) {
			const std::string type_string = std::string (factory->getDataTypeString ((*it).type));
			assert (type_string.size () < 64);

			s.text1b (type_string, 64);
			s.value8b ((*it).value._raw);
			++it;
		}
	}

	template <typename B, GpsFactoryTrait F>
	void deserializeGpsValueVector (B& s, std::vector<GpsValue<typename F::DataType>>& v, const std::shared_ptr<const F>& factory) {
		static_assert (is_instance<B, bitsery::Deserializer>{});
		uint8_t n_elements;
		s.value1b (n_elements);
		v.reserve (std::max (n_elements, GPSVALUEVECTOR_MIN_SIZE));

		for (int i = 0; i < n_elements; ++i) {
			std::string type_string;
			uint64_t value;

			s.text1b (type_string, 64);
			s.value8b (value);

			typename F::DataType type = factory->getDataType (type_string);

			v.push_back (GpsValue<typename F::DataType> {type, value});
		}
	}
}

#endif /* _X_GPSDATA_BITSERY_GPSVALUE_ */
