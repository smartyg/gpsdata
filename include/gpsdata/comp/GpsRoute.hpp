#ifndef _GPSDATA_COMP_GPSROUTE_
#define _GPSDATA_COMP_GPSROUTE_

#include <Logger.hpp>

#include <gpsdata/traits/GpsFactory.hpp>
#include <gpsdata/traits/GpsSegment.hpp>
#include <gpsdata/GpsRoute.hpp>

#include <gpsdata/comp/GpsSegment.hpp>
#include <gpsdata/comp/GpsStatistics.hpp>

namespace gpsdata {
	template<GpsFactoryTrait F, GpsSegmentTrait S>
	bool operator== (const GpsRoute<F, S>& a, const GpsRoute<F, S>& b) {
		DEBUG_MSG ("{:s}: compare GpsRoute\n", __func__);
		if (a.getFactory () != b.getFactory ()) return false;
		if (a.getId () != b.getId ()) return false;
		if (a.getTimezoneOffset () != b.getTimezoneOffset ()) return false;
		if (a.getActivityType () != b.getActivityType ()) return false;
		if (a.getTitle () != b.getTitle ()) return false;
		if (a.getSummary () != b.getSummary ()) return false;
		if (a.getDetails () != b.getDetails ()) return false;

		if (a.getStatistics () != b.getStatistics ()) return false;

		// Get the an iterator to the first element of both lists
		auto it_a = a.cbegin ();
		auto it_b = b.cbegin ();
		// Loop over both lists and compare the elements, if one (or both) lists reach the end, stop.
		while (it_a != a.cend () && it_b !=b.cend ()) {
			if (!(**it_a == **it_b)) return false;
			++it_a;
			++it_b;
		}
		// If both lists reach the end at the same time, they are equal, otherwise not.
		if (it_a != a.cend () || it_b != b.cend ()) return false;
		return true;
	}
}

#endif /* _GPSDATA_COMP_GPSROUTE_ */
