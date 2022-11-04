#ifndef _GPSDATA_UTILS_GPSDATAFACTORYBASIC_
#define _GPSDATA_UTILS_GPSDATAFACTORYBASIC_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <memory>

#include <gpsdata/utils/Convert.hpp>
#include <gpsdata/GpsDataValueType.hpp>
#include <gpsdata/GpsValue.hpp>

namespace gpsdata::utils {
	class GpsDataFactoryBasic : std::enable_shared_from_this<GpsDataFactoryBasic> {
	public:
		using DataType = uint8_t;
		using ActivityType = uint8_t;

	private:
		const std::map<DataType, std::tuple<const std::string_view, gpsdata::GpsDataValueType, const std::string_view, const std::string_view, const std::string_view>> data_type_map = {
			{ 1, {"LAT", gpsdata::GpsDataValueType::S_LONG_T, "latitude", "latitude of the point", "N"} },
			{ 2, {"LON", gpsdata::GpsDataValueType::S_LONG_T, "longitude", "longitude of the point", "W"} },
			{ 3, {"ALT", gpsdata::GpsDataValueType::S_INT_T, "altitude", "altitude of the point", "m"} },
			{ 4, {"SPEED", gpsdata::GpsDataValueType::FLOAT_T, "speed (km/h)", "current speed in km/h", "km/h"} },
			{ 5, {"POWER", gpsdata::GpsDataValueType::S_INT_T, "power (W)", "current power in W", "W"} },
			{ 6, {"HEARTRATE", gpsdata::GpsDataValueType::S_INT_T, "heartrate (bpm)", "heartrate in bpm", "bpm"} },
			{ 7, {"CADENCE", gpsdata::GpsDataValueType::S_INT_T, "cadence (rpm)", "cadence in rpm", "rpm"} }
		};

		const std::map<ActivityType, std::tuple<const std::string_view, const std::string_view, const std::string_view>> activity_type_map = {
			{ 1, {"CYCLING", "cycling", ""} },
			{ 2, {"RUNNING", "running", ""} },
			{ 3, {"WALKING", "walking", ""} },
			{ 4, {"SWIMMING", "swimming", ""} }
		};

		GpsDataFactoryBasic (void) { }
		GpsDataFactoryBasic (const GpsDataFactoryBasic&) = delete;                // copy constructor
		GpsDataFactoryBasic (GpsDataFactoryBasic&&) noexcept = delete;            // move constructor
		GpsDataFactoryBasic& operator= (const GpsDataFactoryBasic&) = delete;     // copy assignment
		GpsDataFactoryBasic& operator= (GpsDataFactoryBasic&&) noexcept = delete; // move assignment

	public:
		~GpsDataFactoryBasic (void) = default;

		std::shared_ptr<GpsDataFactoryBasic> getptr (void) {
			return this->shared_from_this ();
		}

		DataType getDataType (const std::string_view type_str) const {
			for (const auto& [type, value] : this->data_type_map) {
				if (std::get<0>(value).compare (type_str) == 0)
					return type;
			}
			return 0;
		}

		const std::string getDataTypeString (const DataType& type) const {
			try {
				const auto& value = this->data_type_map.at (type);
				return std::string (std::get<0>(value));
			} catch (std::out_of_range& e) {
				return std::string ();
			}
		}

		const std::vector<std::string> getDataTypesString (void) const {
			std::vector<std::string> result (this->data_type_map.size ());
			for (const auto& [type, value] : this->data_type_map) {
				(void)type;
				result.push_back (std::string (std::get<0>(value)));
			}
			return result;
		}

		const std::vector<std::tuple<const std::string, const std::string, const std::string, const std::string>>
		getDataTypesStringFull (void) const {
			std::vector<std::tuple<const std::string, const std::string, const std::string, const std::string>> result (this->data_type_map.size ());
			for (const auto& [type, value] : this->data_type_map) {
				(void)type;
				std::tuple<const std::string, const std::string, const std::string, const std::string> entry = std::make_tuple(std::string (std::get<0>(value)), std::string (std::get<2>(value)), std::string (std::get<3>(value)), std::string (std::get<4>(value)));
				result.push_back (entry);
			}
			return result;
		}

		const std::string getDataTypeFullName (const DataType& type) const {
			try {
				const auto& value = this->data_type_map.at (type);
				return std::string (std::get<2>(value));
			} catch (std::out_of_range& e) {
				return std::string ();
			}
		}

		const std::string getDataTypeDescription (const DataType& type) const {
			try {
				const auto& value = this->data_type_map.at (type);
				return std::string (std::get<3>(value));
			} catch (std::out_of_range& e) {
				return std::string ();
			}
		}

		const std::string getDataTypeUnit (const DataType& type) const {
			try {
				const auto& value = this->data_type_map.at (type);
				return std::string (std::get<4>(value));
			} catch (std::out_of_range& e) {
				return std::string ();
			}
		}

		gpsdata::GpsDataValueType getDataValueType (const DataType& type) const {
			try {
				const auto& value = this->data_type_map.at (type);
				return std::get<1>(value);
			} catch (std::out_of_range& e) {
				return gpsdata::GpsDataValueType::NONE_T;
			}
		}

		ActivityType getActivityType (const std::string_view type_str) const {
			for (const auto& [type, value] : this->activity_type_map) {
				if (std::get<0>(value).compare (type_str) == 0)
					return type;
			}
			return 0;
		}

		const std::string getActivityTypeString (const ActivityType& type) const {
			try {
				const auto& value = this->activity_type_map.at (type);
				return std::string (std::get<0>(value));
			} catch (std::out_of_range& e) {
				return std::string ();
			}
		}

		const std::vector<std::string> getActivityTypesString (void) const {
			std::vector<std::string> result (this->activity_type_map.size ());
			for (const auto& [type, value] : this->activity_type_map) {
				(void)type;
				result.push_back (std::string (std::get<0>(value)));
			}
			return result;
		}

		template<class T>
		bool getValue (const gpsdata::GpsValue<DataType>& d, T& value, bool best_effort = false) const {
			switch (this->getDataValueType (d.type)) {
				case BOOL_T:
					if (Convert::convertValue<T, bool>(value, d.value._bool, best_effort))
						return true;
					else return false;
					break;
				case S_CHAR_T:
					if (Convert::convertValue<T, int8_t>(value, d.value._s_char, best_effort))
						return true;
					else return false;
					break;
				case U_CHAR_T:
					if (Convert::convertValue<T, uint8_t>(value, d.value._s_char, best_effort))
						return true;
					else return false;
					break;
				case S_SHORT_T:
					if (Convert::convertValue<T, int16_t>(value, d.value._s_short, best_effort))
						return true;
					else return false;
					break;
				case U_SHORT_T:
					if (Convert::convertValue<T, uint16_t>(value, d.value._u_short, best_effort))
						return true;
					else return false;
					break;
				case S_INT_T:
					if (Convert::convertValue<T, int32_t>(value, d.value._s_int, best_effort))
						return true;
					else return false;
					break;
				case U_INT_T:
					if (Convert::convertValue<T, uint32_t>(value, d.value._u_int, best_effort))
						return true;
					else return false;
					break;
				case S_LONG_T:
					if (Convert::convertValue<T, int64_t>(value, d.value._s_long, best_effort))
						return true;
					else return false;
					break;
				case U_LONG_T:
					if (Convert::convertValue<T, uint64_t>(value, d.value._u_long, best_effort))
						return true;
					else return false;
					break;
				case FLOAT_T:
					if (Convert::convertValue<T, float>(value, d.value._float, best_effort))
						return true;
					else return false;
					break;
				case DOUBLE_T:
					if (Convert::convertValue<T, double>(value, d.value._double, best_effort))
						return true;
					else return false;
					break;
				case NONE_T:
				default:
					return false;
			}
			return false;
		}

		template<class T>
		bool setValue (gpsdata::GpsValue<DataType>& d, const DataType& t, const T& v, bool best_effort = false) const {
			switch (this->getDataValueType (t)) {
				case BOOL_T:
					bool b;
					if (Convert::convertValue<bool, T>(b, v, best_effort)) {
						d.type = t;
						d.value._raw = 0;
						d.value._bool = b;
						return true;
					} else return false;
					break;
				case S_CHAR_T:
					int8_t i1;
					if (Convert::convertValue<int8_t, T>(i1, v, best_effort)) {
						d.type = t;
						d.value._raw = 0;
						d.value._s_char = i1;
						return true;
					} else return false;
					break;
				case U_CHAR_T:
					uint8_t i2;
					if (Convert::convertValue<uint8_t, T>(i2, v, best_effort)) {
						d.type = t;
						d.value._raw = 0;
						d.value._u_char = i2;
						return true;
					} else return false;
					break;
				case S_SHORT_T:
					int16_t i3;
					if (Convert::convertValue<int16_t, T>(i3, v, best_effort)) {
						d.type = t;
						d.value._raw = 0;
						d.value._s_short = i3;
						return true;
					} else return false;
					break;
				case U_SHORT_T:
					uint16_t i4;
					if (Convert::convertValue<uint16_t, T>(i4, v, best_effort)) {
						d.type = t;
						d.value._raw = 0;
						d.value._u_short = i4;
						return true;
					} else return false;
					break;
				case S_INT_T:
					int32_t i5;
					if (Convert::convertValue<int32_t, T>(i5, v, best_effort)) {
						d.type = t;
						d.value._raw = 0;
						d.value._s_int = i5;
						return true;
					} else return false;
					break;
				case U_INT_T:
					uint32_t i6;
					if (Convert::convertValue<uint32_t, T>(i6, v, best_effort)) {
						d.type = t;
						d.value._raw = 0;
						d.value._u_int = i6;
						return true;
					} else return false;
					break;
				case S_LONG_T:
					int64_t i7;
					if (Convert::convertValue<int64_t, T>(i7, v, best_effort)) {
						d.type = t;
						d.value._raw = 0;
						d.value._s_long = i7;
						return true;
					} else return false;
					break;
				case U_LONG_T:
					uint64_t i8;
					if (Convert::convertValue<uint64_t, T>(i8, v, best_effort)) {
						d.type = t;
						d.value._raw = 0;
						d.value._u_long = i8;
						return true;
					} else return false;
					break;
				case FLOAT_T:
					float f1;
					if (Convert::convertValue<float, T>(f1, v, best_effort)) {
						d.type = t;
						d.value._raw = 0;
						d.value._float = f1;
						return true;
					} else return false;
					break;
				case DOUBLE_T:
					double f2;
					if (Convert::convertValue<double, T>(f2, v, best_effort)) {
						d.type = t;
						d.value._raw = 0;
						d.value._double = f2;
						return true;
					} else return false;
					break;
				case NONE_T:
				default:
					return false;
			}
			return false;
		}

		[[nodiscard]] static std::shared_ptr<GpsDataFactoryBasic> create (void) {
			return std::shared_ptr<GpsDataFactoryBasic>(new GpsDataFactoryBasic ());
		}
	};
}

#endif /* _GPSDATA_UTILS_GPSFACTORY_ */