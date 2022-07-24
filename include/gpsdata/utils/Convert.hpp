#ifndef _X_GPSDATA_CONVERT_
#define _X_GPSDATA_CONVERT_

#include <cstdint>
#include <limits>
#include <sstream>

namespace gpsdata::utils {

	class Convert final {
	public:
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wunused-parameter"
		#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
		template<class T, class U>
		static bool convertValue (T& result, const U& value, bool best_effort = false) noexcept {
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wunused-variable"
			T v{};
			bool match = false;
			#pragma GCC diagnostic pop

			if constexpr (std::is_same_v<T, U>) {
				v = value;
				match = true;
			} else if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>) {
				#pragma GCC diagnostic push
				#pragma GCC diagnostic ignored "-Wunused-variable"
				constexpr bool equal_signed = std::numeric_limits<T>::is_signed && std::numeric_limits<U>::is_signed;
				constexpr bool U_unsigned = !std::numeric_limits<U>::is_signed;
				#pragma GCC diagnostic pop

				#pragma GCC diagnostic push
				#pragma GCC diagnostic ignored "-Wsign-compare"
				#pragma GCC diagnostic ignored "-Wbool-compare"
				if constexpr ((U_unsigned || (equal_signed && std::numeric_limits<T>::min() <= std::numeric_limits<U>::min())) && (std::numeric_limits<T>::max() >= std::numeric_limits<U>::max()))	{
					v = static_cast<T>(value);
					match = true;
				} else if (best_effort) {
					if (std::numeric_limits<T>::min() <= value && std::numeric_limits<T>::max() >= value) {
						v = static_cast<T>(value);
						match = true;
					}
				}
				#pragma GCC diagnostic pop
			} else if constexpr (std::is_arithmetic_v<U> && std::is_same_v<T, char *>) {
				if constexpr (std::numeric_limits<U>::is_signed)
					v = const_cast<T>(std::to_string(static_cast<int64_t>(value)).c_str ());
				else
					v = const_cast<T>(std::to_string(static_cast<uint64_t>(value)).c_str ());
				match = true;
			} else if constexpr (std::is_arithmetic_v<U> && std::is_same_v<T, std::string>) {
				if constexpr (std::numeric_limits<U>::is_signed)
					v = std::to_string(static_cast<int64_t>(value)).c_str ();
				else
					v = std::to_string(static_cast<uint64_t>(value)).c_str ();
				match = true;
			} else if constexpr (std::is_arithmetic_v<U> && std::is_same_v<T, std::istream>) {
				if constexpr (std::numeric_limits<U>::is_signed)
					v << std::to_string(static_cast<int64_t>(value)).c_str ();
				else
					v << std::to_string(static_cast<uint64_t>(value)).c_str ();
				match = true;
			} else if constexpr (std::is_arithmetic_v<T> && (std::is_same_v<U, std::string> || std::is_same_v<U, std::istream *>)) {
				if (best_effort) {
					if constexpr (std::is_same_v<U, std::istream *>) {
						*value >> v;
						if (!value->fail () && !value->bad ()) match = true;
					} else if constexpr (std::is_same_v<U, std::string>) {
						std::stringstream s(value);
						s >> v;
						if (!s.fail () && !s.bad ()) match = true;
					}
				}
			}

			if (match) {
				result = v;
				return true;
			} else {
				return false;
			}
		}
		#pragma GCC diagnostic pop
	};
}

#endif /* _X_GPSDATA_CONVERT_ */
