#ifndef _GPSDATA_GPSPOINTITERATOR_
#define _GPSDATA_GPSPOINTITERATOR_

#include <memory>
#include <functional>
#include <type_traits>
#include <source_location>
#include <Logger.hpp>

#include <gpsdata/traits/GpsRoute.hpp>

namespace gpsdata {
	template<GpsRouteTrait R, typename V = std::shared_ptr<typename R::Point>>
	class GpsPointIterator {
	public:
		typedef V value_type;
		typedef std::shared_ptr<typename R::Point> internal_value_type;
		typedef std::function<value_type(const internal_value_type)> GetterType;

	private:
		bool _valid = false;
		std::shared_ptr<R> _route;
		typename R::const_iterator _it_segment;
		typename R::Segment::const_iterator _it_point;
		GetterType _getter;

	public:
		GpsPointIterator (void) { }

		template<typename U = std::shared_ptr<typename R::Point>, typename std::enable_if<std::is_same<U, V>::value, bool>::type = 0>
		GpsPointIterator (const std::shared_ptr<R> route, const int& n_segment = -1, const bool& valid = true) noexcept : GpsPointIterator (route, n_segment, valid, &GpsPointIterator::defaultGetter) { }

		template<typename U = std::shared_ptr<typename R::Point>, typename std::enable_if<std::is_same<U, V>::value, bool>::type = 0>
		GpsPointIterator (const std::shared_ptr<R> route, const bool& valid) noexcept : GpsPointIterator (route, -1, valid, &GpsPointIterator::defaultGetter) { }

		GpsPointIterator (const std::shared_ptr<R> route, const GetterType getter) noexcept : GpsPointIterator (route, -1, true, getter) { }
		GpsPointIterator (const std::shared_ptr<R> route, const int& n_segment, const GetterType getter) noexcept : GpsPointIterator (route, n_segment, true, getter) { }
		GpsPointIterator (const std::shared_ptr<R> route, const bool& valid, const GetterType getter) noexcept : GpsPointIterator (route, -1, valid, getter) { }
		GpsPointIterator (const std::shared_ptr<R> route, int n_segment, const bool& valid, const GetterType getter) noexcept {
			DEBUG_MSG ("GpsPointIterator::{:s} ({:p}, {:d}, {:s}, {:s})\n", __func__, fmt::ptr (route.get ()), n_segment, valid, getter.target_type ().name ());
			DEBUG_2_MSG (2, "this: {:p}\n", fmt::ptr (this));

			if (!route) cpplogger::Logger::get() (cpplogger::Level::ERR, "Route variable is not pointing to a route object\n", std::source_location::current ());
			if (!getter) cpplogger::Logger::get() (cpplogger::Level::ERR, "Getter is not valid getter function\n", std::source_location::current ());

			this->_route = route;
			this->_getter = getter;

			if ((this->_valid = valid) && this->_route) {
				if (this->_route->cbegin () != this->_route->cend ()) {
					for (this->_it_segment = this->_route->cbegin (); this->_it_segment != this->_route->cend (); ++this->_it_segment) {
						if (n_segment > -1 && (*(this->_it_segment))->getSegmentNumber () != n_segment) continue; // if a specific segment number was given, continue till we encounter that segment number
						else n_segment = -1; // We found the correct segment, now stop segment number check and find first point
						if ((*(this->_it_segment))->cbegin () != (*(this->_it_segment))->cend ()) {
							this->_it_point = (*(this->_it_segment))->cbegin ();
							this->_valid = true;
							break;
						}
					}
				}
			}
		}

		~GpsPointIterator (void) = default;

		GpsPointIterator (const GpsPointIterator& other) : _route (other._route) { // copy constructor
			this->_valid = other._valid;
			this->_route = other._route;
			this->_it_segment = other._it_segment;
			this->_it_point = other._it_point;
			this->_getter = other._getter;
		}

		GpsPointIterator (GpsPointIterator&& other) noexcept { // move constructor
			this->_valid = other._valid;
			this->_route = other._route;
			this->_it_segment = other._it_segment;
			this->_it_point = other._it_point;
			this->_getter = other._getter;
			other._valid = false;
		}

		GpsPointIterator& operator= (const GpsPointIterator& other) { // copy assignment
			this->_valid = other._valid;
			this->_route = other._route;
			this->_it_segment = other._it_segment;
			this->_it_point = other._it_point;
			this->_getter = other._getter;
			return *this;
		}

		GpsPointIterator& operator= (GpsPointIterator&& other) noexcept { // move assignment
			this->_valid = other._valid;
			this->_route = other._route;
			this->_it_segment = other._it_segment;
			this->_it_point = other._it_point;
			this->_getter = other._getter;
			other._valid = false;
			return *this;
		}

		int getSegmentNumber (void) const noexcept {
			DEBUG_MSG ("GpsPointIterator::{:s} ({:p})\n", __func__, fmt::ptr (this));
			if (!this->_valid) return -1;
			if (this->_it_segment == this->_route->cend ()) return -1;
			const auto segment = *(this->_it_segment);
			return segment->getSegmentNumber ();
		}

		bool invalidate (void) noexcept {
			DEBUG_MSG ("GpsPointIterator::{:s} ({:p})\n", __func__, fmt::ptr (this));
			return (this->_valid = false);
		}

		value_type operator* (void) {
			DEBUG_MSG ("GpsPointIterator::{:s} ({:p})\n", __func__, fmt::ptr (this));
			if (this->_valid) {
				if (!this->_getter) cpplogger::Logger::get() (cpplogger::Level::ERR, "Getter is not valid getter function\n", std::source_location::current ());
				else {
					try {
						return this->_getter (*(this->_it_point));
					} catch (const std::bad_function_call& e) {
						EXCEPTION_ERROR_MSG (e);
						return {};
					}
				}
			}
			return {};
		}

		GpsPointIterator& operator++ (void) noexcept {
			DEBUG_MSG ("GpsPointIterator::{:s} ({:p})\n", __func__, fmt::ptr (this));
			if (!this->_valid) return *this;
			++this->_it_point;
			if (this->_it_point == (*(this->_it_segment))->cend ()) {
				this->_valid = false; // Set to false, till we find another point
				++this->_it_segment;
				for (; this->_it_segment != this->_route->cend (); ++this->_it_segment) {
					if ((*(this->_it_segment))->cbegin () != (*(this->_it_segment))->cend ()) {
						this->_it_point = (*(this->_it_segment))->cbegin ();
						this->_valid = true;
						break;
					}
				}
			}
			return *this;
		}

		GpsPointIterator& operator-- (void) noexcept {
			DEBUG_MSG ("GpsPointIterator::{:s} ({:p})\n", __func__, fmt::ptr (this));
			if (!this->_valid) return *this;

			if (this->_it_point == (*(this->_it_segment))->cbegin ()) {
				this->_valid = false; // Set to false, till we find another point

				while (this->_it_segment != this->_route->cbegin ()) {
					--this->_it_segment;
					if ((*(this->_it_segment))->cend () != (*(this->_it_segment))->cbegin ()) {
						this->_it_point = (*(this->_it_segment))->cend ();
						--this->_it_point;
						this->_valid = true;
						break;
					}
				}
			} else --this->_it_point;
			return *this;
		}

		GpsPointIterator operator++ (int) {
			DEBUG_MSG ("GpsPointIterator::{:s} ({:p})\n", __func__, fmt::ptr (this));
			GpsPointIterator old = *this; // copy old value
			this->operator++ (); // prefix increment
			return old;  // return old value
		}

		GpsPointIterator operator-- (int) {
			DEBUG_MSG ("GpsPointIterator::{:s} ({:p})\n", __func__, fmt::ptr (this));
			GpsPointIterator old = *this; // copy old value
			this->operator-- (); // prefix increment
			return old;  // return old value
		}

		bool operator== (const GpsPointIterator<R, V>& b) const {
			DEBUG_MSG ("GpsPointIterator::{:s} ({:p}, ...)\n", __func__, fmt::ptr (this));
			if (this->_valid && b._valid) {
				return (this->_route.get () == b._route.get () &&
				this->_it_segment == b._it_segment &&
				this->_it_point == b._it_point);
			} else if (this->_valid == b._valid) return true; // Both iterators are not valid (end of iterator, so both are equal (return true).
			else return false; // One iterator is valid the other not, they are for sure not equal.
		}

		bool operator!= (const GpsPointIterator<R, V>& b) const {
			DEBUG_MSG ("GpsPointIterator::{:s} ({:p})\n", __func__, fmt::ptr (this));
			return !this->operator== (b);
		}

		operator bool (void) const {
			DEBUG_MSG ("GpsPointIterator::{:s} ({:p})\n", __func__, fmt::ptr (this));
			return this->_valid;
		}

	private:
		template<typename U = std::shared_ptr<typename R::Point>, typename std::enable_if<std::is_same<U, V>::value, bool>::type = 0>
		inline static const value_type defaultGetter (const internal_value_type point) {
			DEBUG_MSG ("GpsPointIterator::{:s} ({:p})\n", __func__, fmt::ptr (point.get ()));
			return point;
		}
	};
}

#endif /* _GPSDATA_GPSPOINTITERATOR_ */