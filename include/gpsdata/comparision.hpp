#ifndef _X_GPSDATA_COMPARISON_
#define _X_GPSDATA_COMPARISON_

#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/traits/GpsSegment.hpp>
#include <gpsdata/traits/GpsPoint.hpp>
#include <gpsdata/GpsRoute.hpp>
#include <gpsdata/GpsSegment.hpp>
#include <gpsdata/GpsPoint.hpp>
#include <gpsdata/GpsStatistics.hpp>
#include <gpsdata/GpsValue.hpp>

namespace gpsdata {
	template<GpsFactoryTrait F, GpsSegmentTrait S>
	bool operator== (const GpsRoute<F, S>& a, const GpsRoute<F, S>& b) {
		DEBUG_MSG("%s: compare GpsRoute\n", __func__);
		if (a.getFactory () != b.getFactory ()) throw std::runtime_error ("comparison is not true");
		if (a.getId () != b.getId ()) throw std::runtime_error ("comparison is not true");
		if (a.getTimezoneOffset () != b.getTimezoneOffset ()) throw std::runtime_error ("comparison is not true");
		if (a.getActivityType () != b.getActivityType ()) throw std::runtime_error ("comparison is not true");
		if (a.getTitle () != b.getTitle ()) throw std::runtime_error ("comparison is not true");
		if (a.getSummary () != b.getSummary ()) throw std::runtime_error ("comparison is not true");
		if (a.getDetails () != b.getDetails ()) throw std::runtime_error ("comparison is not true");

		if (a.getStatistics () != b.getStatistics ()) throw std::runtime_error ("comparison is not true");

		// Get the an iterator to the first element of both lists
		auto it_a = a.cbegin ();
		auto it_b = b.cbegin ();
		// Loop over both lists and compare the elements, if one (or both) lists reach the end, stop.
		while (it_a != a.cend () && it_b !=b.cend ()) {
			if (!(**it_a == **it_b)) throw std::runtime_error ("comparison is not true");
			++it_a;
			++it_b;
		}
		// If both lists reach the end at the same time, they are equal, otherwise not.
		if (it_a != a.cend () || it_b != b.cend ()) throw std::runtime_error ("comparison is not true");
		return true;
	}

	template<GpsFactoryTrait F, GpsPointTrait P>
	bool operator== (const GpsSegment<F, P>& a, const GpsSegment<F, P>& b) {
		DEBUG_MSG("%s: compare GpsSegment\n", __func__);
		if (a.getFactory () != b.getFactory ()) throw std::runtime_error ("comparison is not true");

		if (a.getSegmentNumber () != b.getSegmentNumber ()) throw std::runtime_error ("comparison is not true");

		if (a.getStatistics () != b.getStatistics ()) throw std::runtime_error ("comparison is not true");

		// Get the an iterator to the first element of both lists
		auto it_a = a.cbegin ();
		auto it_b = b.cbegin ();
		// Loop over both lists and compare the elements, if one (or both) lists reach the end, stop.
		while (it_a != a.cend () && it_b !=b.cend ()) {
			if (!(**it_a == **it_b)) throw std::runtime_error ("comparison is not true");
			it_a++;
			it_b++;
		}
		// If both lists reach the end at the same time, they are equal, otherwise not.
		if (it_a != a.cend () || it_b != b.cend ()) throw std::runtime_error ("comparison is not true");
		return true;
	}

	template<GpsFactoryTrait F>
	bool operator== (const GpsPoint<F>& a, const GpsPoint<F>& b) {
		DEBUG_MSG("%s: compare GpsPoint\n", __func__);
		if (a.getFactory () != b.getFactory ()) throw std::runtime_error ("comparison is not true");

		if (a.getTime () != b.getTime ()) throw std::runtime_error ("comparison is not true");

		// Get the an iterator to the first element of both lists
		auto it_a = a.cbegin ();
		auto it_b = b.cbegin ();
		// Loop over both lists and compare the elements, if one (or both) lists reach the end, stop.
		while (it_a != a.cend () && it_b !=b.cend ()) {
			if (!(*it_a == *it_b)) throw std::runtime_error ("comparison is not true");
			it_a++;
			it_b++;
		}
		// If both lists reach the end at the same time, they are equal, otherwise not.
		if (it_a != a.cend () || it_b != b.cend ()) throw std::runtime_error ("comparison is not true");
		return true;
	}

	template<GpsFactoryTrait F>
	bool operator== (const GpsStatistics<F>& a, const GpsStatistics<F>& b) {
		DEBUG_MSG("%s: compare GpsStatistics\n", __func__);
		if (a.getFactory () != b.getFactory ()) throw std::runtime_error ("comparison is not true");

		if (a.getStatistics () != b.getStatistics ()) throw std::runtime_error ("comparison is not true");
		return true;
	}

	template<typename T>
	bool operator== (const GpsValue<T>& a, const GpsValue<T>& b) {
		DEBUG_MSG("%s: compare GpsValue\n", __func__);
		if (a.type != b.type) throw std::runtime_error ("comparison is not true");
		if (a.value._raw != b.value._raw) throw std::runtime_error ("comparison is not true");
		return true;
	}
}

#endif /* _X_GPSDATA_COMPARISON_ */
