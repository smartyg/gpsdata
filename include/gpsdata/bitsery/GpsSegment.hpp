#ifndef _X_GPSDATA_BITSERY_GPSEGMENT_
#define _X_GPSDATA_BITSERY_GPSEGMENT_

#include <type_traits>
#include <memory>
#include <cstdlib>

#include <bitsery/bitsery.h>

#include <gpsdata/traits/is_instance.hpp>
#include <gpsdata/traits/GpsSegment.hpp>
#include <gpsdata/GpsSegment.hpp>

#include <gpsdata/bitsery/GpsPoint.hpp>
#include <gpsdata/bitsery/GpsValue.hpp>

namespace gpsdata {
	template<typename B, GpsSegmentTrait S>
	void serialize (B& s, std::shared_ptr<S>& segment) requires(std::is_base_of<GpsSegment<typename S::GpsFactory, typename S::Point>, S>::value) {

		if constexpr (is_instance<B, bitsery::Serializer>{}) {
			s.value4b (segment->_n);
			serializeGpsValueVector (s, segment->_statistics, segment->getFactory ());

			s.value4b (static_cast<int>(segment->_points.size ()));
			for (const auto& point : *segment) {
				s.object (point);
			}
		}

		if constexpr (is_instance<B, bitsery::Deserializer>{}) {
			s.value4b (segment->_n);
			deserializeGpsValueVector (s, segment->_statistics, segment->getFactory ());

			int n_points;
			s.value4b (n_points);
			if (n_points > 0) {
				typename S::Point *block = static_cast<typename S::Point *>(calloc (n_points, sizeof(typename S::Point)));
				DEBUG_MSG("reserved block at %p\n", static_cast<void *>(block));
				std::shared_ptr<void> block_ptr (static_cast<void *>(block), [] (void *ptr) -> void {
					// This lambda function is called once all references to the shared pointer to this block have gone out of scope.
					// Now we can safely release the reserved memory as well.
					DEBUG_MSG("free block at %p\n", ptr);
					free (ptr);
				});

				auto deleter = [block_ptr] (void *ptr) -> void {
					DEBUG_MSG("call deleter on GpsPoint at %p\n", ptr);
					// Convert the pointer `ptr` back to it's original object
					typename S::Point *obj = reinterpret_cast<typename S::Point *>(ptr);
					// Now call the destructor, this allows to release all resources owned by the object without releasing the memory the object is in.
					std::destroy_at (obj);

					// Hold copy of block_ptr to keep track when all objects from the allocated memory are released.
					// block_ptr.reset () does (somehow) not work as block_ptr seems to be const.
					(void)block_ptr;
					return;
				};

				for (int i = 0; i < n_points; ++i) {
					typename S::Point *point_ptr = &(block[i]);
					typename S::Point *point_ptr2 = new (point_ptr) typename S::Point (segment->getFactory ());
					assert (point_ptr == point_ptr2);

					std::shared_ptr<typename S::Point> point (point_ptr, deleter);

					s.object (point);
					segment->_points.push_back (point);
				}
			}
		}
	};
}

#endif /* _X_GPSDATA_BITSERY_GPSEGMENT_ */
