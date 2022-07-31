#ifndef _X_GPSDATA_GPSFACTORY_
#define _X_GPSDATA_GPSFACTORY_

#include <cstdint>
#include <string>
#include <vector>
#include <tuple>

#include <gpsdata/GpsDataValueType.hpp>
#include <gpsdata/GpsValue.hpp>

#if __cplusplus >= 202002L
#define SAME_TYPE(type) std::same_as<type>
#define CONVERTIBLE_TYPE(type) std::convertible_to<type>
#define CONCEPT_RETURN
#else
#define SAME_TYPE(type) type
#define CONVERTIBLE_TYPE(type) type
#define CONCEPT_RETURN bool
#endif

namespace gpsdata {
	using ListDataTypes = const std::vector<std::string>;
	using ListDataTypesFull = const std::vector<std::tuple<const std::string, const std::string, const std::string>>;
	using ListActivityTypes = const std::vector<std::string>;

	inline namespace traits {
		template<class T>
		concept CONCEPT_RETURN GpsDataFactory = requires(const T t) {
			typename T::DataType;
			typename T::ActivityType;
		} && requires(const T t, const std::string& s, const typename T::DataType& a1, const typename T::ActivityType& a2) {
			{ t.getDataType (s) } -> SAME_TYPE(typename T::DataType);
			{ t.getDataTypeString (a1) } -> CONVERTIBLE_TYPE(const std::string);
			{ t.getDataTypesString () } -> CONVERTIBLE_TYPE(ListDataTypes);
			{ t.getDataTypesStringFull () } -> CONVERTIBLE_TYPE(ListDataTypesFull);
			{ t.getDataTypeFullName (a1) } -> CONVERTIBLE_TYPE(const std::string);
			{ t.getDataTypeDescription (a1) } -> CONVERTIBLE_TYPE(const std::string);
			{ t.getDataValueType (a1) } -> SAME_TYPE(GpsDataValueType);
			{ t.getActivityType (s) } -> SAME_TYPE(typename T::ActivityType);
			{ t.getActivityTypeString (a2) } -> CONVERTIBLE_TYPE(const std::string);
			{ t.getActivityTypesString () } -> CONVERTIBLE_TYPE(ListActivityTypes);
		} && requires(const T t, const GpsValue<typename T::DataType>& d1, bool b, bool d2) {
			{ t.getValue (d1, b, d2) } -> SAME_TYPE(bool);
		} && requires(const T t, const GpsValue<typename T::DataType>& d1, int8_t b, bool d2) {
			{ t.getValue (d1, b, d2) } -> SAME_TYPE(bool);
		} && requires(const T t, const GpsValue<typename T::DataType>& d1, uint8_t b, bool d2) {
			{ t.getValue (d1, b, d2) } -> SAME_TYPE(bool);
		} && requires(const T t, const GpsValue<typename T::DataType>& d1, int16_t b, bool d2) {
			{ t.getValue (d1, b, d2) } -> SAME_TYPE(bool);
		} && requires(const T t, const GpsValue<typename T::DataType>& d1, uint16_t b, bool d2) {
			{ t.getValue (d1, b, d2) } -> SAME_TYPE(bool);
		} && requires(const T t, const GpsValue<typename T::DataType>& d1, int32_t b, bool d2) {
			{ t.getValue (d1, b, d2) } -> SAME_TYPE(bool);
		} && requires(const T t, const GpsValue<typename T::DataType>& d1, uint32_t b, bool d2) {
			{ t.getValue (d1, b, d2) } -> SAME_TYPE(bool);
		} && requires(const T t, const GpsValue<typename T::DataType>& d1, int64_t b, bool d2) {
			{ t.getValue (d1, b, d2) } -> SAME_TYPE(bool);
		} && requires(const T t, const GpsValue<typename T::DataType>& d1, uint64_t b, bool d2) {
			{ t.getValue (d1, b, d2) } -> SAME_TYPE(bool);
		} && requires(const T t, const GpsValue<typename T::DataType>& d1, float b, bool d2) {
			{ t.getValue (d1, b, d2) } -> SAME_TYPE(bool);
		} && requires(const T t, const GpsValue<typename T::DataType>& d1, double b, bool d2) {
			{ t.getValue (d1, b, d2) } -> SAME_TYPE(bool);
		} && requires(const T t, GpsValue<typename T::DataType>& d1, const typename T::DataType& d2, const bool& a, bool d3) {
			{ t.setValue (d1, d2, a, d3) } -> SAME_TYPE(bool);
		} && requires(const T t, GpsValue<typename T::DataType>& d1, const typename T::DataType& d2, const int8_t& a, bool d3) {
			{ t.setValue (d1, d2, a, d3) } -> SAME_TYPE(bool);
		} && requires(const T t, GpsValue<typename T::DataType>& d1, const typename T::DataType& d2, const uint8_t& a, bool d3) {
			{ t.setValue (d1, d2, a, d3) } -> SAME_TYPE(bool);
		} && requires(const T t, GpsValue<typename T::DataType>& d1, const typename T::DataType& d2, const int16_t& a, bool d3) {
			{ t.setValue (d1, d2, a, d3) } -> SAME_TYPE(bool);
		} && requires(const T t, GpsValue<typename T::DataType>& d1, const typename T::DataType& d2, const uint16_t& a, bool d3) {
			{ t.setValue (d1, d2, a, d3) } -> SAME_TYPE(bool);
		} && requires(const T t, GpsValue<typename T::DataType>& d1, const typename T::DataType& d2, const int32_t& a, bool d3) {
			{ t.setValue (d1, d2, a, d3) } -> SAME_TYPE(bool);
		} && requires(const T t, GpsValue<typename T::DataType>& d1, const typename T::DataType& d2, const uint32_t& a, bool d3) {
			{ t.setValue (d1, d2, a, d3) } -> SAME_TYPE(bool);
		} && requires(const T t, GpsValue<typename T::DataType>& d1, const typename T::DataType& d2, const int64_t& a, bool d3) {
			{ t.setValue (d1, d2, a, d3) } -> SAME_TYPE(bool);
		} && requires(const T t, GpsValue<typename T::DataType>& d1, const typename T::DataType& d2, const uint64_t& a, bool d3) {
			{ t.setValue (d1, d2, a, d3) } -> SAME_TYPE(bool);
		} && requires(const T t, GpsValue<typename T::DataType>& d1, const typename T::DataType& d2, const float& a, bool d3) {
			{ t.setValue (d1, d2, a, d3) } -> SAME_TYPE(bool);
		} && requires(const T t, GpsValue<typename T::DataType>& d1, const typename T::DataType& d2, const double& a, bool d3) {
			{ t.setValue (d1, d2, a, d3) } -> SAME_TYPE(bool);
		};
	}
}

#endif /* _X_GPSDATA_GPSFACTORY_ */