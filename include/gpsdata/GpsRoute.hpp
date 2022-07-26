#ifndef _GPSDATA_GPSROUTE_
#define _GPSDATA_GPSROUTE_

#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>
#include <memory>
#include <Logger.hpp>
#include <date/date.h>

#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/traits/GpsSegment.hpp>
#include <gpsdata/traits/GpsRoute.hpp>
#include <gpsdata/types/ObjectId.hpp>
#include <gpsdata/types/ObjectTime.hpp>
#include <gpsdata/GpsValue.hpp>
#include <gpsdata/GpsFactoryUserBase.hpp>
#include <gpsdata/GpsStatistics.hpp>
#include <gpsdata/GpsPoint.hpp>
#include <gpsdata/GpsSegment.hpp>
#include <gpsdata/utils/PointDate.hpp>

namespace bitsery {
	class Access;
}

namespace gpsdata {

	template<GpsFactoryTrait F, GpsSegmentTrait S = GpsSegment<F>>
	class GpsRoute : virtual public GpsStatistics<F>, virtual public internal::GpsFactoryUserBase<F>, std::enable_shared_from_this<GpsRoute<F, S>> {
		friend class bitsery::Access;

		template<typename B, GpsRouteTrait R>
		friend void serialize (B&, std::shared_ptr<R>&) requires(std::is_base_of<GpsRoute<typename R::GpsFactory, typename R::Segment>, R>::value);

		static_assert (std::is_same<typename S::GpsFactory, F>::value);

	public:
		using GpsFactory = F;
		using Segment = S;
		using Point = typename S::Point;
		using DataType = typename F::DataType;
		using ActivityType = typename F::ActivityType;

		using Container = typename std::vector<std::shared_ptr<S>>;
		using iterator = typename Container::iterator;
		using const_iterator = typename Container::const_iterator;

	protected:
		ObjectId _id;
		ActivityType _activity_type;
		std::string _title;
		std::string _summary;
		std::string _details;

		Container _segments;

		GpsRoute (const ObjectId& id, const std::shared_ptr<const F>& factory) : internal::GpsFactoryUserBase<F> (factory), GpsStatistics<F> (factory) {
			DEBUG_MSG ("GpsRoute::{:s} ({:d}, {:p})\n", __func__, static_cast<int64_t>(id), fmt::ptr (factory));
			this->_id = id;
			this->_title = {};
			this->_summary = {};
			this->_details = {};
		}

		GpsRoute (const std::shared_ptr<const F>& factory) : GpsRoute (0, factory) {
			DEBUG_MSG ("GpsRoute::{:s} ({:p})\n", __func__, fmt::ptr (factory));
		}

	private:
		GpsRoute (void) = default;
		GpsRoute (const GpsRoute&) = delete;                // copy constructor
		GpsRoute (GpsRoute&&) noexcept = delete;            // move constructor
		GpsRoute& operator= (const GpsRoute&) = delete;     // copy assignment
		GpsRoute& operator= (GpsRoute&&) noexcept = delete; // move assignment

	public:
		~GpsRoute (void) {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			this->_segments.clear ();
			this->_title.clear ();// = nullptr;
			this->_summary.clear ();// = nullptr;
			this->_details.clear ();// = nullptr;
		}

		template<GpsRouteTrait R = GpsRoute<F, S>>
		[[nodiscard]] static std::shared_ptr<R> create (const ObjectId& id, const std::shared_ptr<const typename R::GpsFactory>& factory) {
			DEBUG_MSG ("GpsRoute::{:s} ({:d}, {:p})\n", __func__, static_cast<int64_t>(id), fmt::ptr (factory));
			return std::shared_ptr<R>(new R (id, factory));
		}

		template<GpsRouteTrait R = GpsRoute<F, S>>
		[[nodiscard]] static std::shared_ptr<R> create (const std::shared_ptr<const typename R::GpsFactory>& factory) {
			DEBUG_MSG ("GpsRoute::{:s} ({:p})\n", __func__, fmt::ptr (factory));
			return std::shared_ptr<R>(new R (factory));
		}

		std::shared_ptr<GpsRoute<F, S>> getptr (void) {
			return this->shared_from_this ();
		}

		/* Route data setters */
		bool setId (const ObjectId& id) {
			DEBUG_MSG ("GpsRoute::{:s} ({:d})\n", __func__, id);
			if (!this->_id)
				this->_id = id;
			return (this->_id == id);
		}

		bool setActivityType (const ActivityType& a) {
			DEBUG_MSG ("GpsRoute::{:s} ({:d})\n", __func__, a);
			this->_activity_type = a;
			return (this->_activity_type == a);
		}

		template<typename U = std::string, typename std::enable_if<!std::is_same<U, ActivityType>::value, bool>::type = 0>
		bool setActivityType (const std::string& a_str) {
			DEBUG_MSG ("GpsRoute::{:s} ({:s})\n", __func__, a_str);
			const ActivityType& a = this->_factory->getActivityType (a_str);
			this->_activity_type = a;
			return (this->_activity_type == a);
		}

		bool setTitle (const std::string& title) {
			DEBUG_MSG ("GpsRoute::{:s} ({:s})\n", __func__, title);
			this->_title = title;
			return true;
		}

		bool setSummary (const std::string& summary) {
			DEBUG_MSG ("GpsRoute::{:s} ({:s})\n", __func__, summary);
			this->_summary = summary;
			return true;
		}

		bool setDetails (const std::string& details) {
			DEBUG_MSG ("GpsRoute::{:s} ({:s})\n", __func__, details);
			this->_details = details;
			return true;
		}

		/* Route data getters */
		const ObjectId getId (void) const {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			return this->_id;
		}

		const ObjectTime getTime (void) const {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			if (!this->hasSegment ()) throw std::runtime_error ("no segments present");
			const auto it = this->_segments.cbegin ();
			return (*it)->getTime ();
		}

		const date::zoned_time<ObjectTime::timeType> getTimezone (void) const {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			if (!this->hasSegment ()) throw std::runtime_error ("no segments present");
			const std::shared_ptr<Segment>& segment = *(this->cbegin ());
			if (segment) {
				const std::shared_ptr<Point>& point = *(segment->cbegin ());
				if (point) {
					return utils::PointDate::getZonedTime (point);
				}
			}
			return {};
		}

		int64_t getTimezoneOffset (void) const {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			const date::zoned_time<ObjectTime::timeType> zone = this->getTimezone ();
			const std::chrono::seconds offset = zone.get_info ().offset;
			return static_cast <int64_t>(offset.count ());
		}

		const ActivityType getActivityType (void) const {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			return this->_activity_type;
		}

		const std::string getTitle (void) const {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			return this->_title;
		}

		const std::string getSummary (void) const {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			return this->_summary;
		}

		const std::string getDetails (void) const {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			return this->_details;
		}

		/* GPS Segments */
		bool addSegment (std::shared_ptr<S> segment) {
			DEBUG_MSG ("GpsRoute::{:s} ({:p})\n", __func__, fmt::ptr (segment));
			this->_segments.push_back (segment);
			return true;
		}

		bool addSegment (const int& n_segment) {
			DEBUG_MSG ("GpsRoute::{:s} ({:d})\n", __func__, n_segment);
			std::shared_ptr<Segment> segment = Segment::template create<Segment> (n_segment, this->_factory);
			return this->addSegment (segment);
		}

		bool hasSegment (void) const {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			return (this->_segments.size () >= 1);
		}

		bool hasSegment (const int& n_segment) const {
			DEBUG_MSG ("GpsRoute::{:s} ({:d})\n", __func__, n_segment);
			for (const std::shared_ptr<S>& s : this->_segments) {
				if (s->getSegmentNumber () == n_segment) return true;
			}
			return false;
		}

		int nSegments (void) const {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			return this->_segments.size ();
		}

		const std::vector<std::shared_ptr<S>> getSegments (void) const {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			return this->_segments;
		}

		const std::shared_ptr<S> getSegment (const int& n_segment) const {
			DEBUG_MSG ("GpsRoute::{:s} ()\n", __func__);
			for (const std::shared_ptr<S>& s : this->_segments) {
				if (s->getSegmentNumber () == n_segment)
					return s;
			}
			return {};
		}

		/* GPS Points */
		bool hasPoint (const ObjectTime& time) const {
			DEBUG_MSG ("GpsRoute::{:s} ({:d})\n", __func__, time.get ());
			/* TODO */
			(void)time;
			return true;
		}

		bool addPoint (const int& segment, std::shared_ptr<Point> point) {
			DEBUG_MSG ("GpsRoute::{:s} ({:d}, {:p})\n", __func__, segment, fmt::ptr (point));
			if (!this->hasSegment (segment)) this->addSegment (segment);
			return this->_segments[segment]->addPoint (point);
		}

		bool addPoint (int segment, const ObjectTime& time) {
			DEBUG_MSG ("GpsRoute::{:s} ({:d}, {:d})\n", __func__, segment, time.get ());
			if (!this->hasSegment (segment)) this->addSegment (segment);
			return this->_segments[segment]->addPoint (time);
		}

		bool addPointData (const int& segment, const ObjectTime& time, const GpsValue<DataType>& value)  {
			DEBUG_MSG ("GpsRoute::{:s} ({:d}, {:d}, ...)\n", __func__, segment, time.get ());
			if (!this->hasSegment (segment)) this->addSegment (segment);
			return this->_segments[segment]->addPointData (time, value);
		}

		template<class T>
		inline bool addPointData (const int& segment, const ObjectTime& time, const DataType& type, const T& value, bool best_effort = false) {
			DEBUG_MSG ("GpsRoute::{:s} ({:d}, {:d}, {:d}, {}, {:d})\n", __func__, segment, time.get (), type, value, best_effort);
			GpsValue<DataType> data;
			if (this->_factory->setValue (data, type, value, best_effort))
				return this->addPointData (segment, time, data);
			return false;
		}

		template<class T, typename U = std::string, typename std::enable_if<!std::is_same<U, DataType>::value, bool>::type = 0>
		inline bool addPointData (const int& segment, const ObjectTime& time, const std::string& type_str, const T& value, bool best_effort = false) {
			DEBUG_MSG ("GpsRoute::{:s} ({:d}, {:d}, {:s}, {}, {:d})\n", __func__, segment, time.get (), type_str, value, best_effort);
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

		operator bool (void) const {
			return (this->_id);
		}
	};
}

#endif /* _GPSDATA_GPSROUTE_ */
