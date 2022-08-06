#ifndef _X_GPSDATA_GPSSEGMENT_
#define _X_GPSDATA_GPSSEGMENT_

#include <cstdint>
#include <string>
#include <type_traits>
#include <list>
#include <memory>

#include <gpsdata/types/ObjectTime.hpp>
#include <gpsdata/GpsValue.hpp>
#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/GpsFactoryUserBase.hpp>
#include <gpsdata/GpsStatistics.hpp>
#include <gpsdata/GpsPoint.hpp>

namespace gpsdata {
	template<GpsDataFactory F, class P = GpsPoint<F>>
	class GpsSegment : virtual public GpsStatistics<F>, virtual public internal::GpsFactoryUserBase<F>, std::enable_shared_from_this<GpsSegment<F, P>> {
		static_assert (std::is_base_of<GpsPoint<F>, P>::value);
		static_assert (std::is_same<typename P::GpsFactory, F>::value);

	public:
		using GpsFactory = F;
		using Point = P;
		using DataType = typename F::DataType;

		using Container = typename std::list<std::shared_ptr<Point>>;
		using iterator = typename Container::iterator;
		using const_iterator = typename Container::const_iterator;

	protected:
		int _n;
		Container _points;

		GpsSegment (const std::shared_ptr<const F>& factory) : internal::GpsFactoryUserBase<F> (factory), GpsStatistics<F> (factory) {
			DEBUG_MSG("GpsSegment::%s (%p)\n", __func__, &factory);
			this->_n = -1;
		}

		GpsSegment (const int& n, const std::shared_ptr<const F>& factory) : internal::GpsFactoryUserBase<F> (factory), GpsStatistics<F> (factory) {
			DEBUG_MSG("GpsSegment::%s (%d, %p)\n", __func__, n, &factory);
			this->_n = n;
		}

	private:
		GpsSegment (void) = delete;
		GpsSegment (const GpsSegment&) = delete;                // copy constructor
		GpsSegment (GpsSegment&&) noexcept = delete;            // move constructor
		GpsSegment& operator= (const GpsSegment&) = delete;     // copy assignment
		GpsSegment& operator= (GpsSegment&&) noexcept = delete; // move assignment

	public:
		~GpsSegment (void) {
			DEBUG_MSG("GpsSegment::%s ()\n", __func__);
			this->_points.clear ();
		}

		template <class S = GpsSegment<F, P>>
		[[nodiscard]] static std::shared_ptr<S> create (const std::shared_ptr<const typename S::GpsFactory>& factory) {
			DEBUG_MSG("GpsSegment::%s (%p)\n", __func__, &factory);
			return std::shared_ptr<S>(new S (factory));
		}

		template <class S = GpsSegment<F, P>>
		[[nodiscard]] static std::shared_ptr<S> create (const int& n, const std::shared_ptr<const typename S::GpsFactory>& factory) {
			DEBUG_MSG("GpsSegment::%s (%d, %p)\n", __func__, n, &factory);
			return std::shared_ptr<S>(new S (n, factory));
		}

		std::shared_ptr<GpsSegment<F, P>> getptr (void) {
			return this->shared_from_this ();
		}

		int getSegmentNumber (void) const  {
			DEBUG_MSG("GpsSegment::%s ()\n", __func__);
			return this->_n;
		}

		const ObjectTime getTime (void) const {
			DEBUG_MSG("GpsSegment::%s ()\n", __func__);
			if (!this->hasPoint ()) throw std::runtime_error ("no points present");
			const auto it = this->_points.cbegin ();
			return *it->getTime ();
		}

		/* GPS Points */
		bool addPoint (std::shared_ptr<P>& point) {
			DEBUG_MSG("GpsSegment::%s (%p)\n", __func__, &point);
			const ObjectTime time = point->getTime ();
			auto it = this->getIterator (time);
			if (GpsSegment::iteratorMatch (it, time)) return false;
			this->_points.push_back (point);
			return true;
		};

		template<class T>
		bool addPoint (const ObjectTime& time) {
			DEBUG_MSG("GpsSegment::%s (%ld)\n", __func__, time.getTime ());
			auto it = this->getIterator (time);
			if (GpsSegment::iteratorMatch (it, time)) return false;
			std::shared_ptr<Point> point = Point::template create<Point> (time, this->_factory);
			this->_points.insert (it, point);
			return true;
		}

		bool addPointData (const ObjectTime& time, const GpsValue<DataType>& value) {
			DEBUG_MSG("GpsSegment::%s (%ld, ...)\n", __func__, time.getTime ());
			auto it = this->getIterator (time);
			// If the point does not (yet) exists, create create the point; otherwise add the data to the point.
			if (!GpsSegment::iteratorMatch (it, time)) {
				std::shared_ptr<Point> point = Point::template create<Point> (time, this->_factory);
				point->addData (value);
				return this->addPoint (point);
			}
			(*it)->addData (value);
			return true;
		}

		template<class T>
		inline bool addPointData (const ObjectTime& time, const DataType& type, const T& value, bool best_effort = false) {
			DEBUG_MSG("GpsSegment::%s (%ld, %d, ..., %d)\n", __func__, time.getTime (), type, best_effort);
			GpsValue<DataType> data;
			this->_factory->setValue (data, type, value, best_effort);
			return this->addPointData (time, data);
		}

		template<class T, typename U = std::string, typename std::enable_if<!std::is_same<U, DataType>::value, bool>::type = 0>
		inline bool addPointData (const ObjectTime& time, const std::string& type_str, const T& value, bool best_effort = false) {
			DEBUG_MSG("GpsSegment::%s (%ld, %s, ..., %d)\n", __func__, time.getTime (), type_str.c_str (), best_effort);
			GpsValue<DataType> data;
			DataType type = this->_factory->getDataType (type_str);
			this->_factory->setValue (data, type, value, best_effort);
			return this->addPointData (time, data);
		}

		bool hasPoint () const {
			DEBUG_MSG("GpsSegment::%s ()\n", __func__);
			return (this->_points.size () >= 1);
		}

		bool hasPoint (const ObjectTime& time) const {
			DEBUG_MSG("GpsSegment::%s (%ld)\n", __func__, time.getTime ());
			auto it = this->getIterator (time);
			if ((*it)->getTime () == time) return true;
			return false;
		}

		const std::list<std::shared_ptr<P>> getPoints (void) {
			DEBUG_MSG("GpsSegment::%s ()\n", __func__);
			return this->_points;
		}

		iterator begin(void) {
			return this->_points.begin ();
		}

		const_iterator begin (void) const {
			return this->_points.begin ();
		}

		const_iterator cbegin (void) const {
			return this->_points.cbegin ();
		}

		iterator end (void) {
			return this->_points.end ();
		}

		const_iterator end (void) const {
			return this->_points.end ();
		}

		const_iterator cend (void) const {
			return this->_points.cend ();
		}

	private:
		typename std::list<std::shared_ptr<P>>::iterator getIterator (const ObjectTime& time) {
			DEBUG_MSG("GpsSegment::%s (%ld)\n", __func__, time.getTime ());
			auto it = this->_points.end ();
			--it;
			for (; it != this->_points.begin (); --it) {
				if ((*it)->getTime () == time) return it;
				if ((*it)->getTime () < time) return it++;
			}
			return this->_points.begin ();
		}

		typename std::list<std::shared_ptr<P>>::const_iterator getIterator (const ObjectTime& time) const {
			DEBUG_MSG("GpsSegment::%s (%ld)\n", __func__, time.getTime ());
			auto it = this->_points.end ();
			--it;
			for (; it != this->_points.begin (); --it) {
				if ((*it)->getTime () == time) return it;
				if ((*it)->getTime () < time) return it++;
			}
			return this->_points.begin ();
		}

		static bool iteratorMatch (typename std::list<std::shared_ptr<P>>::iterator it, const ObjectTime& time) {
			DEBUG_MSG("GpsSegment::%s (%p, %ld)\n", __func__, &it, time.getTime ());
			if (*it == nullptr) return false;
			return ((*it)->getTime () == time);
		}
	};
}

#endif /* _X_GPSDATA_GPSSEGMENT_ */
