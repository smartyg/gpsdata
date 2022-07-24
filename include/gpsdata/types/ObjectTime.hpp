#ifndef _X_GPSDATA_OBJECTTIME_
#define _X_GPSDATA_OBJECTTIME_

#include <cstdint>
#include <climits>
#include <chrono>
#include <date/date.h>

namespace gpsdata {
	inline namespace types {
		class ObjectTime final {
		public:
			using internalTimeType = uint64_t;
			using Representation = std::chrono::milliseconds;
			using internalPrecision = Representation::period;
			using timeType = std::chrono::duration<internalTimeType, internalPrecision>;

			timeType _time;

			ObjectTime (void) noexcept { this->_time = timeType::zero (); }

			ObjectTime (internalTimeType time) noexcept {
				this->_time = timeType{time};
			}

			ObjectTime (const timeType& time) noexcept {
				this->_time = time;
			}

			ObjectTime (const Representation& time) noexcept {
				this->_time = std::chrono::duration_cast<timeType>(time);
			}

			ObjectTime (int year, int month = 1, int day = 1, int hour = 0, int minutes = 0, double seconds = 0.0) noexcept {
				date::year_month_day ymd{date::year(year), date::month(month), date::day(day)};

				this->_time += std::chrono::duration_cast<timeType>(std::chrono::hours(hour));
				this->_time += std::chrono::duration_cast<timeType>(std::chrono::minutes(minutes));
				this->_time += std::chrono::duration_cast<timeType>(std::chrono::duration<double, std::ratio<1, 1>>(seconds));
				this->_time += std::chrono::duration_cast<timeType>(date::sys_days(ymd).time_since_epoch ());
			}

			ObjectTime (const ObjectTime& other) noexcept : ObjectTime(other._time) {} // copy constructor

			ObjectTime (ObjectTime&& other) noexcept { this->_time = other._time; } // move constructor

			ObjectTime& operator=(const ObjectTime& other) noexcept { // copy assignment
				this->_time = other._time;
				return *this;
			}

			ObjectTime& operator=(ObjectTime&& other) noexcept { // move assignment
				this->_time = other._time;
				return *this;
			}

			ObjectTime& operator=(const internalTimeType& time) noexcept {
				this->_time = timeType{time};
				return *this;
			}

			void* operator new (size_t) = delete;
			void operator delete (void*) = delete;

			inline ObjectTime& operator+=(const ObjectTime& rhs)                noexcept { this->_time += rhs._time; return *this; }
			inline ObjectTime& operator+=(const Representation& rhs) noexcept { this->_time += std::chrono::duration_cast<timeType>(rhs); return *this; }
			inline ObjectTime& operator+=(const internalTimeType& rhs)          noexcept { this->_time += timeType{rhs}; return *this; }

			inline ObjectTime& operator-=(const ObjectTime& rhs)                noexcept { this->_time -= rhs._time; return *this; }
			inline ObjectTime& operator-=(const Representation& rhs) noexcept { this->_time -= std::chrono::duration_cast<timeType>(rhs); return *this; }
			inline ObjectTime& operator-=(const internalTimeType& rhs)          noexcept { this->_time -= timeType{rhs}; return *this; }

			inline ObjectTime& operator++(void) noexcept = delete;
			inline ObjectTime  operator++(int)  noexcept = delete;
			inline ObjectTime& operator--(void) noexcept = delete;
			inline ObjectTime  operator--(int)  noexcept = delete;

			explicit inline operator bool() const noexcept { return (this->_time != timeType::zero ()); }

			inline operator internalTimeType() const noexcept { return this->_time.count (); }
			explicit inline operator Representation() noexcept { return std::chrono::duration_cast<Representation>(this->_time); }

			#if INT_MAX >= UINT64_MAX
			explicit inline operator int() const noexcept { return static_cast<int>(this->_time.count ()); }
			#endif
			#if UINT_MAX >= UINT64_MAX
			explicit inline operator unsigned int() const noexcept { return static_cast<unsigned int>(this->_time.count ()); }
			#endif
			#if LONG_MAX >= UINT64_MAX
			explicit inline operator long int() const noexcept { return static_cast<long int>(this->_time.count ()); }
			#endif
			#if ULONG_MAX >= UINT64_MAX
			explicit inline operator long unsigned int() const noexcept { return static_cast<long unsigned int>(this->_time.count ()); }
			#endif
			#if LLONG_MAX >= UINT64_MAX
			explicit inline operator long long int() const noexcept { return static_cast<long long int>(this->_time.count ()); }
			#endif
			#if ULLONG_MAX >= UINT64_MAX
			explicit inline operator long long unsigned int() const noexcept { return static_cast<long long unsigned int>(this->_time.count ()); }
			#endif

			inline Representation get (void) const noexcept {
				return std::chrono::duration_cast<Representation>(this->_time);
			}

			inline internalTimeType getTime (void) const noexcept {
				return this->_time.count ();
			}

			inline int getHours (void) const noexcept {
				auto d = this->_time - std::chrono::duration_cast<date::days>(date::detail::abs(this->_time));
				std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(date::detail::abs(d));
				return h.count ();
			}

			inline int getMinutes (void) const noexcept {
				auto d = this->_time - std::chrono::duration_cast<std::chrono::hours>(date::detail::abs(this->_time));
				std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(date::detail::abs(d));
				return m.count ();
			}

			inline int getSeconds (void) const noexcept {
				auto d = this->_time - std::chrono::duration_cast<std::chrono::minutes>(date::detail::abs(this->_time));
				std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(date::detail::abs(d));
				return s.count ();
			}

			inline int getMilliseconds (void) const noexcept {
				auto d = this->_time - std::chrono::duration_cast<std::chrono::seconds>(date::detail::abs(this->_time));
				Representation ms = std::chrono::duration_cast<std::chrono::milliseconds>(date::detail::abs(d));
				return ms.count ();
			}

			inline int getDay (void) const noexcept {
				const date::sys_days tp{std::chrono::duration_cast<date::days>(this->_time)};
				const date::year_month_day ymd{tp};
				return static_cast<int>(static_cast<unsigned>(ymd.day ()));
			}

			inline int getMonth (void) const noexcept {
				const date::sys_days tp{std::chrono::duration_cast<date::days>(this->_time)};
				const date::year_month_day ymd{tp};
				return static_cast<int>(static_cast<unsigned>(ymd.month ()));
			}

			inline int getYear (void) const noexcept {
				const date::sys_days tp{std::chrono::duration_cast<date::days>(this->_time)};
				const date::year_month_day ymd{tp};
				return static_cast<int>(ymd.year ());
			}
		};

		inline bool operator==(const ObjectTime& lhs, const ObjectTime& rhs)       noexcept { return (lhs._time == rhs._time); }
		inline bool operator==(const ObjectTime& lhs, const ObjectTime::Representation& rhs)   noexcept { return (lhs._time == std::chrono::duration_cast<ObjectTime::timeType>(rhs)); }
		inline bool operator==(const ObjectTime::Representation& lhs, const ObjectTime& rhs)   noexcept { return (std::chrono::duration_cast<ObjectTime::timeType>(lhs) == rhs._time); }
		inline bool operator==(const ObjectTime& lhs, const ObjectTime::internalTimeType& rhs) noexcept { return (lhs._time.count () == rhs); }
		inline bool operator==(const ObjectTime::internalTimeType& lhs, const ObjectTime& rhs) noexcept { return (lhs == rhs._time.count ()); }

		inline bool operator!=(const ObjectTime& lhs, const ObjectTime& rhs)       noexcept { return (lhs._time != rhs._time); }
		inline bool operator!=(const ObjectTime& lhs, const ObjectTime::Representation& rhs)   noexcept { return (lhs._time != std::chrono::duration_cast<ObjectTime::timeType>(rhs)); }
		inline bool operator!=(const ObjectTime::Representation& lhs, const ObjectTime& rhs)   noexcept { return (std::chrono::duration_cast<ObjectTime::timeType>(lhs) != rhs._time); }
		inline bool operator!=(const ObjectTime& lhs, const ObjectTime::internalTimeType& rhs) noexcept { return (lhs._time.count () != rhs); }
		inline bool operator!=(const ObjectTime::internalTimeType& lhs, const ObjectTime& rhs) noexcept { return (lhs != rhs._time.count ()); }

		inline bool operator< (const ObjectTime& lhs, const ObjectTime& rhs)       noexcept { return (lhs._time < rhs._time); }
		inline bool operator< (const ObjectTime& lhs, const ObjectTime::Representation& rhs)   noexcept { return (lhs._time < std::chrono::duration_cast<ObjectTime::timeType>(rhs)); }
		inline bool operator< (const ObjectTime::Representation& lhs, const ObjectTime& rhs)   noexcept { return (std::chrono::duration_cast<ObjectTime::timeType>(lhs) < rhs._time); }
		inline bool operator< (const ObjectTime& lhs, const ObjectTime::internalTimeType& rhs) noexcept { return (lhs._time.count () < rhs); }
		inline bool operator< (const ObjectTime::internalTimeType& lhs, const ObjectTime& rhs) noexcept { return (lhs < rhs._time.count ()); }

		inline bool operator> (const ObjectTime& lhs, const ObjectTime& rhs)       noexcept { return (lhs._time > rhs._time); }
		inline bool operator> (const ObjectTime& lhs, const ObjectTime::Representation& rhs)   noexcept { return (lhs._time > std::chrono::duration_cast<ObjectTime::timeType>(rhs)); }
		inline bool operator> (const ObjectTime::Representation& lhs, const ObjectTime& rhs)   noexcept { return (std::chrono::duration_cast<ObjectTime::timeType>(lhs) > rhs._time); }
		inline bool operator> (const ObjectTime& lhs, const ObjectTime::internalTimeType& rhs) noexcept { return (lhs._time.count () > rhs); }
		inline bool operator> (const ObjectTime::internalTimeType& lhs, const ObjectTime& rhs) noexcept { return (lhs > rhs._time.count ()); }

		inline bool operator<=(const ObjectTime& lhs, const ObjectTime& rhs)       noexcept { return (lhs._time <= rhs._time); }
		inline bool operator<=(const ObjectTime& lhs, const ObjectTime::Representation& rhs)   noexcept { return (lhs._time <= std::chrono::duration_cast<ObjectTime::timeType>(rhs)); }
		inline bool operator<=(const ObjectTime::Representation& lhs, const ObjectTime& rhs)   noexcept { return (std::chrono::duration_cast<ObjectTime::timeType>(lhs) <= rhs._time); }
		inline bool operator<=(const ObjectTime& lhs, const ObjectTime::internalTimeType& rhs) noexcept { return (lhs._time.count () <= rhs); }
		inline bool operator<=(const ObjectTime::internalTimeType& lhs, const ObjectTime& rhs) noexcept { return (lhs <= rhs._time.count ()); }

		inline bool operator>=(const ObjectTime& lhs, const ObjectTime& rhs)       noexcept { return (lhs._time >= rhs._time); }
		inline bool operator>=(const ObjectTime& lhs, const ObjectTime::Representation& rhs)   noexcept { return (lhs._time >= std::chrono::duration_cast<ObjectTime::timeType>(rhs)); }
		inline bool operator>=(const ObjectTime::Representation& lhs, const ObjectTime& rhs)   noexcept { return (std::chrono::duration_cast<ObjectTime::timeType>(lhs) >= rhs._time); }
		inline bool operator>=(const ObjectTime& lhs, const ObjectTime::internalTimeType& rhs) noexcept { return (lhs._time.count () >= rhs); }
		inline bool operator>=(const ObjectTime::internalTimeType& lhs, const ObjectTime& rhs) noexcept { return (lhs >= rhs._time.count ()); }

		inline ObjectTime operator+(ObjectTime lhs, const ObjectTime& rhs)       noexcept { lhs._time += rhs._time; return lhs; }
		inline ObjectTime operator+(ObjectTime lhs, const ObjectTime::Representation& rhs)   noexcept { lhs._time += std::chrono::duration_cast<ObjectTime::timeType>(rhs); return lhs; }
		inline ObjectTime operator+(ObjectTime::Representation lhs, const ObjectTime& rhs)   noexcept { lhs += std::chrono::duration_cast<ObjectTime::Representation>(rhs._time); return lhs; }
		inline ObjectTime operator+(ObjectTime lhs, const ObjectTime::internalTimeType& rhs) noexcept { lhs._time += ObjectTime::timeType{rhs}; return lhs; }
		inline ObjectTime operator+(ObjectTime::internalTimeType lhs, const ObjectTime& rhs) noexcept { lhs += rhs._time.count (); return lhs; }

		inline ObjectTime operator-(ObjectTime lhs, const ObjectTime& rhs)       noexcept { lhs._time -= rhs._time; return lhs; }
		inline ObjectTime operator-(ObjectTime lhs, const ObjectTime::Representation& rhs)   noexcept { lhs._time -= std::chrono::duration_cast<ObjectTime::timeType>(rhs); return lhs; }
		inline ObjectTime operator-(ObjectTime::Representation lhs, const ObjectTime& rhs)   noexcept { lhs -= std::chrono::duration_cast<ObjectTime::Representation>(rhs._time); return lhs; }
		inline ObjectTime operator-(ObjectTime lhs, const ObjectTime::internalTimeType& rhs) noexcept { lhs._time -= ObjectTime::timeType{rhs}; return lhs; }
		inline ObjectTime operator-(ObjectTime::internalTimeType lhs, const ObjectTime& rhs) noexcept { lhs -= rhs._time.count (); return lhs; }
	}
}

#endif /* _X_GPSDATA_OBJECTTIME_ */