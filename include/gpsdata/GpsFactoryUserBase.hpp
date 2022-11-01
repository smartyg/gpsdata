#ifndef _GPRDATA_GPSFACTORYUSERBASE_
#define _GPRDATA_GPSFACTORYUSERBASE_

#include <type_traits>
#include <memory>

#include <gpsdata/traits/GpsFactory.hpp>

namespace gpsdata::internal {
	template<GpsFactoryTrait F>
	class GpsFactoryUserBase {

	protected:
		const std::shared_ptr<const F> _factory;

		GpsFactoryUserBase (const std::shared_ptr<const F>& factory) : _factory(factory) { }
		~GpsFactoryUserBase (void) = default;

	public:
		const std::shared_ptr<const F> getFactory (void) const {
			return this->_factory;
		}
	};
}

#endif /* _GPRDATA_GPSFACTORYUSERBASE_ */
