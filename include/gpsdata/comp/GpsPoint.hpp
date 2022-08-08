#ifndef _X_GPSDATA_COMP_GPSPOINT_
#define _X_GPSDATA_COMP_GPSPOINT_

#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/GpsPoint.hpp>

#include <gpsdata/comp/GpsValue.hpp>

namespace gpsdata {
	template<GpsFactoryTrait F>
	bool operator== (const GpsPoint<F>& a, const GpsPoint<F>& b) {
		DEBUG_MSG("%s: compare GpsPoint\n", __func__);
		if (a.getFactory () != b.getFactory ()) return false;

		if (a.getTime () != b.getTime ()) return false;

		// Get the an iterator to the first element of both lists
		auto it_a = a.cbegin ();
		auto it_b = b.cbegin ();
		// Loop over both lists and compare the elements, if one (or both) lists reach the end, stop.
		while (it_a != a.cend () && it_b !=b.cend ()) {
			if (!(*it_a == *it_b)) return false;
			it_a++;
			it_b++;
		}
		// If both lists reach the end at the same time, they are equal, otherwise not.
		if (it_a != a.cend () || it_b != b.cend ()) return false;
		return true;
	}
}

#endif /* _X_GPSDATA_COMP_GPSPOINT_ */
