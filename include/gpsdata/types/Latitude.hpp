#ifndef _X_GPSDATA_LATITUDE_
#define _X_GPSDATA_LATITUDE_

#include <cstdint>
#include <cfloat>
#include <float.h>
#include <string>

#ifndef POWER_10_7
#define POWER_10_7 (10000000)
#endif

namespace gpsdata {
	inline namespace types {

		class Latitude final {
		private:
			using internal_type = int32_t;
			internal_type _latitude;

			Latitude (void) noexcept : _latitude (INT32_MIN) { }

			explicit Latitude (const internal_type& latitude) noexcept : _latitude(latitude) { }

		public:
			explicit Latitude (const float& latitude) noexcept {
				this->_latitude = static_cast<internal_type>(latitude * POWER_10_7);
			}

			explicit Latitude (const double& latitude) noexcept {
				this->_latitude = static_cast<internal_type>(latitude * POWER_10_7);
			}

			explicit Latitude (const long double& latitude) noexcept {
				this->_latitude = static_cast<internal_type>(latitude * POWER_10_7);
			}

			Latitude (const Latitude& other) : _latitude(other._latitude) {} // copy constructor

			Latitude (Latitude&& other) : _latitude(other._latitude) {} // move constructor

			Latitude& operator= (const Latitude& other) { // copy assignment
				this->_latitude = other._latitude;
				return *this;
			}

			Latitude& operator= (Latitude&& other) { // move assignment
				this->_latitude = other._latitude;
				return *this;
			}

			Latitude& operator= (const float& latitude) noexcept {
				this->_latitude = static_cast<internal_type>(latitude * POWER_10_7);
				return *this;
			}

			Latitude& operator= (const double& latitude) noexcept {
				this->_latitude = static_cast<internal_type>(latitude * POWER_10_7);
				return *this;
			}

			Latitude& operator= (const long double& latitude) noexcept {
				this->_latitude = static_cast<internal_type>(latitude * POWER_10_7);
				return *this;
			}

			//inline float       getLatitude (void) const noexcept { return static_cast<float>(this->_latitude) / POWER_10_7; }
			//inline double      getLatitude (void) const noexcept { return static_cast<double>(this->_latitude) / POWER_10_7; }
			inline long double getLatitude (void) const noexcept { return static_cast<long double>(this->_latitude) / POWER_10_7; }

			void* operator new (size_t) = delete;
			void operator delete (void*) = delete;

			inline bool operator== (const Latitude& rhs)    const noexcept { return (this->_latitude == rhs._latitude); }
			inline bool operator== (const long double& rhs) const noexcept { return (this->_latitude == rhs); }
			inline bool operator!= (const Latitude& rhs)    const noexcept { return (this->_latitude != rhs._latitude); }
			inline bool operator!= (const long double& rhs) const noexcept { return (this->_latitude != rhs); }

			inline bool operator< (const Latitude& rhs)  const noexcept { return (this->_latitude < rhs._latitude); }
			inline bool operator> (const Latitude& rhs)  const noexcept { return (this->_latitude > rhs._latitude); }
			inline bool operator<= (const Latitude& rhs) const noexcept { return (this->_latitude <= rhs._latitude); }
			inline bool operator>= (const Latitude& rhs) const noexcept { return (this->_latitude >= rhs._latitude); }

			inline Latitude& operator+= (const Latitude&) noexcept = delete;
			inline Latitude& operator-= (const Latitude&) noexcept = delete;

			inline Latitude& operator++ (void) noexcept = delete;
			inline Latitude  operator++ (int)  noexcept = delete;
			inline Latitude& operator-- (void) noexcept = delete;
			inline Latitude  operator-- (int)  noexcept = delete;

			inline Latitude operator+ (const Latitude& rhs)    const noexcept { return Latitude (Latitude::check<internal_type>(this->_latitude + rhs._latitude)); }
			inline Latitude operator+ (const long double& rhs) const noexcept { return Latitude (static_cast<long double>(this->_latitude) / POWER_10_7 + rhs); }
			inline Latitude operator- (const Latitude& rhs)    const noexcept { return Latitude (Latitude::check<internal_type>(this->_latitude - rhs._latitude)); }
			inline Latitude operator- (const long double& rhs) const noexcept { return Latitude (static_cast<long double>(this->_latitude) / POWER_10_7 - rhs); }

			inline operator bool() const noexcept { return this->_latitude >= -900000000 && this->_latitude <= 900000000; }

			inline operator float() const noexcept { return static_cast<float>(this->_latitude) / POWER_10_7; }
			/* TODO: fix detection if double and long double are bigger. Code below does not work as DBL_MAX evaluates to a floating point that cpp can not handle.
			#if DBL_MAX > FLT_MAX
			inline operator double() const noexcept { return static_cast<double>(this->_latitude) / POWER_10_7; }
			#endif
			#if LDBL_MAX > DBL_MAX
			inline operator long double() const noexcept { return static_cast<long double>(this->_latitude) / POWER_10_7; }
			#endif
			*/

			const std::string getString (void) const {
				std::string str;
				if (this->_latitude >= 0) {
					str = std::to_string (static_cast<float>(this->_latitude) / POWER_10_7) + "° N";
				} else {
					str = std::to_string (static_cast<float>(std::abs (this->_latitude)) / POWER_10_7) + "° S";
				}
				return str;
			}

		private:
			template <typename S>
			void serialize (S& s) {
				s.value4b (this->_latitude);
			}

			template<class T>
			static float check (const T& latitude) noexcept {
				if (latitude == 0.0) return 0.0;
				return latitude % 90;
			}
		};
	}
}

#endif /* _X_GPSDATA_LATITUDE_ */
