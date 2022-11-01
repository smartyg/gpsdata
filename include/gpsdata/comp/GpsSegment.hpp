#ifndef _GPSDATA_COMP_GPSSEGMENT_
#define _GPSDATA_COMP_GPSSEGMENT_

#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/traits/GpsPoint.hpp>
#include <gpsdata/GpsSegment.hpp>

#include <gpsdata/comp/GpsPoint.hpp>
#include <gpsdata/comp/GpsStatistics.hpp>

namespace gpsdata {
	template<GpsFactoryTrait F, GpsPointTrait P>
	bool operator== (const GpsSegment<F, P>& a, const GpsSegment<F, P>& b) {
		DEBUG_MSG("%s: compare GpsSegment\n", __func__);
		if (a.getFactory () != b.getFactory ()) return false;

		if (a.getSegmentNumber () != b.getSegmentNumber ()) return false;

		if (a.getStatistics () != b.getStatistics ()) return false;

		// Get the an iterator to the first element of both lists
		auto it_a = a.cbegin ();
		auto it_b = b.cbegin ();
		// Loop over both lists and compare the elements, if one (or both) lists reach the end, stop.
		while (it_a != a.cend () && it_b !=b.cend ()) {
			if (!(**it_a == **it_b)) return false;
			it_a++;
			it_b++;
		}
		// If both lists reach the end at the same time, they are equal, otherwise not.
		if (it_a != a.cend () || it_b != b.cend ()) return false;
		return true;
	}
}

#endif /* _GPSDATA_COMP_GPSSEGMENT_ */
