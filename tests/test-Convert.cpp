#include <gtest/gtest.h>

#include "config.h"

#include <array>
#include <string>
#include <string_view>

#include "gpsdata/utils/Convert.hpp"

using gpsdata::utils::Convert;

/*
 * types to test (conversion to and from):
 * bool
 * unisgned char
 * signed char
 * unsigned int
 * signed int
 * uint64_t
 * int64_t
 * float
 * double
 * long double
 * std::string
 */

/* test function, this function will be run for each to and from type. */
template<typename FROM, typename TO, std::size_t N>
void RunTest (const std::array<FROM, N>& in_array, const std::array<bool, N>& res_array, const std::array<TO, N>& test_array, const bool& best_effort) {
	for (std::size_t i = 0; i < N; ++i) {
		FROM t_in = in_array[i];
		TO t_out;
		std::string t_in_str;
		if constexpr (std::is_same_v<FROM, std::string>) {
			t_in_str = t_in;
		} else if constexpr (std::is_same_v<FROM, std::string_view>) {
			t_in_str = std::string (t_in);
		} else {
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wsign-promo"
			t_in_str = std::to_string (t_in);
			#pragma GCC diagnostic pop
		}

		SCOPED_TRACE ("testing value: " + t_in_str + " (best effort: " + std::to_string (static_cast<int>(best_effort)) + ")");

		bool res = gpsdata::utils::Convert::convertValue (t_out, t_in, best_effort);

		if (res_array[i] == true) {
			std::string t_test_str;
			if constexpr (std::is_same_v<TO, std::string>) {
				t_test_str = test_array[i];
			} else if constexpr (std::is_same_v<TO, std::string_view>) {
				t_test_str = std::string (test_array[i]);
			} else {
				#pragma GCC diagnostic push
				#pragma GCC diagnostic ignored "-Wsign-promo"
				t_test_str = std::to_string (test_array[i]);
				#pragma GCC diagnostic pop
			}

			EXPECT_TRUE (res) << "Expect true while converting '" << t_in_str << "' to: " << typeid (t_out).name () << " (best effort: " << std::to_string (static_cast<int>(best_effort)) << ")";
			if constexpr (std::is_floating_point_v<TO> && std::is_same_v<std::remove_cv_t<std::remove_reference_t<TO>>, float>)
				EXPECT_FLOAT_EQ (t_out, test_array[i]) << "Expect result of '" << t_test_str << "' while converting '" << t_in_str << "' to: " << typeid (t_out).name () << " (best effort: " << std::to_string (static_cast<int>(best_effort)) << ")";
			else if constexpr (std::is_floating_point_v<TO>)
				EXPECT_DOUBLE_EQ (t_out, test_array[i]) << "Expect result of '" << t_test_str << "' while converting '" << t_in_str << "' to: " << typeid (t_out).name () << " (best effort: " << std::to_string (static_cast<int>(best_effort)) << ")";
			else if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<TO>>, std::string>)
				EXPECT_STREQ (t_out.c_str (), test_array[i].c_str ()) << "Expect result of '" << t_test_str << "' while converting '" << t_in_str << "' to: " << typeid (t_out).name () << " (best effort: " << std::to_string (static_cast<int>(best_effort)) << ")";
			else if (std::is_integral_v<TO>)
				EXPECT_EQ (t_out, test_array[i]) << "Expect result of '" << t_test_str << "' while converting '" << t_in_str << "' to: " << typeid (t_out).name () << " (best effort: " << std::to_string (static_cast<int>(best_effort)) << ")";
			else
				static_assert (true);


		} else {
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wsign-promo"
			EXPECT_FALSE (res) << "Expect false while converting '" << t_in_str << "' to: " << typeid (t_out).name () << " (best effort: " << std::to_string (static_cast<int>(best_effort)) << ")";
			#pragma GCC diagnostic pop
		}
	}
}

TEST(ConvertTest, IsInRange1Test)
{
	bool res;

	res = Convert::is_in_range<bool, bool>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<bool, char>();
	EXPECT_EQ (res, false);
	res = Convert::is_in_range<bool, signed char>();
	EXPECT_EQ (res, false);
	res = Convert::is_in_range<bool, int>();
	EXPECT_EQ (res, false);

	res = Convert::is_in_range<char, bool>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<char, char>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<char, int>();
	EXPECT_EQ (res, false);
	res = Convert::is_in_range<char, float>();
	EXPECT_EQ (res, false);

	res = Convert::is_in_range<signed char, bool>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<signed char, signed char>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<signed char, unsigned char>();
	EXPECT_EQ (res, false);
	res = Convert::is_in_range<signed char, int>();
	EXPECT_EQ (res, false);
	res = Convert::is_in_range<signed char, float>();
	EXPECT_EQ (res, false);

	res = Convert::is_in_range<unsigned char, bool>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<unsigned char, unsigned char>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<unsigned char, int>();
	EXPECT_EQ (res, false);
	res = Convert::is_in_range<unsigned char, float>();
	EXPECT_EQ (res, false);

	res = Convert::is_in_range<signed int, bool>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<signed int, unsigned char>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<signed int, signed int>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<signed int, unsigned int>();
	EXPECT_EQ (res, false);
	res = Convert::is_in_range<signed int, float>();
	EXPECT_EQ (res, false);

	res = Convert::is_in_range<unsigned int, bool>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<unsigned int, unsigned char>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<unsigned int, unsigned int>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<unsigned int, float>();
	EXPECT_EQ (res, false);

	res = Convert::is_in_range<int64_t, bool>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<int64_t, unsigned char>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<int64_t, signed int>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<int64_t, int64_t>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<int64_t, uint64_t>();
	EXPECT_EQ (res, false);
	res = Convert::is_in_range<int64_t, float>();
	EXPECT_EQ (res, false);

	res = Convert::is_in_range<uint64_t, bool>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<uint64_t, unsigned char>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<uint64_t, unsigned int>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<uint64_t, int64_t>();
	EXPECT_EQ (res, false);
	res = Convert::is_in_range<uint64_t, uint64_t>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<uint64_t, float>();
	EXPECT_EQ (res, false);

	res = Convert::is_in_range<float, bool>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<float, unsigned char>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<float, unsigned int>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<float, int64_t>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<float, uint64_t>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<float, float>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<float, double>();
	EXPECT_EQ (res, false);
	res = Convert::is_in_range<float, long double>();
	EXPECT_EQ (res, false);

	res = Convert::is_in_range<double, bool>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<double, unsigned char>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<double, unsigned int>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<double, int64_t>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<double, uint64_t>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<double, float>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<double, double>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<double, long double>();
	EXPECT_EQ (res, false);

	res = Convert::is_in_range<long double, bool>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<long double, unsigned char>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<long double, unsigned int>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<long double, int64_t>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<long double, uint64_t>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<long double, float>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<long double, double>();
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<long double, long double>();
	EXPECT_EQ (res, true);
}

TEST(ConvertTest, IsInRange2Test)
{
	bool res;

	res = Convert::is_in_range<bool, char>(0);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<bool, signed char>(1);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<bool, float>(1.0);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<bool, int>(-1);
	EXPECT_EQ (res, false);

	res = Convert::is_in_range<char, int>(0);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<char, int>(127);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<char, int>(-129);
	EXPECT_EQ (res, false);
	res = Convert::is_in_range<char, float>(63.0);
	EXPECT_EQ (res, true);

	res = Convert::is_in_range<signed int, bool>(true);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<signed int, uint64_t>(0);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<signed int, int64_t>(-10);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<signed int, uint64_t>(127);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<signed int, uint64_t>(4294967296);
	EXPECT_EQ (res, false);
	res = Convert::is_in_range<signed int, float>(63.0);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<signed int, float>(-63.0);
	EXPECT_EQ (res, true);

	res = Convert::is_in_range<float, uint64_t>(18446744073709551615ul);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<float, double>(0.001);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<float, double>(-10.5);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<float, double>(127.2);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<float, double>(4294967296);
	EXPECT_EQ (res, true);
	res = Convert::is_in_range<float, double>(DBL_MAX);
	EXPECT_EQ (res, false);
}

TEST(ConvertTest, BoolTest)
{
	// input array
	constexpr const auto t_in_array = std::to_array<bool>({true, false});

	// expected result of the conversion of input array
	// for bool type, these are all true
	constexpr const auto t_res_array = std::to_array<bool>({true, true});

	SCOPED_TRACE ("conversion from bool");

	{
		// expected output for conversion to bool (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<bool>({true, false});
		SCOPED_TRACE ("conversion to bool");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned char>({1, 0});
		SCOPED_TRACE ("conversion to unsigned char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array, t_out_array, false);
	}

	{
		// expected output for conversion to signed char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed char>({1, 0});
		SCOPED_TRACE ("conversion to signed char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned int>({1, 0});
		SCOPED_TRACE ("conversion to unsigned int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array, t_out_array, false);
	}

	{
		// expected output for conversion to signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed int>({1, 0});
		SCOPED_TRACE ("conversion to signed int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<uint64_t>({1, 0});
		SCOPED_TRACE ("conversion to uint64_t");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<int64_t>({1, 0});
		SCOPED_TRACE ("conversion to int64_t");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array, t_out_array, false);
	}

	{
		// expected output for conversion to float (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<float>({1.0, 0.0});
		SCOPED_TRACE ("conversion to float");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array, t_out_array, false);
	}

	{
		// expected output for conversion to double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<double>({1.0, 0.0});
		SCOPED_TRACE ("conversion to double");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<long double>({1.0, 0.0});
		SCOPED_TRACE ("conversion to long double");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		const auto t_out_array = std::to_array<std::string>({"1", "0"});
		SCOPED_TRACE ("conversion to string");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array, t_out_array, false);
	}
}

TEST(ConvertTest, UCharTest)
{
	// input array
	constexpr const auto t_in_array = std::to_array<unsigned char>({std::numeric_limits<unsigned char>::min (), 1, std::numeric_limits<unsigned char>::max ()});

	// expected result of the conversion of input array
	// for bool type, these are all true
	constexpr const auto t_res_array_true = std::to_array<bool>({true, true, true});
	constexpr const auto t_res_array_false = std::to_array<bool>({false, false, false});

	SCOPED_TRACE ("conversion from unsigned char");

	{
		// expected output for conversion to bool (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<bool>({false, true, false});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false});
		SCOPED_TRACE ("conversion to bool");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned char>({std::numeric_limits<unsigned char>::min (), 1, std::numeric_limits<unsigned char>::max ()});
		SCOPED_TRACE ("conversion to unsigned char");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to signed char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed char>({std::numeric_limits<unsigned char>::min (), 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false});
		SCOPED_TRACE ("conversion to signed char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned int>({std::numeric_limits<unsigned char>::min (), 1, std::numeric_limits<unsigned char>::max ()});
		SCOPED_TRACE ("conversion to unsigned int");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed int>({std::numeric_limits<unsigned char>::min (), 1, std::numeric_limits<unsigned char>::max ()});
		SCOPED_TRACE ("conversion to signed int");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<uint64_t>({std::numeric_limits<unsigned char>::min (), 1, std::numeric_limits<unsigned char>::max ()});
		SCOPED_TRACE ("conversion to uint64_t");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<int64_t>({std::numeric_limits<unsigned char>::min (), 1, std::numeric_limits<unsigned char>::max ()});
		SCOPED_TRACE ("conversion to int64_t");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to float (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<float>({std::numeric_limits<unsigned char>::min (), 1, std::numeric_limits<unsigned char>::max ()});
		SCOPED_TRACE ("conversion to float");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<double>({std::numeric_limits<unsigned char>::min (), 1, std::numeric_limits<unsigned char>::max ()});
		SCOPED_TRACE ("conversion to double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<long double>({std::numeric_limits<unsigned char>::min (), 1, std::numeric_limits<unsigned char>::max ()});
		SCOPED_TRACE ("conversion to long double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wsign-promo"
		const auto t_out_array = std::to_array<std::string>({std::to_string (std::numeric_limits<unsigned char>::min ()), "1", std::to_string (std::numeric_limits<unsigned char>::max ())});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to string");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}
}

TEST(ConvertTest, SCharTest)
{
	// input array
	constexpr const auto t_in_array = std::to_array<signed char>({std::numeric_limits<signed char>::min (), -1, 0, 1, std::numeric_limits<signed char>::max ()});

	// expected result of the conversion of input array
	// for bool type, these are all true
	constexpr const auto t_res_array_true = std::to_array<bool>({true, true, true, true, true});
	constexpr const auto t_res_array_false = std::to_array<bool>({false, false, false, false, false});

	SCOPED_TRACE ("conversion from signed char");

	{
		// expected output for conversion to bool (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<bool>({false, false, false, true, false});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, false});
		SCOPED_TRACE ("conversion to bool");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned char>({0, 0, 0, 1, std::numeric_limits<signed char>::max ()});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, true});
		SCOPED_TRACE ("conversion to unsigned char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed char>({std::numeric_limits<signed char>::min (), -1, 0, 1, std::numeric_limits<signed char>::max ()});
		SCOPED_TRACE ("conversion to signed char");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned int>({0, 0, 0, 1, std::numeric_limits<signed char>::max ()});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, true});
		SCOPED_TRACE ("conversion to unsigned int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed int>({std::numeric_limits<signed char>::min (), -1, 0, 1, std::numeric_limits<signed char>::max ()});
		SCOPED_TRACE ("conversion to signed int");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<uint64_t>({0, 0, 0, 1, std::numeric_limits<signed char>::max ()});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, true});
		SCOPED_TRACE ("conversion to uint64_t");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<int64_t>({std::numeric_limits<signed char>::min (), -1, 0, 1, std::numeric_limits<signed char>::max ()});
		SCOPED_TRACE ("conversion to int64_t");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to float (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<float>({std::numeric_limits<signed char>::min (), -1, 0, 1, std::numeric_limits<signed char>::max ()});
		SCOPED_TRACE ("conversion to float");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<double>({std::numeric_limits<signed char>::min (), -1, 0, 1, std::numeric_limits<signed char>::max ()});
		SCOPED_TRACE ("conversion to double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<long double>({std::numeric_limits<signed char>::min (), -1, 0, 1, std::numeric_limits<signed char>::max ()});
		SCOPED_TRACE ("conversion to long double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wsign-promo"
		const auto t_out_array = std::to_array<std::string>({std::to_string (std::numeric_limits<signed char>::min ()), "-1", "0", "1", std::to_string (std::numeric_limits<signed char>::max ())});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to string");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}
}

TEST(ConvertTest, UIntTest)
{
	// input array
	constexpr const auto t_in_array = std::to_array<unsigned int>({std::numeric_limits<unsigned int>::min (), 1, std::numeric_limits<unsigned int>::max ()});

	// expected result of the conversion of input array
	// for bool type, these are all true
	constexpr const auto t_res_array_true = std::to_array<bool>({true, true, true});
	constexpr const auto t_res_array_false = std::to_array<bool>({false, false, false});

	SCOPED_TRACE ("conversion from unsigned int");

	{
		// expected output for conversion to bool (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<bool>({false, true, false});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false});
		SCOPED_TRACE ("conversion to bool");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned char>({0, 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false});
		SCOPED_TRACE ("conversion to unsigned char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed char>({0, 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false});
		SCOPED_TRACE ("conversion to signed char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned int>({std::numeric_limits<unsigned int>::min (), 1, std::numeric_limits<unsigned int>::max ()});
		SCOPED_TRACE ("conversion to unsigned int");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed int>({std::numeric_limits<unsigned int>::min (), 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false});
		SCOPED_TRACE ("conversion to signed int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<uint64_t>({std::numeric_limits<unsigned int>::min (), 1, std::numeric_limits<unsigned int>::max ()});
		SCOPED_TRACE ("conversion to uint64_t");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<int64_t>({std::numeric_limits<unsigned int>::min (), 1, std::numeric_limits<unsigned int>::max ()});
		SCOPED_TRACE ("conversion to int64_t");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to float (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<float>({0, 1, 4294967295});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to float");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<double>({0, 1, 4294967295});
		SCOPED_TRACE ("conversion to double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<long double>({0, 1, 4294967295});
		SCOPED_TRACE ("conversion to long double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wsign-promo"
		const auto t_out_array = std::to_array<std::string>({std::to_string (std::numeric_limits<unsigned int>::min ()), "1", std::to_string (std::numeric_limits<unsigned int>::max ())});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to string");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}
}

TEST(ConvertTest, SIntTest)
{
	// input array
	constexpr const auto t_in_array = std::to_array<signed int>({std::numeric_limits<signed int>::min (), -1, 0, 1, std::numeric_limits<signed int>::max ()});

	// expected result of the conversion of input array
	// for bool type, these are all true
	constexpr const auto t_res_array_true = std::to_array<bool>({true, true, true, true, true});
	constexpr const auto t_res_array_false = std::to_array<bool>({false, false, false, false, false});

	SCOPED_TRACE ("conversion from signed int");

	{
		// expected output for conversion to bool (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<bool>({false, false, false, true, false});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, false});
		SCOPED_TRACE ("conversion to bool");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned char>({0, 0, 0, 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, false});
		SCOPED_TRACE ("conversion to unsigned char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed char>({0, -1, 0, 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, true, true, true, false});
		SCOPED_TRACE ("conversion to signed char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned int>({0, 0, 0, 1, std::numeric_limits<signed int>::max ()});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, true});
		SCOPED_TRACE ("conversion to unsigned int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed int>({std::numeric_limits<signed int>::min (), -1, 0, 1, std::numeric_limits<signed int>::max ()});
		SCOPED_TRACE ("conversion to signed int");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<uint64_t>({0, 0, 0, 1, std::numeric_limits<signed int>::max ()});
		constexpr const auto t_res_array_uint64_1 = std::to_array<bool>({false, false, true, true, true});
		SCOPED_TRACE ("conversion to uint64_t");
		RunTest (t_in_array, t_res_array_uint64_1, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<int64_t>({std::numeric_limits<signed int>::min (), -1, 0, 1, std::numeric_limits<signed int>::max ()});
		SCOPED_TRACE ("conversion to int64_t");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to float (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<float>({-2147483648, -1, 0, 1, 2147483647});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to float");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<double>({-2147483648, -1, 0, 1, 2147483647});
		SCOPED_TRACE ("conversion to double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<long double>({-2147483648, -1, 0, 1, 2147483647});
		SCOPED_TRACE ("conversion to long double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wsign-promo"
		const auto t_out_array = std::to_array<std::string>({std::to_string (std::numeric_limits<signed int>::min ()), "-1", "0", "1", std::to_string (std::numeric_limits<signed int>::max ())});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to string");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}
}

TEST(ConvertTest, UInt64Test)
{
	// input array
	constexpr const auto t_in_array = std::to_array<uint64_t>({std::numeric_limits<uint64_t>::min (), 1, std::numeric_limits<uint64_t>::max ()});

	// expected result of the conversion of input array
	// for bool type, these are all true
	constexpr const auto t_res_array_true = std::to_array<bool>({true, true, true});
	constexpr const auto t_res_array_false = std::to_array<bool>({false, false, false});

	SCOPED_TRACE ("conversion from 64-bits unsigned int");

	{
		// expected output for conversion to bool (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<bool>({false, true, false});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false});
		SCOPED_TRACE ("conversion to bool");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned char>({0, 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false});
		SCOPED_TRACE ("conversion to unsigned char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed char>({0, 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false});
		SCOPED_TRACE ("conversion to signed char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned int>({0, 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false});
		SCOPED_TRACE ("conversion to unsigned int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed int>({0, 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false});
		SCOPED_TRACE ("conversion to signed int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<uint64_t>({std::numeric_limits<uint64_t>::min (), 1, std::numeric_limits<uint64_t>::max ()});
		SCOPED_TRACE ("conversion to uint64_t");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<int64_t>({std::numeric_limits<uint64_t>::min (), 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false});
		SCOPED_TRACE ("conversion to int64_t");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to float (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<float>({std::numeric_limits<uint64_t>::min (), 1, std::numeric_limits<uint64_t>::max ()});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to float");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<double>({std::numeric_limits<uint64_t>::min (), 1, std::numeric_limits<uint64_t>::max ()});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<long double>({std::numeric_limits<uint64_t>::min (), 1, std::numeric_limits<uint64_t>::max ()});
		SCOPED_TRACE ("conversion to long double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wsign-promo"
		const auto t_out_array = std::to_array<std::string>({std::to_string (std::numeric_limits<uint64_t>::min ()), "1", std::to_string (std::numeric_limits<uint64_t>::max ())});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to string");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}
}

TEST(ConvertTest, SInt64Test)
{
	// input array
	constexpr const auto t_in_array = std::to_array<int64_t>({std::numeric_limits<int64_t>::min (), -1, 0, 1, std::numeric_limits<int64_t>::max ()});

	// expected result of the conversion of input array
	// for bool type, these are all true
	constexpr const auto t_res_array_true = std::to_array<bool>({true, true, true, true, true});
	constexpr const auto t_res_array_false = std::to_array<bool>({false, false, false, false, false});

	SCOPED_TRACE ("conversion from 64-bits signed int");

	{
		// expected output for conversion to bool (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<bool>({false, false, false, true, false});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, false});
		SCOPED_TRACE ("conversion to bool");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned char>({0, 0, 0, 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, false});
		SCOPED_TRACE ("conversion to unsigned char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed char>({0, -1, 0, 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, true, true, true, false});
		SCOPED_TRACE ("conversion to signed char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned int>({0, 0, 0, 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, false});
		SCOPED_TRACE ("conversion to unsigned int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed int>({0, -1, 0, 1, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, true, true, true, false});
		SCOPED_TRACE ("conversion to signed int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<uint64_t>({0, 0, 0, 1, std::numeric_limits<int64_t>::max ()});
		constexpr const auto t_res_array_uint64_1 = std::to_array<bool>({false, false, true, true, true});
		SCOPED_TRACE ("conversion to uint64_t");
		RunTest (t_in_array, t_res_array_uint64_1, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<int64_t>({std::numeric_limits<int64_t>::min (), -1, 0, 1, std::numeric_limits<int64_t>::max ()});
		SCOPED_TRACE ("conversion to int64_t");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to float (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<float>({std::numeric_limits<int64_t>::min (), -1, 0, 1, std::numeric_limits<int64_t>::max ()});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to float");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<double>({std::numeric_limits<int64_t>::min (), -1, 0, 1, std::numeric_limits<int64_t>::max ()});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<long double>({std::numeric_limits<int64_t>::min (), -1, 0, 1, std::numeric_limits<int64_t>::max ()});
		SCOPED_TRACE ("conversion to long double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wsign-promo"
		const auto t_out_array = std::to_array<std::string>({std::to_string (std::numeric_limits<int64_t>::min ()), "-1", "0", "1", std::to_string (std::numeric_limits<int64_t>::max ())});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to string");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}
}

TEST(ConvertTest, FloatTest)
{
	// input array
	constexpr const auto t_in_array = std::to_array<float>({std::numeric_limits<float>::lowest (), -10.5, -1, 0, 1, 20.4, std::numeric_limits<float>::max ()});

	// expected result of the conversion of input array
	// for bool type, these are all true
	constexpr const auto t_res_array_true = std::to_array<bool>({true, true, true, true, true, true, true});
	constexpr const auto t_res_array_false = std::to_array<bool>({false, false, false, false, false, false, false});

	SCOPED_TRACE ("conversion from float");

	{
		// expected output for conversion to bool (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<bool>({false, false, false, false, true, false, false});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, true, true, false, false});
		SCOPED_TRACE ("conversion to bool");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned char>({0, 0, 0, 0, 1, 20, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, true, true, true, false});
		SCOPED_TRACE ("conversion to unsigned char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed char>({0, -10, -1, 0, 1, 20, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, true, true, true, true, true, false});
		SCOPED_TRACE ("conversion to signed char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned int>({0, 0, 0, 0, 1, 20, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, true, true, true, false});
		SCOPED_TRACE ("conversion to unsigned int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed int>({0, -10, -1, 0, 1, 20, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, true, true, true, true, true, false});
		SCOPED_TRACE ("conversion to signed int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<uint64_t>({0, 0, 0, 0, 1, 20, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, true, true, true, false});
		SCOPED_TRACE ("conversion to uint64_t");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<int64_t>({0, -10, -1, 0, 1, 20, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, true, true, true, true, true, false});
		SCOPED_TRACE ("conversion to int64_t");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to float (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<float>({std::numeric_limits<float>::lowest (), -10.5f, -1, 0, 1, 20.4f, std::numeric_limits<float>::max ()});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to float");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<double>({std::numeric_limits<float>::lowest (), -10.5f, -1, 0, 1, 20.4f, std::numeric_limits<float>::max ()});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<long double>({std::numeric_limits<float>::lowest (), -10.5f, -1, 0, 1, 20.4f, std::numeric_limits<float>::max ()});
		SCOPED_TRACE ("conversion to long double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wsign-promo"
		const auto t_out_array = std::to_array<std::string>({std::to_string (std::numeric_limits<float>::lowest ()), "-10.500000", "-1.000000", "0.000000", "1.000000", "20.400000", std::to_string (std::numeric_limits<float>::max ())});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to string");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}
}

TEST(ConvertTest, DoubleTest)
{
	// input array
	constexpr const auto t_in_array = std::to_array<double>({std::numeric_limits<double>::lowest (), std::numeric_limits<float>::lowest (), -10.5, -1, 0, 1, 20.4, std::numeric_limits<float>::max (), std::numeric_limits<double>::max ()});

	// expected result of the conversion of input array
	// for bool type, these are all true
	constexpr const auto t_res_array_true = std::to_array<bool>({true, true, true, true, true, true, true, true, true});
	constexpr const auto t_res_array_false = std::to_array<bool>({false, false, false, false, false, false, false, false, false});

	SCOPED_TRACE ("conversion from double");

	{
		// expected output for conversion to bool (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<bool>({false, false, false, false, false, true, false, false, false});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, true, true, false, false, false});
		SCOPED_TRACE ("conversion to bool");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned char>({0, 0, 0, 0, 0, 1, 20, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, true, true, true, false, false});
		SCOPED_TRACE ("conversion to unsigned char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed char>({0, 0, -10, -1, 0, 1, 20, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, true, true, true, false, false});
		SCOPED_TRACE ("conversion to signed char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned int>({0, 0, 0, 0, 0, 1, 20, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, true, true, true, false, false});
		SCOPED_TRACE ("conversion to unsigned int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed int>({0, 0, -10, -1, 0, 1, 20, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, true, true, true, false, false});
		SCOPED_TRACE ("conversion to signed int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<uint64_t>({0, 0, 0, 0, 0, 1, 20, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, true, true, true, false, false});
		SCOPED_TRACE ("conversion to uint64_t");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<int64_t>({0, 0, -10, -1, 0, 1, 20, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, true, true, true, false, false});
		SCOPED_TRACE ("conversion to int64_t");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to float (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<float>({0, std::numeric_limits<float>::lowest (), -10.5f, -1, 0, 1, 20.4f, std::numeric_limits<float>::max (), 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, true, true, true, true, true, true, true, false});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to float");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<double>({std::numeric_limits<double>::lowest (), std::numeric_limits<float>::lowest (), -10.5, -1, 0, 1, 20.4, std::numeric_limits<float>::max (), std::numeric_limits<double>::max ()});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<long double>({std::numeric_limits<double>::lowest (), std::numeric_limits<float>::lowest (), -10.5, -1, 0, 1, 20.4, std::numeric_limits<float>::max (), std::numeric_limits<double>::max ()});
		SCOPED_TRACE ("conversion to long double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wsign-promo"
		const auto t_out_array = std::to_array<std::string>({std::to_string (std::numeric_limits<double>::lowest ()), std::to_string (std::numeric_limits<float>::lowest ()), "-10.500000", "-1.000000", "0.000000", "1.000000", "20.400000", std::to_string (std::numeric_limits<float>::max ()), std::to_string (std::numeric_limits<double>::max ())});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to string");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}
}

TEST(ConvertTest, LDoubleTest)
{
	// input array
	constexpr const auto t_in_array = std::to_array<long double>({std::numeric_limits<long double>::lowest (), std::numeric_limits<float>::lowest (), -10.5, -1, 0, 1, 20.4, std::numeric_limits<float>::max (), std::numeric_limits<long double>::max ()});

	// expected result of the conversion of input array
	// for bool type, these are all true
	constexpr const auto t_res_array_true = std::to_array<bool>({true, true, true, true, true, true, true, true, true});
	constexpr const auto t_res_array_false = std::to_array<bool>({false, false, false, false, false, false, false, false, false});

	SCOPED_TRACE ("conversion from long double");

	{
		// expected output for conversion to bool (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<bool>({false, false, false, false, false, true, false, false, false});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, true, true, false, false, false});
		SCOPED_TRACE ("conversion to bool");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned char>({0, 0, 0, 0, 0, 1, 20, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, true, true, true, false, false});
		SCOPED_TRACE ("conversion to unsigned char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed char>({0, 0, -10, -1, 0, 1, 20, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, true, true, true, false, false});
		SCOPED_TRACE ("conversion to signed char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned int>({0, 0, 0, 0, 0, 1, 20, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, true, true, true, false, false});
		SCOPED_TRACE ("conversion to unsigned int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed int>({0, 0, -10, -1, 0, 1, 20, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, true, true, true, false, false});
		SCOPED_TRACE ("conversion to signed int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<uint64_t>({0, 0, 0, 0, 0, 1, 20, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, true, true, true, false, false});
		SCOPED_TRACE ("conversion to uint64_t");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<int64_t>({0, 0, -10, -1, 0, 1, 20, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, true, true, true, true, true, false, false});
		SCOPED_TRACE ("conversion to int64_t");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to float (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<float>({0, std::numeric_limits<float>::lowest (), -10.5f, -1, 0, 1, 20.4f, std::numeric_limits<float>::max (), 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, true, true, true, true, true, true, true, false});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to float");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<double>({std::numeric_limits<long double>::lowest (), std::numeric_limits<float>::lowest (), -10.5, -1, 0, 1, 20.4, std::numeric_limits<float>::max (), std::numeric_limits<long double>::max ()});
		constexpr const auto t_res_array = std::to_array<bool>({false, true, true, true, true, true, true, true, false});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to double");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<long double>({std::numeric_limits<long double>::lowest (), std::numeric_limits<float>::lowest (), -10.5, -1, 0, 1, 20.4, std::numeric_limits<float>::max (), std::numeric_limits<long double>::max ()});
		SCOPED_TRACE ("conversion to long double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wsign-promo"
		const auto t_out_array = std::to_array<std::string>({std::to_string (std::numeric_limits<long double>::lowest ()), std::to_string (std::numeric_limits<float>::lowest ()), "-10.500000", "-1.000000", "0.000000", "1.000000", "20.400000", std::to_string (std::numeric_limits<float>::max ()), std::to_string (std::numeric_limits<long double>::max ())});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to string");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}
}


TEST(ConvertTest, StringTest)
{
	// input array
	const auto t_in_array = std::to_array<std::string>({"-5123456789", "-65535", "-65534.94123", "-128.1", "-32", "-10.123456789", "-1", "0", "0.4", "0.6", "1", "21.9876543210", "65535", "9876543210"});

	// expected result of the conversion of input array
	// for bool type, these are all true
	constexpr const auto t_res_array_true = std::to_array<bool>({true, true, true, true, true, true, true, true, true, true, true, true, true, true});
	constexpr const auto t_res_array_false = std::to_array<bool>({false, false, false, false, false, false, false, false, false, false, false, false, false, false});

	SCOPED_TRACE ("conversion from string");

	{
		// expected output for conversion to bool (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<bool>({false, false, false, false, false, false, false, false, false, false, true, false, false, false});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, false, false, false, true, false, false, true, false, false, false});
		SCOPED_TRACE ("conversion to bool");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned char>({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, false, false, false, true, false, false, true, false, false, false});
		SCOPED_TRACE ("conversion to unsigned char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed char (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed char>({0, 0, 0, 0, -32, 0, -1, 0, 0, 0, 1, 0, 0, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, true, false, true, true, false, false, true, false, false, false});
		SCOPED_TRACE ("conversion to signed char");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<unsigned int>({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 65535, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, false, false, false, true, false, false, true, false, true, false});
		SCOPED_TRACE ("conversion to unsigned int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<signed int>({0, -65535, 0, 0, -32, 0, -1, 0, 0, 0, 1, 0, 65535, 0});
		constexpr const auto t_res_array = std::to_array<bool>({false, true, false, false, true, false, true, true, false, false, true, false, true, false});
		SCOPED_TRACE ("conversion to signed int");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit unsigned int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<uint64_t>({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 65535, 9876543210});
		constexpr const auto t_res_array = std::to_array<bool>({false, false, false, false, false, false, false, true, false, false, true, false, true, true});
		SCOPED_TRACE ("conversion to uint64_t");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to 64-bit signed int (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<int64_t>({-5123456789, -65535, 0, 0, -32, 0, -1, 0, 0, 0, 1, 0, 65535, 9876543210});
		constexpr const auto t_res_array = std::to_array<bool>({true, true, false, false, true, false, true, true, false, false, true, false, true, true});
		SCOPED_TRACE ("conversion to int64_t");
		RunTest (t_in_array, t_res_array, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to float (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<float>({-5123456789, -65535, -65534.94123, -128.1, -32, -10.123456789, -1, 0, 0.4, 0.6, 1, 21.9876543210, 65535, 9876543210});
		//constexpr const auto t_res_array = std::to_array<bool>({false, true, true, true, true, true, true, true, false});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to float");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to double (best_effort: both true & false)
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		constexpr const auto t_out_array = std::to_array<double>({-5123456789, -65535, -65534.94123, -128.1, -32, -10.123456789, -1, 0, 0.4, 0.6, 1, 21.9876543210, 65535, 9876543210});
		//constexpr const auto t_res_array = std::to_array<bool>({false, true, true, true, true, true, true, true, false});
		#pragma GCC diagnostic pop
		SCOPED_TRACE ("conversion to double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		constexpr const auto t_out_array = std::to_array<long double>({-5123456789, -65535, -65534.94123, -128.1, -32, -10.123456789, -1, 0, 0.4, 0.6, 1, 21.9876543210, 65535, 9876543210});
		SCOPED_TRACE ("conversion to long double");
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_false, t_out_array, false);
	}

	{
		// expected output for conversion to long double (best_effort: both true & false)
		const auto t_out_array = std::to_array<std::string>({"-5123456789", "-65535", "-65534.94123", "-128.1", "-32", "-10.123456789", "-1", "0", "0.4", "0.6", "1", "21.9876543210", "65535", "9876543210"});
		RunTest (t_in_array, t_res_array_true, t_out_array, true);
		RunTest (t_in_array, t_res_array_true, t_out_array, false);
	}
}

TEST(ConvertTest, InvalidStringTest)
{
	// input array
	const auto t_in_array = std::to_array<std::string>({"10 A Inval1d Str1ng w!th s0m3 numb3rs."});

	SCOPED_TRACE ("conversion from and invalid string");
	// expected output for conversion to bool (best_effort: both true & false)
	constexpr const auto t_out_array = std::to_array<int>({0});
	constexpr const auto t_res_array = std::to_array<bool>({false});
	SCOPED_TRACE ("conversion to int");
	RunTest (t_in_array, t_res_array, t_out_array, true);
	RunTest (t_in_array, t_res_array, t_out_array, false);
}