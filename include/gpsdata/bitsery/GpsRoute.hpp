#ifndef _GPSDATA_BITSERY_GPSROUTE_
#define _GPSDATA_BITSERY_GPSROUTE_

#include <type_traits>
#include <memory>
#include <cstdlib>
#include <Logger.hpp>

#include <bitsery/bitsery.h>
#include <bitsery/traits/string.h>

#include <gpsdata/traits/is_instance.hpp>
#include <gpsdata/traits/GpsRoute.hpp>
#include <gpsdata/GpsRoute.hpp>

#include <gpsdata/bitsery/GpsSegment.hpp>
#include <gpsdata/bitsery/ObjectId.hpp>
#include <gpsdata/bitsery/GpsValue.hpp>

namespace gpsdata {
	template<typename B, GpsRouteTrait R>
	void serialize (B& s, std::shared_ptr<R>& route) requires(std::is_base_of<GpsRoute<typename R::GpsFactory, typename R::Segment>, R>::value) {
		if constexpr (is_instance<B, bitsery::Serializer>{}) {
			s.object (route->_id);
			s.text1b (std::string (route->getFactory ()->getActivityTypeString (route->_activity_type)), 128);
			s.text1b (route->_title, 128);
			s.text1b (route->_summary, 128);
			s.text1b (route->_details, 128);

			serializeGpsValueVector (s, route->_statistics, route->getFactory ());

			s.value4b (route->nSegments ());
			for (const auto& segment : *route) {
				s.object (segment);
			}
		}

		if constexpr (is_instance<B, bitsery::Deserializer>{}) {
			s.object (route->_id);
			std::string activity_string;
			s.text1b (activity_string, 128);
			route->_activity_type = route->getFactory ()->getActivityType (activity_string);
			s.text1b (route->_title, 128);
			s.text1b (route->_summary, 128);
			s.text1b (route->_details, 128);

			deserializeGpsValueVector (s, route->_statistics, route->getFactory ());

			int n_segments;
			s.value4b (n_segments);
			if (n_segments > 0) {
				route->_segments.reserve (n_segments);

				typename R::Segment *block = static_cast<typename R::Segment *>(calloc (n_segments, sizeof(typename R::Segment)));
				DEBUG_MSG ("reserved block at {:p}\n", static_cast<void *>(block));
				std::shared_ptr<void> block_ptr (static_cast<void *>(block), [] (void *ptr) -> void {
					// This lambda function is called once all references to the shared pointer to this block have gone out of scope.
					// Now we can safely release the reserved memory as well.
					DEBUG_MSG("free block at %p\n", ptr);
					free (ptr);
				});

				auto deleter = [block_ptr] (void *ptr) -> void {
					DEBUG_MSG ("call deleter on GpsSegment at {:p}\n", ptr);
					// Convert the pointer `ptr` back to it's original object
					typename R::Segment *obj = reinterpret_cast<typename R::Segment *>(ptr);
					// Now call the destructor, this allows to release all resources owned by the object without releasing the memory the object is in.
					std::destroy_at (obj);

					// Hold copy of block_ptr to keep track when all objects from the allocated memory are released.
					// block_ptr.reset () does (somehow) not work as block_ptr seems to be const.
					(void)block_ptr;
					return;
				};

				for (int i = 0; i < n_segments; ++i) {
					typename R::Segment *segment_ptr = &(block[i]);
					typename R::Segment *segment_ptr2 = new (segment_ptr) typename R::Segment (route->getFactory ());
					assert (segment_ptr == segment_ptr2);

					std::shared_ptr<typename R::Segment> segment (segment_ptr, deleter);
					s.object (segment);
					route->_segments.push_back (segment);
				}
			}
		}
	};
}

#endif /* _GPSDATA_BITSERY_GPSROUTE_ */
