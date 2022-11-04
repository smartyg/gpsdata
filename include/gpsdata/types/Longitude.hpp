#ifndef _GPSDATA_TYPE_LONGITUDE_
#define _GPSDATA_TYPE_LONGITUDE_

#include <cstdint>
#include <cfloat>
#include <float.h>
#include <string>

#ifndef POWER_10_7
#define POWER_10_7 (10000000)
#endif

namespace gpsdata {
	inline namespace types {

		class Longitude final {
		private:
			using internal_type = int32_t;
			internal_type _longitude;

			Longitude (void) noexcept : _longitude (INT32_MIN) { }

			explicit Longitude (const internal_type& longitude) noexcept : _longitude(longitude) { }

		public:
			explicit Longitude (const float& longitude) noexcept {
				this->_longitude = static_cast<internal_type>(longitude * POWER_10_7);
			}

			explicit Longitude (const double& longitude) noexcept {
				this->_longitude = static_cast<internal_type>(longitude * POWER_10_7);
			}

			explicit Longitude (const long double& longitude) noexcept {
				this->_longitude = static_cast<internal_type>(longitude * POWER_10_7);
			}

			Longitude (const Longitude& other) : _longitude(other._longitude) {} // copy constructor

			Longitude (Longitude&& other) : _longitude(other._longitude) {} // move constructor

			Longitude& operator= (const Longitude& other) { // copy assignment
				this->_longitude = other._longitude;
				return *this;
			}

			Longitude& operator= (Longitude&& other) { // move assignment
				this->_longitude = other._longitude;
				return *this;
			}

			Longitude& operator= (const float& longitude) noexcept {
				this->_longitude = static_cast<internal_type>(longitude * POWER_10_7);
				return *this;
			}

			Longitude& operator= (const double& longitude) noexcept {
				this->_longitude = static_cast<internal_type>(longitude * POWER_10_7);
				return *this;
			}

			Longitude& operator= (const long double& longitude) noexcept {
				this->_longitude = static_cast<internal_type>(longitude * POWER_10_7);
				return *this;
			}

			//inline float       getLongitude (void) const noexcept { return static_cast<float>(this->_longitude) / POWER_10_7; }
			//inline double      getLongitude (void) const noexcept { return static_cast<double>(this->_longitude) / POWER_10_7; }
			inline long double getLongitude (void) const noexcept { return static_cast<long double>(this->_longitude) / POWER_10_7; }

			void* operator new (size_t) = delete;
			void operator delete (void*) = delete;

			inline bool operator== (const Longitude& rhs)   const noexcept { return (this->_longitude == rhs._longitude); }
			inline bool operator== (const long double& rhs) const noexcept { return (this->_longitude == rhs); }
			inline bool operator!= (const Longitude& rhs)   const noexcept { return (this->_longitude != rhs._longitude); }
			inline bool operator!= (const long double& rhs) const noexcept { return (this->_longitude != rhs); }

			inline bool operator< (const Longitude& rhs)  const noexcept { return (this->_longitude < rhs._longitude); }
			inline bool operator> (const Longitude& rhs)  const noexcept { return (this->_longitude > rhs._longitude); }
			inline bool operator<= (const Longitude& rhs) const noexcept { return (this->_longitude <= rhs._longitude); }
			inline bool operator>= (const Longitude& rhs) const noexcept { return (this->_longitude >= rhs._longitude); }

			inline Longitude& operator+= (const Longitude&) noexcept = delete;
			inline Longitude& operator-= (const Longitude&) noexcept = delete;

			inline Longitude& operator++ (void) noexcept = delete;
			inline Longitude  operator++ (int)  noexcept = delete;
			inline Longitude& operator-- (void) noexcept = delete;
			inline Longitude  operator-- (int)  noexcept = delete;

			inline Longitude operator+ (const Longitude& rhs)   const noexcept { return Longitude (Longitude::check<internal_type>(this->_longitude + rhs._longitude)); }
			inline Longitude operator+ (const long double& rhs) const noexcept { return Longitude (static_cast<long double>(this->_longitude) / POWER_10_7 + rhs); }
			inline Longitude operator- (const Longitude& rhs)   const noexcept { return Longitude (Longitude::check<internal_type>(this->_longitude - rhs._longitude)); }
			inline Longitude operator- (const long double& rhs) const noexcept { return Longitude (static_cast<long double>(this->_longitude) / POWER_10_7 - rhs); }

			inline operator bool() const noexcept { return this->_longitude >= -1800000000 && this->_longitude <= 1800000000; }

			inline operator float() const noexcept { return static_cast<float>(this->_longitude) / POWER_10_7; }
			/* TODO: fix detection if double and long double are bigger. Code below does not work as DBL_MAX evaluates to a floating point that cpp can not handle.
			#if DBL_MAX > FLT_MAX
			inline operator double() const noexcept { return static_cast<double>(this->_longitude) / POWER_10_7; }
			#endif
			#if LDBL_MAX > DBL_MAX
			inline operator long double() const noexcept { return static_cast<long double>(this->_longitude) / POWER_10_7; }
			#endif
			*/

			const std::string getString (void) const {
				std::string str;
				if (this->_longitude >= 0) {
					str = std::to_string (static_cast<float>(this->_longitude) / POWER_10_7) + "° E";
				} else {
					str = std::to_string (static_cast<float>(std::abs (this->_longitude)) / POWER_10_7) + "° W";
				}
				return str;
			}

		private:
			template <typename S>
			void serialize (S& s) {
				s.value4b (this->_longitude);
			}

			template<class T>
			static float check (const T& longitude) noexcept {
				if (longitude == 0.0) return 0.0;
				return longitude % 180;
			}
		};
	}
}

#endif /* _GPSDATA_TYPE_LONGITUDE_ */
