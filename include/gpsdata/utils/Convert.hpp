#ifndef _GPSDATA_UTILS_CONVERT_
#define _GPSDATA_UTILS_CONVERT_

#include <cstdint>
#include <limits>
#include <sstream>

namespace gpsdata::utils {

	class Convert final {
		Convert (void) = delete;
		~Convert (void) = delete;

	public:
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wunused-parameter"
		#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
		template<class T, class U>
		constexpr static inline bool convertValue (T& result, const U& value, bool best_effort = false) noexcept {
			static_assert (!std::is_const_v<T>);
			static_assert (!std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::string_view>);
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wunused-variable"
			T v{};
			bool match = false;
			#pragma GCC diagnostic pop

			if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::remove_cv_t<std::remove_reference_t<U>>>) {
				v = value;
				match = true;
			} else if constexpr (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>) {
				if constexpr (Convert::is_in_range<T, U>()) {
					v = static_cast<T>(value);
					match = true;
				} else if (best_effort) {
					if (Convert::is_in_range<T> (value)) {
						v = static_cast<T>(value);
						match = true;
					}
				} else return false;
			} else if constexpr (std::is_arithmetic_v<U> && std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, char *>) {
				if constexpr (std::numeric_limits<U>::is_signed)
					v = const_cast<T>(std::to_string(static_cast<int64_t>(value)).c_str ());
				else
					v = const_cast<T>(std::to_string(static_cast<uint64_t>(value)).c_str ());
				match = true;
			} else if constexpr (std::is_arithmetic_v<U> && std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::string>) {
				if constexpr (std::numeric_limits<U>::is_integer && std::numeric_limits<U>::is_signed)
					v = std::to_string(static_cast<int64_t>(value));//.c_str ();
				else if constexpr (std::numeric_limits<U>::is_integer)
					v = std::to_string(static_cast<uint64_t>(value));//.c_str ();
				else
					v = std::to_string (value);
				match = true;
			} else if constexpr (std::is_arithmetic_v<U> && std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::istream>) {
				if constexpr (std::numeric_limits<U>::is_signed)
					v << std::to_string(static_cast<int64_t>(value)).c_str ();
				else
					v << std::to_string(static_cast<uint64_t>(value)).c_str ();
				match = true;
			} else if constexpr (std::is_arithmetic_v<T> &&
				(std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, std::string> ||
					std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, std::string_view> ||
					std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, std::istream *>)) {
				if (best_effort) {
					if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, std::istream *>) {
						*value >> v;
						if (!value->fail () && !value->bad ()) match = true;
					} else if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, std::string>) {
						std::stringstream s(value);
						if constexpr (!std::is_same_v<T, bool> && std::numeric_limits<T>::is_integer && (sizeof (v) <= 1 || !std::is_signed_v<T>)) {
								int64_t v1;
								s >> v1;
								if (!s.fail () && !s.bad () && s.eof ())
									match = Convert::convertValue (v, v1, true);
						} else {
							s >> v;
							if (!s.fail () && !s.bad () && s.eof ()) match = true;
						}
					}
				} else
					return false;
			} else
				static_assert (true);

			if (match) {
				result = std::move (v);
				return true;
			} else {
				return false;
			}
		}
		#pragma GCC diagnostic pop

		template<class TO, class FROM>
		constexpr static inline bool is_in_range (void) {
			static_assert (std::is_arithmetic_v<FROM>);
			static_assert (std::is_arithmetic_v<TO>);

			if constexpr (std::is_same_v<TO, FROM>) return true;

			constexpr const bool from_signed = std::numeric_limits<FROM>::is_signed;
			constexpr const bool to_signed = std::numeric_limits<TO>::is_signed;
			constexpr const FROM from_min = std::numeric_limits<FROM>::lowest ();
			constexpr const FROM from_max = std::numeric_limits<FROM>::max ();
			constexpr const TO to_min = std::numeric_limits<TO>::lowest ();
			constexpr const TO to_max = std::numeric_limits<TO>::max ();

			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wbool-compare"
			if constexpr (from_signed && to_signed) {
				if constexpr (from_min >= to_min && from_max <= to_max) return true;
				else return false;
			} else if constexpr (from_signed) {
				return false;
			} else if constexpr (from_max <= to_max) {
				return true;
			} else return false;
			#pragma GCC diagnostic pop
		}

		template<class TO, class FROM>
		constexpr static inline bool is_in_range (const FROM& value) {
			static_assert (std::is_arithmetic_v<FROM>);
			static_assert (std::is_arithmetic_v<TO>);

			if constexpr (Convert::is_in_range<TO, FROM>()) {
				return true;
			} else if constexpr (std::is_same_v<TO, bool>) {
				return (value == 0 || value == 1);
			}

			constexpr const bool to_signed = std::is_signed_v<TO>;
			constexpr const bool from_signed = std::is_signed_v<FROM>;

			if constexpr (to_signed == from_signed) { // if both are either signed or unsigned
				return (std::numeric_limits<TO>::lowest () <= value && std::numeric_limits<TO>::max () >= value);
			} else if constexpr (from_signed) { // FROM is signed and TO is unsigned type
				if (value < 0) return false;
				else {// `value` is not negative
					#pragma GCC diagnostic push
					#pragma GCC diagnostic ignored "-Wsign-compare"
					return (std::numeric_limits<TO>::max () >= value);
					#pragma GCC diagnostic pop
				}
			} else { // FROM is unsigned and TO is signed
				// if FROM is a bool, we assume it's always convertable
				if constexpr (std::is_same_v<FROM, bool>) return true;
				// As FROM is unsigned, `value` is not negative
				else return (std::numeric_limits<TO>::max () >= value);
			}
		}
	};
}

#endif /* _GPSDATA_UTILS_CONVERT_ */
