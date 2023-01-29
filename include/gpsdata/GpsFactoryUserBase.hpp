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

		GpsFactoryUserBase (void) = default;
		GpsFactoryUserBase (const std::shared_ptr<const F>& factory) noexcept : _factory(factory) { }

	private:
		GpsFactoryUserBase (const GpsFactoryUserBase&)                = delete; // copy constructor
		GpsFactoryUserBase (GpsFactoryUserBase&&) noexcept            = delete; // move constructor
		GpsFactoryUserBase& operator= (const GpsFactoryUserBase&)     = delete; // copy assignment
		GpsFactoryUserBase& operator= (GpsFactoryUserBase&&) noexcept = delete; // move assignment

	public:
		~GpsFactoryUserBase (void) = default;

		const std::shared_ptr<const F> getFactory (void) const noexcept {
			return this->_factory;
		}
	};
}

#endif /* _GPRDATA_GPSFACTORYUSERBASE_ */
