#ifndef _GPSDATA_UTILS_CONVERT_
#define _GPSDATA_UTILS_CONVERT_

#include <cstdint>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>

template<typename T> std::ctype<T>::~ctype (void) { return; }
template<typename T> bool std::ctype<T>::do_is(mask, T) const { return false; }
template<typename T> const T* std::ctype<T>::do_is(const T*, const T*, mask*) const { return nullptr; }
template<typename T> const T* std::ctype<T>::do_scan_is (mask, const T*, const T*) const { return nullptr; }
template<typename T> const T* std::ctype<T>::do_scan_not (mask, const T*, const T*) const { return nullptr; }
template<typename T> T std::ctype<T>::do_toupper (T) const { return 0; }
template<typename T> const T* std::ctype<T>::do_toupper (T*, const T*) const { return nullptr; }
template<typename T> T std::ctype<T>::do_tolower (T) const { return 0; }
template<typename T> const T* std::ctype<T>::do_tolower (T*, const T*) const { return nullptr; }
template<typename T> char std::ctype<T>::do_narrow (T, char) const { return 0; }
template<typename T> const T* std::ctype<T>::do_narrow (const T*, const T*, char, char*) const { return nullptr; }
template<typename T> T std::ctype<T>::do_widen(char) const { return 0; }
template<typename T> const char* std::ctype<T>::do_widen(const char*, const char*, T*) const { return nullptr; }

template<typename T> std::numpunct<T>::~numpunct (void) { }
template<typename T> void std::numpunct<T>::_M_initialize_numpunct(__locale_struct*) { }

namespace gpsdata::utils {
	namespace internal {
		template<typename>
		struct is_std_string : std::false_type {};

		template<class CharT, class Traits, class Allocator>
		struct is_std_string<std::basic_string<CharT, Traits, Allocator>> : std::true_type {};

		template<typename>
		struct is_std_string_view : std::false_type {};

		template<class CharT, class Traits>
		struct is_std_string_view<std::basic_string_view<CharT, Traits>> : std::true_type {};

		template<typename CharT>
		struct ascii_table : private std::ctype_base {
			typedef CharT char_type;

			struct TableEntry {
				const char_type _character;
				const char_type _upper;
				const char_type _lower;
				const std::ctype_base::mask _mask;
			};

			constexpr static const std::array<TableEntry, 128> table = std::to_array<TableEntry> ({
				{ 0, 0, 0, cntrl },
				{ 1, 0, 0, cntrl },
				{ 2, 0, 0, cntrl },
				{ 3, 0, 0, cntrl },
				{ 4, 0, 0, cntrl },
				{ 5, 0, 0, cntrl },
				{ 6, 0, 0, cntrl },
				{ 7, 0, 0, cntrl },
				{ 8, 0, 0, cntrl },
				{ 9, 0, 0, cntrl | space | blank }, // Tab
				{ 10, 0, 0, cntrl | space },
				{ 11, 0, 0, cntrl | space },
				{ 12, 0, 0, cntrl | space },
				{ 13, 0, 0, cntrl | space },
				{ 14, 0, 0, cntrl },
				{ 15, 0, 0, cntrl },
				{ 16, 0, 0, cntrl },
				{ 17, 0, 0, cntrl },
				{ 18, 0, 0, cntrl },
				{ 19, 0, 0, cntrl },
				{ 20, 0, 0, cntrl },
				{ 21, 0, 0, cntrl },
				{ 22, 0, 0, cntrl },
				{ 23, 0, 0, cntrl },
				{ 24, 0, 0, cntrl },
				{ 25, 0, 0, cntrl },
				{ 26, 0, 0, cntrl },
				{ 27, 0, 0, cntrl },
				{ 28, 0, 0, cntrl },
				{ 29, 0, 0, cntrl },
				{ 30, 0, 0, cntrl },
				{ 31, 0, 0, cntrl },
				{ 32, 0, 0, print | space | blank }, // space
				{ 33, 0, 0, print | punct | graph }, // !
				{ 34, 0, 0, print | punct | graph }, // "
				{ 35, 0, 0, print | punct | graph }, // #
				{ 36, 0, 0, print | punct | graph }, // $
				{ 37, 0, 0, print | punct | graph }, // %
				{ 38, 0, 0, print | punct | graph }, // &
				{ 39, 0, 0, print | punct | graph }, // '
				{ 40, 0, 0, print | punct | graph }, // (
				{ 41, 0, 0, print | punct | graph }, // )
				{ 42, 0, 0, print | punct | graph }, // *
				{ 43, 0, 0, print | punct | graph }, // +
				{ 44, 0, 0, print | punct | graph }, // ,
				{ 45, 0, 0, print | punct | graph }, // ~
				{ 46, 0, 0, print | punct | graph }, // .
				{ 47, 0, 0, print | punct | graph }, // /
				{ 48, 0, 0, print | digit | xdigit | alnum | graph }, // 0
				{ 49, 0, 0, print | digit | xdigit | alnum | graph }, // 1
				{ 50, 0, 0, print | digit | xdigit | alnum | graph }, // 2
				{ 51, 0, 0, print | digit | xdigit | alnum | graph }, // 3
				{ 52, 0, 0, print | digit | xdigit | alnum | graph }, // 4
				{ 53, 0, 0, print | digit | xdigit | alnum | graph }, // 5
				{ 54, 0, 0, print | digit | xdigit | alnum | graph }, // 6
				{ 55, 0, 0, print | digit | xdigit | alnum | graph }, // 7
				{ 56, 0, 0, print | digit | xdigit | alnum | graph }, // 8
				{ 57, 0, 0, print | digit | xdigit | alnum | graph }, // 9
				{ 58, 0, 0, print | punct | graph }, // :
				{ 59, 0, 0, print | punct | graph }, // ;
				{ 60, 0, 0, print | punct | graph }, // <
				{ 61, 0, 0, print | punct | graph }, // =
				{ 62, 0, 0, print | punct | graph }, // >
				{ 63, 0, 0, print | punct | graph }, // ?
				{ 64, 0, 0, print | punct | graph }, // @
				{ 65, 0, 97, print | upper | alpha | alnum | graph | xdigit }, // A
				{ 66, 0, 98, print | upper | alpha | alnum | graph | xdigit }, // B
				{ 67, 0, 99, print | upper | alpha | alnum | graph | xdigit }, // C
				{ 68, 0, 100, print | upper | alpha | alnum | graph | xdigit }, // D
				{ 69, 0, 101, print | upper | alpha | alnum | graph | xdigit }, // E
				{ 70, 0, 102, print | upper | alpha | alnum | graph | xdigit }, // F
				{ 71, 0, 103, print | upper | alpha | alnum | graph }, // G
				{ 72, 0, 104, print | upper | alpha | alnum | graph }, // H
				{ 73, 0, 105, print | upper | alpha | alnum | graph }, // I
				{ 74, 0, 106, print | upper | alpha | alnum | graph }, // J
				{ 75, 0, 107, print | upper | alpha | alnum | graph }, // K
				{ 76, 0, 108, print | upper | alpha | alnum | graph }, // L
				{ 77, 0, 109, print | upper | alpha | alnum | graph }, // M
				{ 78, 0, 110, print | upper | alpha | alnum | graph }, // N
				{ 79, 0, 111, print | upper | alpha | alnum | graph }, // O
				{ 80, 0, 112, print | upper | alpha | alnum | graph }, // P
				{ 81, 0, 113, print | upper | alpha | alnum | graph }, // Q
				{ 82, 0, 114, print | upper | alpha | alnum | graph }, // R
				{ 83, 0, 115, print | upper | alpha | alnum | graph }, // S
				{ 84, 0, 116, print | upper | alpha | alnum | graph }, // T
				{ 85, 0, 117, print | upper | alpha | alnum | graph }, // U
				{ 86, 0, 118, print | upper | alpha | alnum | graph }, // V
				{ 87, 0, 119, print | upper | alpha | alnum | graph }, // W
				{ 88, 0, 120, print | upper | alpha | alnum | graph }, // X
				{ 89, 0, 121, print | upper | alpha | alnum | graph }, // Y
				{ 90, 0, 122, print | upper | alpha | alnum | graph }, // Z
				{ 91, 0, 0, print | punct | graph }, // {
				{ 92, 0, 0, print | punct | graph },
				{ 93, 0, 0, print | punct | graph }, // }
				{ 94, 0, 0, print | punct | graph }, // ^
				{ 95, 0, 0, print | punct | graph }, // _
				{ 96, 0, 0, print | punct | graph }, // `
				{ 97, 65, 0, print | lower | alpha | alnum | graph | xdigit }, // a
				{ 98, 66, 0, print | lower | alpha | alnum | graph | xdigit }, // b
				{ 99, 67, 0, print | lower | alpha | alnum | graph | xdigit }, // c
				{ 100, 68, 0, print | lower | alpha | alnum | graph | xdigit }, // d
				{ 101, 69, 0, print | lower | alpha | alnum | graph | xdigit }, // e
				{ 102, 70, 0, print | lower | alpha | alnum | graph | xdigit }, // f
				{ 103, 71, 0, print | lower | alpha | alnum | graph }, // g
				{ 104, 72, 0, print | lower | alpha | alnum | graph }, // h
				{ 105, 73, 0, print | lower | alpha | alnum | graph }, // i
				{ 106, 74, 0, print | lower | alpha | alnum | graph }, // j
				{ 107, 75, 0, print | lower | alpha | alnum | graph }, // k
				{ 108, 76, 0, print | lower | alpha | alnum | graph }, // l
				{ 109, 77, 0, print | lower | alpha | alnum | graph }, // m
				{ 110, 78, 0, print | lower | alpha | alnum | graph }, // n
				{ 111, 79, 0, print | lower | alpha | alnum | graph }, // o
				{ 112, 80, 0, print | lower | alpha | alnum | graph }, // p
				{ 113, 81, 0, print | lower | alpha | alnum | graph }, // q
				{ 114, 82, 0, print | lower | alpha | alnum | graph }, // r
				{ 115, 83, 0, print | lower | alpha | alnum | graph }, // s
				{ 116, 84, 0, print | lower | alpha | alnum | graph }, // t
				{ 117, 85, 0, print | lower | alpha | alnum | graph }, // u
				{ 118, 86, 0, print | lower | alpha | alnum | graph }, // v
				{ 119, 87, 0, print | lower | alpha | alnum | graph }, // w
				{ 120, 88, 0, print | lower | alpha | alnum | graph }, // x
				{ 121, 89, 0, print | lower | alpha | alnum | graph }, // y
				{ 122, 90, 0, print | lower | alpha | alnum | graph }, // Z
				{ 123, 0, 0, print | punct | graph }, // {
				{ 124, 0, 0, print | punct | graph }, // |
				{ 125, 0, 0, print | punct | graph }, // }
				{ 126, 0, 0, print | punct | graph }, // ~
				{ 127, 0, 0, cntrl }, // DEL
			});
		};

		template<typename CharT>
		struct ascii_ctype : public std::ctype<CharT> {
			typedef CharT char_type;

			ascii_ctype (void) { }
			~ascii_ctype (void) { }

			constexpr bool do_is (std::ctype_base::mask m, char_type c) const override {
				if (c > 127) return false;
				return ((ascii_table<char_type>::table[c]._mask & m) == m);
			}

			const char_type* do_is (const char_type* low, const char_type* high, std::ctype_base::mask* vec) const override {
				char_type* ptr = const_cast<char_type*>(low);
				while (ptr != high) {
					if (*ptr > 127)
						*vec = 0;
					else
						*vec = ascii_table<char_type>::table[*ptr]._mask;
					++ptr;
					++vec;
				}
				return high;
			}

			const char_type* do_scan_is (std::ctype_base::mask m, const char_type* beg, const char_type* end) const override {
				char_type* ptr = const_cast<char_type*>(beg);
				while (ptr != end) {
					if (this->do_is (m, *ptr)) return ptr;
					++ptr;
				}
				return end;
			}

			const char_type* do_scan_not (std::ctype_base::mask m, const char_type* beg, const char_type* end) const override {
				char_type* ptr = const_cast<char_type*>(beg);
				while (ptr != end) {
					if (!this->do_is (m, *ptr)) return ptr;
					++ptr;
				}
				return end;
			}

			constexpr char_type do_toupper (char_type c) const override {
				if (this->do_is (std::ctype_base::lower, c))
					return ascii_table<char_type>::table[c]._upper;
				return c;
			}

			const char_type* do_toupper (char_type* beg, const char_type* end) const override {
				while (beg != end) {
					*beg = this->do_toupper (*beg);
					++beg;
				}
				return end;
			}

			constexpr char_type do_tolower (char_type c) const override {
				if (this->do_is (std::ctype_base::upper, c))
					return ascii_table<char_type>::table[c]._lower;
				return c;
			}

			const char_type* do_tolower (char_type* beg, const char_type* end) const override {
				while (beg != end) {
					*beg = this->do_tolower (*beg);
					++beg;
				}
				return end;
			}

			constexpr char do_narrow (char_type c, char) const override {
				return static_cast<char>(c);
			}

			const char_type* do_narrow (const char_type* beg, const char_type* end, char, char* dst) const override {
				char_type* ptr_in = const_cast<char_type*>(beg);
				while (ptr_in != end) {
					*dst = static_cast<char>(*ptr_in);
					++ptr_in;
					++dst;
				}
				return end;
			}

			constexpr char_type do_widen (char c) const override {
				return static_cast<char_type>(c);
			}

			const char* do_widen (const char* beg, const char* end, char_type* dst) const override {
				char* ptr_in = const_cast<char*>(beg);
				while (ptr_in != end) {
					*dst = static_cast<char_type>(*ptr_in);
					++ptr_in;
					++dst;
				}
				return end;
			}
		};

		template<> struct ascii_ctype<char> : public std::ctype<char> { };

		template<typename CharT>
		struct ascii_numpunct : public std::numpunct<CharT> {
			typedef CharT char_type;

			~ascii_numpunct (void) = default;

			constexpr char_type do_decimal_point (void) const override {
				return ascii_table<char_type>::table[46]._character;
			}

			constexpr char_type do_thousands_sep (void) const override {
				return ascii_table<char_type>::table[44]._character;
			}

			std::string do_grouping (void) const override {
				return {};
			}

			std::basic_string<char_type> do_truename (void) const override {
				return {};
			}

			std::basic_string<char_type> do_falsename (void) const override {
				return {};
			}
		};

		template<typename CharT>
		struct ascii_num_get : public std::num_get<CharT> {
		};

		template<typename CharT>
		std::locale make_locale (void) {
			std::locale l = std::locale::classic ();
			l = std::locale (l, new ascii_ctype<CharT> ());
			l = std::locale (l, new ascii_numpunct<CharT> ());
			l = std::locale (l, new ascii_num_get<CharT> ());
			return l;
		}

	}

	class Convert final {
		Convert (void)                           = delete;
		~Convert (void)                          = delete;
		Convert (const Convert& other)           = delete; // copy constructor
		Convert (Convert&& other) noexcept       = delete; // move constructor
		void operator=(const Convert& other)     = delete; // copy assignment
		void operator=(Convert&& other) noexcept = delete; // move assignment

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
					(internal::is_std_string<std::remove_cv_t<std::remove_reference_t<U>>>::value ||
					internal::is_std_string_view<std::remove_cv_t<std::remove_reference_t<U>>>::value ||
					std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, std::istream *>)) {
				if (best_effort) {
					if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, std::istream *>) {
						*value >> v;
						if (!value->fail () && !value->bad ()) match = true;
					} else if constexpr (internal::is_std_string<std::remove_cv_t<std::remove_reference_t<U>>>::value ||
							internal::is_std_string_view<std::remove_cv_t<std::remove_reference_t<U>>>::value) {
						std::basic_stringstream<typename U::value_type, typename U::traits_type> s;
						s.imbue (internal::make_locale<typename U::value_type>());
						s << value;
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