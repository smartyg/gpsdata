#ifndef _X_GPSDATA_BITSERY_GPSROUTE_
#define _X_GPSDATA_BITSERY_GPSROUTE_

#include <bitsery/bitsery.h>
#include <bitsery/traits/string.h>
#include <bitsery/traits/vector.h>
#include <bitsery/traits/list.h>

#include <gpsdata/traits/GpsPoint.hpp>
#include <gpsdata/traits/GpsSegment.hpp>
#include <gpsdata/traits/GpsRoute.hpp>

#include <gpsdata/GpsPoint.hpp>
#include <gpsdata/GpsSegment.hpp>
#include <gpsdata/GpsRoute.hpp>

namespace gpsdata {
	namespace
	{
		template <typename, template <typename...> typename>
		struct is_instance_impl : public std::false_type {};

		template <template <typename...> typename U, typename...Ts>
		struct is_instance_impl<U<Ts...>, U> : public std::true_type {};
	}

	template <typename T, template <typename ...> typename U>
	using is_instance = is_instance_impl<std::decay_t<T>, U>;

	template <typename B, GpsFactoryTrait F>
	void serializeGpsValueVector (B& s, std::vector<GpsValue<typename F::DataType>>& v, const std::shared_ptr<const F>& factory) {
		assert (v.size () < 256);
		uint8_t n_elements = v.size ();
		s.value1b (n_elements);
		auto it = v.cbegin ();
		for (int i = 0; i < n_elements; ++i) {
			const std::string type_string = factory->getDataTypeString ((*it).type);
			assert (type_string.size () < 16);

			s.text1b (type_string, 16);
			s.value8b ((*it).value._raw);
			++it;
		}
	}

	template <typename B, GpsFactoryTrait F>
	void deserializeGpsValueVector (B& s, std::vector<GpsValue<typename F::DataType>>& v, const std::shared_ptr<const F>& factory) {
		uint8_t n_elements;
		s.value1b (n_elements);
		v.reserve (std::max (n_elements, GPSVALUEVECTOR_MIN_SIZE));

		for (int i = 0; i < n_elements; ++i) {
			std::string type_string;
			uint64_t value;

			s.text1b (type_string, 16);
			s.value8b (value);

			typename F::DataType type = factory->getDataType (type_string);

			v.push_back (GpsValue<typename F::DataType> {type, value});
		}
	}

	template<typename B, GpsRouteTrait R>
	void serialize (B& s, std::shared_ptr<R>& route) requires(std::is_base_of<GpsRoute<typename R::GpsFactory, typename R::Segment>, R>::value) {
		if constexpr (is_instance<B, bitsery::Serializer>{}) {
			std::cout << "GpsRoute Serializer function" << std::endl;
			s.object (route->_id);
			s.value4b (route->_timezone_offset);
			s.text1b (route->getFactory ()->getActivityTypeString (route->_activity_type), 128);
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
			std::cout << "GpsRoute Deserializer function" << std::endl;
			s.object (route->_id);
			s.value4b (route->_timezone_offset);
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
				DEBUG_MSG("reserved block at %p\n", static_cast<void *>(block));
				std::shared_ptr<void> block_ptr (static_cast<void *>(block), [] (void *ptr) -> void {
					// This lambda function is called once all references to the shared pointer to this block have gone out of scope.
					// Now we can safely release the reserved memory as well.
					DEBUG_MSG("free block at %p\n", ptr);
					free (ptr);
				});

				auto deleter = [block_ptr] (void *ptr) -> void {
					DEBUG_MSG("call deleter on GpsSegment at %p\n", ptr);
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
					INFO_MSG ("segment %d\n", i);
					typename R::Segment *segment_ptr = &(block[i]);
					typename R::Segment *segment_ptr2 = new (segment_ptr) typename R::Segment (route->getFactory ());
					INFO_MSG ("new segment create at %p\n", segment_ptr);
					assert (segment_ptr == segment_ptr2);

					INFO_MSG ("create shared pointer from pointer %p\n", segment_ptr2);
					std::shared_ptr<typename R::Segment> segment (segment_ptr, deleter);
					s.object (segment);
					INFO_MSG ("segment size: %d\n", route->_segments.size ());
					route->_segments.push_back (segment);
					INFO_MSG ("segment size: %d\n", route->_segments.size ());
				}
			}
		}
	};

	template<typename B, GpsSegmentTrait S>
	void serialize (B& s, std::shared_ptr<S>& segment) requires(std::is_base_of<GpsSegment<typename S::GpsFactory, typename S::Point>, S>::value) {

		if constexpr (is_instance<B, bitsery::Serializer>{}) {
			std::cout << "GpsSegment Serializer function" << std::endl;

			s.value4b (segment->_n);

			serializeGpsValueVector (s, segment->_statistics, segment->getFactory ());

			s.value4b (static_cast<int>(segment->_points.size ()));
			for (const auto& point : *segment) {
				s.object (point);
			}
		}

		if constexpr (is_instance<B, bitsery::Deserializer>{}) {
			std::cout << "GpsSegment Deserializer function" << std::endl;

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

	template<typename B, GpsPointTrait P>
	void serialize (B& s, std::shared_ptr<P>& point) requires(std::is_base_of<GpsPoint<typename P::GpsFactory>, P>::value) {

		if constexpr (is_instance<B, bitsery::Serializer>{}) {
			std::cout << "GpsPoint Serializer function" << std::endl;
			s.object (point->_time);

			serializeGpsValueVector (s, point->_data, point->getFactory ());
		}

		if constexpr (is_instance<B, bitsery::Deserializer>{}) {
			std::cout << "GpsPoint Deserializer function" << std::endl;
			s.object (point->_time);

			deserializeGpsValueVector (s, point->_data, point->getFactory ());
		}
	};

	template<typename B, GpsStatisticsTrait S>
	void serialize (B& s, S& statistics) requires(std::is_base_of<GpsStatistics<typename S::GpsFactory>, S>::value) {
		if constexpr (is_instance<B, bitsery::Serializer>{}) {
			std::cout << "GpsStatistics Serializer function" << std::endl;

			serializeGpsValueVector (s, statistics->_statistics, statistics->getFactory ());
		}

		if constexpr (is_instance<B, bitsery::Deserializer>{}) {
			std::cout << "GpsStatistics Deserializer function" << std::endl;

			deserializeGpsValueVector (s, statistics->_statistics, statistics->getFactory ());
		}
	};

	inline namespace types {
		template <typename B>
		void serialize (B& s, ObjectId& o) {
			s.value4b (o._id);
		};

		template <typename B>
		void serialize (B& s, ObjectTime& o) {
			if constexpr (is_instance<B, bitsery::Serializer>{}) {
				std::cout << "ObjectTime Serializer function" << std::endl;
				ObjectTime::internalTimeType value = o.getTime ();
				//std::cout << "write value: " << std::to_string (value) << std::endl;
				s.value8b (value);
			}

			if constexpr (is_instance<B, bitsery::Deserializer>{}) {
				std::cout << "ObjectTime Deserializer function" << std::endl;
				ObjectTime::internalTimeType value;
				s.value8b (value);
				//std::cout << "read value: " << std::to_string (value) << std::endl;
				o = value;
			}
		};

		template <typename B>
		void serialize (B& s, const ObjectTime& o) {
			if constexpr (is_instance<B, bitsery::Serializer>{}) {
				std::cout << "ObjectTime Serializer function" << std::endl;
				ObjectTime::internalTimeType value = o.getTime ();
				s.value8b (value);
			}
		};
	}
}

#endif /* _X_GPSDATA_BITSERY_GPSROUTE_ */
