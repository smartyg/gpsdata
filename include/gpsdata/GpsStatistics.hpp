#ifndef _X_GPSDATA_GPSSTATISTICS_
#define _X_GPSDATA_GPSSTATISTICS_

#include <string>
#include <type_traits>
#include <vector>
#include <memory>

#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/GpsValue.hpp>
#include <gpsdata/GpsFactoryUserBase.hpp>

namespace bitsery {
	class Access;
}

namespace gpsdata {
	template<GpsFactoryTrait F>
	class GpsStatistics : virtual public internal::GpsFactoryUserBase<F> {
		friend class bitsery::Access;

		template<typename B, GpsStatisticsTrait S>
		friend void serialize (B&, S&) requires(std::is_base_of<GpsStatistics<typename S::GpsFactory>, S>::value);

	public:
		using DataType = typename F::DataType;
		using Container = typename std::vector<GpsValue<DataType>>;

	protected:
		Container _statistics;

		GpsStatistics (const std::shared_ptr<const F>& factory) : internal::GpsFactoryUserBase<F> (factory) {
			DEBUG_MSG("GpsStatistics::%s (%p)\n", __func__, &factory);
			this->_statistics.clear ();
			this->_statistics.reserve (GPSVALUEVECTOR_MIN_SIZE);
		}

	private:
		GpsStatistics (void) = default;
		GpsStatistics (const GpsStatistics&) = delete;                // copy constructor
		GpsStatistics (GpsStatistics&&) noexcept = delete;            // move constructor
		GpsStatistics& operator= (const GpsStatistics&) = delete;     // copy assignment
		GpsStatistics& operator= (GpsStatistics&&) noexcept = delete; // move assignment

	public:
		~GpsStatistics (void) {
			DEBUG_MSG("GpsStatistics::%s ()\n", __func__);
			this->_statistics.clear ();
		}

		/* GPS Statistics */
		bool addStatistic (const GpsValue<DataType>& value) {
			DEBUG_MSG("GpsStatistics::%s (...)\n", __func__);
			if (this->hasStatistic (value.type)) return false;
			this->_statistics.push_back (value);
			return true;
		}

		template<class T>
		inline bool addStatistic (const DataType& type, const T& value) {
			DEBUG_MSG("GpsStatistics::%s (%d, ...)\n", __func__, type);
			GpsValue<DataType> data;
			if(this->_factory->setValue (data, type, value))
				return this->addStatistic (data);
			return false;
		}

		template<class T, typename U = std::string, typename std::enable_if<!std::is_same<U, DataType>::value, bool>::type = 0>
		inline bool addStatistic (const std::string& type, const T& value) {
			DEBUG_MSG("GpsStatistics::%s (%s, ...)\n", __func__, type.c_str ());
			GpsValue<DataType> data;
			if(this->_factory->setValue (data, type, value))
				return this->addStatistic (data);
			return false;
		}

		bool setStatistic (const GpsValue<DataType>& value) {
			DEBUG_MSG("GpsStatistics::%s (..)\n", __func__);
			if (!this->hasStatistic (value.type))
				this->_statistics.push_back (value);
			else {
				for (GpsValue<DataType>& v1 : this->_statistics) {
					if (v1.type == value.type)
						v1.value._raw = value.value._raw;
				}
			}
			return true;
		}

		template<class T>
		inline bool setStatistic (const DataType& type, const T& value) {
			DEBUG_MSG("GpsStatistics::%s (%d, ...)\n", __func__, type);
			GpsValue<DataType> data;
			if(this->_factory->setValue (data, type, value))
				return this->setStatistic (data);
			return false;
		}

		template<class T, typename U = std::string, typename std::enable_if<!std::is_same<U, DataType>::value, bool>::type = 0>
		inline bool setStatistic (const std::string& type, const T& value) {
			DEBUG_MSG("GpsStatistics::%s (%s, ...)\n", __func__, type.c_str ());
			GpsValue<DataType> data;
			if(this->_factory->setValue (data, type, value))
				return this->setStatistic (data);
			return false;
		}

		bool hasStatistic (const DataType& type) const {
			DEBUG_MSG("GpsStatistics::%s (%d)\n", __func__, type);
			for (const GpsValue<DataType>& v1 : this->_statistics) {
				if (v1.type == type)
					return true;
			}
			return false;
		}

		//template<typename std::enable_if<!std::is_same<std::string, DataType>::value, bool>::type>
		template<typename T = std::string, typename std::enable_if<!std::is_same<T, DataType>::value, bool>::type = 0>
		inline bool hasStatistic (const std::string& type_str) const {
			DEBUG_MSG("GpsStatistics::%s (%s)\n", __func__, type_str.c_str ());
			const DataType type = this->_factory->getDataType (type_str);
			return this->hasStatistic (type);
		}

		const GpsValue<DataType> getStatistic (const DataType& type) const {
			DEBUG_MSG("GpsStatistics::%s (%d)\n", __func__, type);
			for (const GpsValue<DataType>& v1 : this->_statistics) {
				if (v1.type == type)
					return v1;
			}
			const GpsValue<DataType>& v = { 0, 0 };
			return v;
		}

		//template<typename std::enable_if<!std::is_same<std::string, DataType>::value, bool>::type>
		template<typename T = std::string, typename std::enable_if<!std::is_same<T, DataType>::value, bool>::type = 0>
		inline const GpsValue<DataType> getStatistic (const std::string& type_str) const {
			DEBUG_MSG("GpsStatistics::%s (%s)\n", __func__, type_str.c_str ());
			const DataType type = this->_factory->getDataType (type_str);
			return this->getStatistic (type);
		}

		const Container getStatistics (void) const {
			DEBUG_MSG("GpsStatistics::%s ()\n", __func__);
			return this->_statistics;
		}
	};
}

#endif /* _X_GPSDATA_GPSSTATISTICS_ */
