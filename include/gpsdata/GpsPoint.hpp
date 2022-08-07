#ifndef _X_GPSDATA_GPSPOINT_
#define _X_GPSDATA_GPSPOINT_

#include <type_traits>
#include <vector>
#include <memory>

#include <gpsdata/types/ObjectTime.hpp>
#include <gpsdata/GpsValue.hpp>
#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/GpsFactoryUserBase.hpp>

namespace gpsdata {
	template<GpsDataFactory F>
	class GpsPoint : virtual public internal::GpsFactoryUserBase<F>, std::enable_shared_from_this<GpsPoint<F>> {

	public:
		using GpsFactory = F;
		using DataType = typename F::DataType;

		using Container = typename std::vector<GpsValue<DataType>>;
		using iterator = typename Container::iterator;
		using const_iterator = typename Container::const_iterator;

	protected:
		const ObjectTime _time;
		Container _data;

		GpsPoint (const ObjectTime& time, const std::shared_ptr<const F>& factory) : internal::GpsFactoryUserBase<F> (factory), _time(time) {
			//DEBUG_MSG("GpsPoint::%s (%p, %ld)\n", __func__, &factory, static_cast<const uint64_t>(time));
			this->_data.clear ();
			this->_data.reserve (GPSVALUEVECTOR_MIN_SIZE);
		}

		GpsPoint (const std::shared_ptr<const F>& factory) : internal::GpsFactoryUserBase<F> (factory) {
			DEBUG_MSG("GpsPoint::%s (%p)\n", __func__, &factory);
			this->_data.clear ();
			this->_data.reserve (GPSVALUEVECTOR_MIN_SIZE);
		}

	private:
		GpsPoint (void) = delete;
		GpsPoint (const GpsPoint&) = delete;                // copy constructor
		GpsPoint (GpsPoint&&) noexcept = delete;            // move constructor
		GpsPoint& operator= (const GpsPoint&) = delete;     // copy assignment
		GpsPoint& operator= (GpsPoint&&) noexcept = delete; // move assignment

	public:
		template <class P = GpsPoint<F>>
		[[nodiscard]] static std::shared_ptr<P> create (const ObjectTime& time, const std::shared_ptr<const typename P::GpsFactory>& factory) {
			return std::shared_ptr<P>(new P (time, factory));
		}

		std::shared_ptr<GpsPoint<F>> getptr (void) {
			return this->shared_from_this ();
		}

		~GpsPoint (void) {
			DEBUG_MSG("GpsPoint::%s ()\n", __func__);
			this->_data.clear ();
		}

		bool hasDataType (const DataType& type) const {
			DEBUG_MSG("GpsPoint::%s (%d)\n", __func__, type);
			for (const GpsValue<DataType>& d : this->_data) {
				if (d.type == type) return true;
			}
			return false;
		}

		template<typename U = std::string, typename std::enable_if<!std::is_same<U, DataType>::value, bool>::type = 0>
		inline bool hasDataType (const std::string& type_str) const {
			DEBUG_MSG("GpsPoint::%s (%s)\n", __func__, type_str.c_str ());
			const DataType& type = this->_factory->getDataType (type_str);
			return this->hasDataType (type);
		}

		const ObjectTime getTime (void) const {
			DEBUG_MSG("GpsPoint::%s ()\n", __func__);
			return this->_time;
		}

		const GpsValue<DataType> getData (const DataType& type) const {
			DEBUG_MSG("GpsPoint::%s (%d)\n", __func__, type);
			for (const GpsValue<DataType>& d : this->_data) {
				if (d.type == type) return d;
			}
			GpsValue<DataType> data;
			data.type = 0;
			return data;
		}

		template<typename U = std::string, typename std::enable_if<!std::is_same<U, DataType>::value, bool>::type = 0>
		inline const GpsValue<DataType> getData (const std::string& type_str) const {
			DEBUG_MSG("GpsPoint::%s (%s)\n", __func__, type_str.c_str ());
			const DataType& type = this->_factory->getDataType (type_str);
			return this->getData (type);
		}

		bool addData (const GpsValue<DataType>& data) {
			DEBUG_MSG("GpsPoint::%s ()\n", __func__);
			for (const GpsValue<DataType>& d : this->_data) {
				if (d.type == data.type) return false;
			}
			this->_data.push_back (data);
			return true;
		}

		template<class T>
		bool addData (const DataType& type, const T& value, bool best_effort = false) {
			DEBUG_MSG("GpsPoint::%s (%d, ..., %d)\n", __func__, type, best_effort);
			for (const GpsValue<DataType>& data : this->_data) {
				if (data.type == type) return false;
			}
			GpsValue<DataType> data;
			if (!this->_factory->setValue (data, type, value, best_effort)) return false;
			this->_data.push_back (data);
			return true;
		}

		template<class T, typename U = std::string, typename std::enable_if<!std::is_same<U, DataType>::value, bool>::type = 0>
		inline bool addData (const std::string& type_str, const T& value, bool best_effort = false) {
			DEBUG_MSG("GpsPoint::%s (%s, ..., %d)\n", __func__, type_str.c_str (), best_effort);
			GpsValue<DataType> data;
			DataType type = this->_factory->getDataType (type_str);
			if(this->_factory->setValue (data, type, value, best_effort))
				return this->addData (data);
			return false;
		}

		inline bool hasLatLon (void) const {
			return this->hasDataType ("LAT") && this->hasDataType ("LON");
		}

		iterator begin(void) {
			return this->_data.begin ();
		}

		const_iterator begin (void) const {
			return this->_data.begin ();
		}

		const_iterator cbegin (void) const {
			return this->_data.cbegin ();
		}

		iterator end (void) {
			return this->_data.end ();
		}

		const_iterator end (void) const {
			return this->_data.end ();
		}

		const_iterator cend (void) const {
			return this->_data.cend ();
		}
	};
}

#endif /* _X_GPSDATA_GPSPOINT_ */
