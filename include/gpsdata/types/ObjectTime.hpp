#ifndef _GPSDATA_TYPE_OBJECTTIME_
#define _GPSDATA_TYPE_OBJECTTIME_

#include <cstdint>
#include <climits>
#include <chrono>
#include <date/date.h>

namespace gpsdata {
	inline namespace types {
		class ObjectTime final {
			template <typename B>
			friend void serialize (B&, ObjectTime&);

		public:
			using internalTimeType = uint64_t;
			using internalPrecision = std::milli;
			using Representation = std::chrono::duration<internalTimeType, internalPrecision>;

			using timeType = Representation;

			friend bool operator==(const ObjectTime& lhs, const ObjectTime& rhs) noexcept;
			friend bool operator==(const ObjectTime& lhs, const ObjectTime::Representation& rhs) noexcept;
			friend bool operator==(const ObjectTime::Representation& lhs, const ObjectTime& rhs) noexcept;

			friend bool operator!=(const ObjectTime& lhs, const ObjectTime& rhs) noexcept;
			friend bool operator!=(const ObjectTime& lhs, const ObjectTime::Representation& rhs) noexcept;
			friend bool operator!=(const ObjectTime::Representation& lhs, const ObjectTime& rhs) noexcept;

			friend bool operator< (const ObjectTime& lhs, const ObjectTime& rhs) noexcept;
			friend bool operator< (const ObjectTime& lhs, const ObjectTime::Representation& rhs) noexcept;
			friend bool operator< (const ObjectTime::Representation& lhs, const ObjectTime& rhs) noexcept;

			friend bool operator> (const ObjectTime& lhs, const ObjectTime& rhs) noexcept;
			friend bool operator> (const ObjectTime& lhs, const ObjectTime::Representation& rhs) noexcept;
			friend bool operator> (const ObjectTime::Representation& lhs, const ObjectTime& rhs) noexcept;

			friend bool operator<=(const ObjectTime& lhs, const ObjectTime& rhs) noexcept;
			friend bool operator<=(const ObjectTime& lhs, const ObjectTime::Representation& rhs) noexcept;
			friend bool operator<=(const ObjectTime::Representation& lhs, const ObjectTime& rhs) noexcept;

			friend bool operator>=(const ObjectTime& lhs, const ObjectTime& rhs) noexcept;
			friend bool operator>=(const ObjectTime& lhs, const ObjectTime::Representation& rhs) noexcept;
			friend bool operator>=(const ObjectTime::Representation& lhs, const ObjectTime& rhs) noexcept;

			friend ObjectTime operator+(const ObjectTime& lhs, const ObjectTime::Representation& rhs) noexcept;
			friend ObjectTime operator+(const ObjectTime::Representation& rhs, const ObjectTime& lhs) noexcept;

			friend ObjectTime operator- (const ObjectTime& lhs, const ObjectTime::Representation& rhs) noexcept;
			friend ObjectTime::Representation operator- (const ObjectTime& lhs, const ObjectTime& rhs) noexcept;

		private:
			timeType _time;

		public:
			ObjectTime (void) noexcept { this->_time = timeType::zero (); }

			ObjectTime (const internalTimeType& time) noexcept {
				this->_time = timeType{time};
			}

			explicit ObjectTime (const Representation& time) noexcept {
				this->_time = std::chrono::duration_cast<timeType>(time);
			}

			template<class Rep, class Period>
			explicit ObjectTime (const std::chrono::duration<Rep, Period>& time) noexcept {
				this->_time = std::chrono::duration_cast<timeType>(time);
			}

			ObjectTime (int year, int month, int day, int hour = 0, int minutes = 0, double seconds = 0.0) noexcept {
				date::year_month_day ymd{date::year(year), date::month(month), date::day(day)};

				this->_time = std::chrono::duration_cast<timeType>(date::sys_days(ymd).time_since_epoch ());
				this->_time += std::chrono::duration_cast<timeType>(std::chrono::hours(hour));
				this->_time += std::chrono::duration_cast<timeType>(std::chrono::minutes(minutes));
				this->_time += std::chrono::duration_cast<timeType>(std::chrono::duration<double, std::ratio<1, 1>>(seconds));
			}

			ObjectTime (const ObjectTime& other) noexcept : ObjectTime(other._time) {} // copy constructor

			ObjectTime (ObjectTime&& other) noexcept { // move constructor
				this->_time = other._time;
				other._time = timeType::zero ();
			}

			ObjectTime& operator=(const ObjectTime& other) noexcept { // copy assignment
				this->_time = other._time;
				return *this;
			}

			ObjectTime& operator=(ObjectTime&& other) noexcept { // move assignment
				this->_time = other._time;
				other._time = timeType::zero ();
				return *this;
			}

			ObjectTime& operator=(const internalTimeType& time) noexcept {
				this->_time = timeType{time};
				return *this;
			}

			void* operator new (size_t) = delete;
			void operator delete (void*) = delete;

			template<class Rep, class Period>
			inline ObjectTime& operator+=(const std::chrono::duration<Rep, Period>& rhs) noexcept { this->_time += std::chrono::duration_cast<timeType>(rhs); return *this; }

			template<class Rep, class Period>
			inline ObjectTime& operator-=(const std::chrono::duration<Rep, Period>& rhs) noexcept { this->_time -= std::chrono::duration_cast<timeType>(rhs); return *this; }

			inline ObjectTime& operator++(void) noexcept = delete;
			inline ObjectTime  operator++(int)  noexcept = delete;
			inline ObjectTime& operator--(void) noexcept = delete;
			inline ObjectTime  operator--(int)  noexcept = delete;

			inline operator bool() const noexcept { return (this->_time != timeType::zero ()); }
			inline operator Representation (void) noexcept { return this->_time; }

			inline internalTimeType get (void) const noexcept {
				return this->_time.count ();
			}

			inline Representation getTime (void) const noexcept {
				return this->_time;
			}

			inline int getHours (void) const noexcept {
				auto d = this->_time - std::chrono::duration_cast<date::days>(date::detail::abs (this->_time));
				std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(date::detail::abs (d));
				return h.count ();
			}

			inline int getMinutes (void) const noexcept {
				auto d = this->_time - std::chrono::duration_cast<std::chrono::hours>(date::detail::abs (this->_time));
				std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(date::detail::abs (d));
				return m.count ();
			}

			inline int getSeconds (void) const noexcept {
				auto d = this->_time - std::chrono::duration_cast<std::chrono::minutes>(date::detail::abs (this->_time));
				std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(date::detail::abs (d));
				return s.count ();
			}

			inline int getMilliseconds (void) const noexcept {
				auto d = this->_time - std::chrono::duration_cast<std::chrono::seconds>(date::detail::abs (this->_time));
				Representation ms = std::chrono::duration_cast<std::chrono::milliseconds>(date::detail::abs (d));
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

		inline bool operator!=(const ObjectTime& lhs, const ObjectTime& rhs)       noexcept { return (lhs._time != rhs._time); }
		inline bool operator!=(const ObjectTime& lhs, const ObjectTime::Representation& rhs)   noexcept { return (lhs._time != std::chrono::duration_cast<ObjectTime::timeType>(rhs)); }
		inline bool operator!=(const ObjectTime::Representation& lhs, const ObjectTime& rhs)   noexcept { return (std::chrono::duration_cast<ObjectTime::timeType>(lhs) != rhs._time); }

		inline bool operator< (const ObjectTime& lhs, const ObjectTime& rhs)       noexcept { return (lhs._time < rhs._time); }
		inline bool operator< (const ObjectTime& lhs, const ObjectTime::Representation& rhs)   noexcept { return (lhs._time < std::chrono::duration_cast<ObjectTime::timeType>(rhs)); }
		inline bool operator< (const ObjectTime::Representation& lhs, const ObjectTime& rhs)   noexcept { return (std::chrono::duration_cast<ObjectTime::timeType>(lhs) < rhs._time); }

		inline bool operator> (const ObjectTime& lhs, const ObjectTime& rhs)       noexcept { return (lhs._time > rhs._time); }
		inline bool operator> (const ObjectTime& lhs, const ObjectTime::Representation& rhs)   noexcept { return (lhs._time > std::chrono::duration_cast<ObjectTime::timeType>(rhs)); }
		inline bool operator> (const ObjectTime::Representation& lhs, const ObjectTime& rhs)   noexcept { return (std::chrono::duration_cast<ObjectTime::timeType>(lhs) > rhs._time); }

		inline bool operator<=(const ObjectTime& lhs, const ObjectTime& rhs)       noexcept { return (lhs._time <= rhs._time); }
		inline bool operator<=(const ObjectTime& lhs, const ObjectTime::Representation& rhs)   noexcept { return (lhs._time <= std::chrono::duration_cast<ObjectTime::timeType>(rhs)); }
		inline bool operator<=(const ObjectTime::Representation& lhs, const ObjectTime& rhs)   noexcept { return (std::chrono::duration_cast<ObjectTime::timeType>(lhs) <= rhs._time); }

		inline bool operator>=(const ObjectTime& lhs, const ObjectTime& rhs)       noexcept { return (lhs._time >= rhs._time); }
		inline bool operator>=(const ObjectTime& lhs, const ObjectTime::Representation& rhs)   noexcept { return (lhs._time >= std::chrono::duration_cast<ObjectTime::timeType>(rhs)); }
		inline bool operator>=(const ObjectTime::Representation& lhs, const ObjectTime& rhs)   noexcept { return (std::chrono::duration_cast<ObjectTime::timeType>(lhs) >= rhs._time); }

		inline ObjectTime operator+ (const ObjectTime& lhs, const ObjectTime::Representation& rhs) noexcept { ObjectTime::Representation r = lhs._time + rhs; return ObjectTime (r); };
		inline ObjectTime operator+ (const ObjectTime::Representation& lhs, const ObjectTime& rhs) noexcept { ObjectTime::Representation r = rhs._time + lhs; return ObjectTime (r); };
		inline ObjectTime operator- (const ObjectTime& lhs, const ObjectTime::Representation& rhs) noexcept { ObjectTime::Representation r = lhs._time - rhs; return ObjectTime (r); };
		inline ObjectTime::Representation operator- (const ObjectTime& lhs, const ObjectTime& rhs) noexcept { ObjectTime::Representation r = lhs._time - rhs._time; return r; };
	}
}

#endif /* _GPSDATA_TYPE_OBJECTTIME_ */