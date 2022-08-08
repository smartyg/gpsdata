#ifndef _X_GPSDATA_GPSROUTE_
#define _X_GPSDATA_GPSROUTE_

#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>
#include <memory>

#include <gpsdata/types/ObjectId.hpp>
#include <gpsdata/types/ObjectTime.hpp>
#include <gpsdata/GpsValue.hpp>
#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/GpsFactoryUserBase.hpp>
#include <gpsdata/GpsStatistics.hpp>
#include <gpsdata/GpsPoint.hpp>
#include <gpsdata/GpsSegment.hpp>

namespace gpsdata {
	template<GpsDataFactory F, class S = GpsSegment<F>, class P = typename S::Point>
	class GpsRoute : virtual public GpsStatistics<F>, virtual public internal::GpsFactoryUserBase<F>, std::enable_shared_from_this<GpsRoute<F, S, P>> {
		static_assert (std::is_base_of<GpsSegment<F>, S>::value);
		static_assert (std::is_base_of<typename S::Point, P>::value);
		static_assert (std::is_base_of<GpsPoint<F>, P>::value);
		static_assert (std::is_same<typename S::GpsFactory, F>::value);
		static_assert (std::is_same<typename P::GpsFactory, F>::value);

	public:
		using GpsFactory = F;
		using Segment = S;
		using Point = P;
		using DataType = typename F::DataType;
		using ActivityType = typename F::ActivityType;

		using Container = typename std::vector<std::shared_ptr<S>>;
		using iterator = typename Container::iterator;
		using const_iterator = typename Container::const_iterator;

	protected:
		ObjectId _id;
		int32_t _timezone_offset;
		ActivityType _activity_type;
		std::string _title;
		std::string _summary;
		std::string _details;

		Container _segments;

		GpsRoute (const ObjectId& id, const std::shared_ptr<const F>& factory) : internal::GpsFactoryUserBase<F> (factory), GpsStatistics<F> (factory) {
			DEBUG_MSG("GpsRoute::%s (%ld, %p)\n", __func__, static_cast<int64_t>(id), &factory);
			this->_id = id;
			//this->_time = 0;
			this-> _timezone_offset = 0;
			this->_title = {};
			this->_summary = {};
			this->_details = {};
		}

		GpsRoute (const std::shared_ptr<const F>& factory) : GpsRoute (0, factory) { }

	private:
		GpsRoute (void) = delete;
		GpsRoute (const GpsRoute&) = delete;                // copy constructor
		GpsRoute (GpsRoute&&) noexcept = delete;            // move constructor
		GpsRoute& operator= (const GpsRoute&) = delete;     // copy assignment
		GpsRoute& operator= (GpsRoute&&) noexcept = delete; // move assignment

	public:
		~GpsRoute (void) {
			DEBUG_MSG("GpsRoute::%s ()\n", __func__);
			this->_segments.clear ();
			this->_title.clear ();// = nullptr;
			this->_summary.clear ();// = nullptr;
			this->_details.clear ();// = nullptr;
		}

		[[nodiscard]] static std::shared_ptr<GpsRoute<F, S, P>> create (const ObjectId& id, const std::shared_ptr<const F>& factory) {
			DEBUG_MSG("GpsRoute::%s (%ld, %p)\n", __func__, static_cast<int64_t>(id), &factory);
			return std::shared_ptr<GpsRoute<F, S, P>>(new GpsRoute<F, S, P> (id, factory));
		}

		[[nodiscard]] static std::shared_ptr<GpsRoute<F, S, P>> create (const std::shared_ptr<const F>& factory) {
			DEBUG_MSG("GpsRoute::%s (%p)\n", __func__, &factory);
			return std::shared_ptr<GpsRoute<F, S, P>>(new GpsRoute<F, S, P> (factory));
		}

		std::shared_ptr<GpsRoute<F, S, P>> getptr (void) {
			return this->shared_from_this ();
		}

		/* Route data setters */
		/*
		bool setTime (const ObjectTime& time) {
			DEBUG_MSG("GpsRoute::%s (%d)\n", __func__, time);
			this->_time = time;
			return true;
		}*/

		bool setTimezoneOffset (int32_t offset) {
			DEBUG_MSG("GpsRoute::%s (%d)\n", __func__, offset);
			this->_timezone_offset = offset;
			return true;
		}

		bool setActivityType (const ActivityType& a) {
			DEBUG_MSG("GpsRoute::%s (%d)\n", __func__, a);
			this->_activity_type = a;
			return true;
		}

		template<typename U = std::string, typename std::enable_if<!std::is_same<U, ActivityType>::value, bool>::type = 0>
		bool setActivityType (const std::string& a_str) {
			DEBUG_MSG("GpsRoute::%s (%s)\n", __func__, a_str.c_str ());
			const ActivityType& a = this->_factory->getActivityType (a_str);
			this->_activity_type = a;
			return true;
		}

		bool setTitle (const std::string& title) {
			DEBUG_MSG("GpsRoute::%s (%s)\n", __func__, title.c_str ());
			this->_title = title;
			return true;
		}

		bool setSummary (const std::string& summary) {
			DEBUG_MSG("GpsRoute::%s (%s)\n", __func__, summary.c_str ());
			this->_summary = summary;
			return true;
		}

		bool setDetails (const std::string& details) {
			DEBUG_MSG("GpsRoute::%s (%s)\n", __func__, details.c_str ());
			this->_details = details;
			return true;
		}

		/* Route data getters */
		const ObjectId getId (void) const {
			DEBUG_MSG("GpsRoute::%s ()\n", __func__);
			return this->_id;
		}

		const ObjectTime getTime (void) const {
			DEBUG_MSG("GpsRoute::%s ()\n", __func__);
			if (!this->hasSegment ()) throw std::runtime_error ("no segments present");
			const auto it = this->_segments.cbegin ();
			return *it->getTime ();
		}

		int32_t getTimezoneOffset (void) const {
			DEBUG_MSG("GpsRoute::%s ()\n", __func__);
			return this->_timezone_offset;
		}

		const ActivityType getActivityType (void) const {
			DEBUG_MSG("GpsRoute::%s ()\n", __func__);
			return this->_activity_type;
		}

		const std::string getTitle (void) const {
			DEBUG_MSG("GpsRoute::%s ()\n", __func__);
			return this->_title;
		}

		const std::string getSummary (void) const {
			DEBUG_MSG("GpsRoute::%s ()\n", __func__);
			return this->_summary;
		}

		const std::string getDetails (void) const {
			DEBUG_MSG("GpsRoute::%s ()\n", __func__);
			return this->_details;
		}

		/* GPS Segments */
		bool addSegment (std::shared_ptr<S> segment) {
			DEBUG_MSG("GpsRoute::%s (%p)\n", __func__, &segment);
			this->_segments.push_back (segment);
			return true;
		}

		bool addSegment (const int& n_segment) {
			DEBUG_MSG("GpsRoute::%s (%d)\n", __func__, n_segment);
			std::shared_ptr<Segment> segment = Segment::create (n_segment, this->_factory);
			return this->addSegment (segment);
		}

		bool hasSegment (void) const {
			DEBUG_MSG("GpsRoute::%s ()\n", __func__);
			return (this->_segments.size () >= 1);
		}

		bool hasSegment (const int& n_segment) const {
			DEBUG_MSG("GpsRoute::%s (%d)\n", __func__, n_segment);
			for (const std::shared_ptr<S>& s : this->_segments) {
				if (s->getSegmentNumber () == n_segment) return true;
			}
			return false;
		}

		int nSegments (void) const {
			DEBUG_MSG("GpsRoute::%s ()\n", __func__);
			return this->_segments.size ();
		}

		const std::vector<std::shared_ptr<S>> getSegments (void) const {
			DEBUG_MSG("GpsRoute::%s ()\n", __func__);
			return this->_segments;
		}

		/* GPS Points */
		bool hasPoint (const ObjectTime& time) const {
			DEBUG_MSG("GpsRoute::%s (%ld)\n", __func__, time.getTime ());
			/* TODO */
			(void)time;
			return true;
		}

		bool addPoint (const int& segment, std::shared_ptr<Point> point) {
			DEBUG_MSG("GpsRoute::%s (%d, %p)\n", __func__, segment, point);
			if (!this->hasSegment (segment)) this->addSegment (segment);
			return this->_segments[segment]->addPoint (point);
		}

		bool addPoint (int segment, const ObjectTime& time) {
			DEBUG_MSG("GpsRoute::%s (%d, %ld)\n", __func__, segment, time.getTime ());
			if (!this->hasSegment (segment)) this->addSegment (segment);
			return this->_segments[segment]->addPoint (time);
		}

		bool addPointData (const int& segment, const ObjectTime& time, const GpsValue<DataType>& value)  {
			DEBUG_MSG("GpsRoute::%s (%d, %ld)\n", __func__, segment, time.getTime ());
			if (!this->hasSegment (segment)) this->addSegment (segment);
			return this->_segments[segment]->addPointData (time, value);
		}

		template<class T>
		inline bool addPointData (const int& segment, const ObjectTime& time, const DataType& type, const T& value, bool best_effort = false) {
			DEBUG_MSG("GpsRoute::%s (%d, %ld, %d, ...)\n", __func__, segment, time.getTime (), type);
			GpsValue<DataType> data;
			if (this->_factory->setValue (data, type, value, best_effort))
				return this->addPointData (segment, time, data);
			return false;
		}

		template<class T, typename U = std::string, typename std::enable_if<!std::is_same<U, DataType>::value, bool>::type = 0>
		inline bool addPointData (const int& segment, const ObjectTime& time, const std::string& type_str, const T& value, bool best_effort = false) {
			DEBUG_MSG("GpsRoute::%s (%d, %ld, %s, ...)\n", __func__, segment, time.getTime (), type_str.c_str ());
			GpsValue<DataType> data;
			DataType type = this->_factory->getDataType (type_str);
			if (this->_factory->setValue (data, type, value, best_effort))
				return this->addPointData (segment, time, data);
			return false;
		}

		iterator begin(void) {
			return this->_segments.begin ();
		}

		const_iterator begin (void) const {
			return this->_segments.begin ();
		}

		const_iterator cbegin (void) const {
			return this->_segments.cbegin ();
		}

		iterator end (void) {
			return this->_segments.end ();
		}

		const_iterator end (void) const {
			return this->_segments.end ();
		}

		const_iterator cend (void) const {
			return this->_segments.cend ();
		}
	};
}

#endif /* _X_GPSDATA_GPSROUTE_ */
