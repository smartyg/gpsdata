#ifndef _X_GPSDATA_OBJECTID_
#define _X_GPSDATA_OBJECTID_

#include <cstdint>
#include <climits>

namespace gpsdata {
	inline namespace types {

		class ObjectId final {
			template <typename B>
			friend void serialize (B&, ObjectId&);

		private:
			uint32_t _id;

		public:
			ObjectId (void) noexcept { this->_id = 0; }
			ObjectId (uint32_t id) noexcept {
				this->_id = id;
			}

			explicit ObjectId (const ObjectId& other) : ObjectId(other._id) {} // copy constructor

			ObjectId (ObjectId&& other) { this->_id = other._id; } // move constructor

			ObjectId& operator=(const ObjectId& other) {// copy assignment
				this->_id = other._id;
				return *this;
			}

			ObjectId& operator=(ObjectId&& other) { // move assignment
				this->_id = other._id;
				return *this;
			}

			ObjectId& operator=(const uint64_t& id) noexcept {
				this->_id = id;
				return *this;
			}

			inline uint32_t getId (void) const noexcept { return this->_id; }

			void* operator new (size_t) = delete;
			void operator delete (void*) = delete;

			inline bool operator==(const ObjectId& rhs) const noexcept { return (this->_id == rhs._id); }
			inline bool operator==(const uint32_t& rhs) const noexcept { return (this->_id == rhs); }
			inline bool operator!=(const ObjectId& rhs) const noexcept { return (this->_id != rhs._id); }
			inline bool operator!=(const uint32_t& rhs) const noexcept { return (this->_id != rhs); }

			inline bool operator< (const ObjectId&) const noexcept = delete;
			inline bool operator> (const ObjectId&) const noexcept = delete;
			inline bool operator<=(const ObjectId&) const noexcept = delete;
			inline bool operator>=(const ObjectId&) const noexcept = delete;

			inline ObjectId& operator+=(const ObjectId&) noexcept = delete;
			inline ObjectId& operator-=(const ObjectId&) noexcept = delete;

			inline ObjectId& operator++(void) noexcept = delete;
			inline ObjectId  operator++(int)  noexcept = delete;
			inline ObjectId& operator--(void) noexcept = delete;
			inline ObjectId  operator--(int)  noexcept = delete;

			inline ObjectId operator+(const ObjectId&) noexcept = delete;
			inline ObjectId operator+(const int&)      noexcept = delete;
			inline ObjectId operator-(const ObjectId&) noexcept = delete;
			inline ObjectId operator-(const int&)      noexcept = delete;

			explicit inline operator bool() const noexcept { return (this->_id > 0); }

			inline operator uint32_t() const noexcept { return this->_id; }
			#if INT_MAX >= UINT32_MAX
			explicit inline operator int() const noexcept { return this->_id; }
			#endif
			#if UINT_MAX >= UINT32_MAX
			explicit inline operator unsigned int() const noexcept { return this->_id; }
			#endif
			#if LONG_MAX >= UINT32_MAX
			explicit inline operator long int() const noexcept { return this->_id; }
			#endif
			#if ULONG_MAX >= UINT32_MAX
			explicit inline operator long unsigned int() const noexcept { return this->_id; }
			#endif
			#if LLONG_MAX >= UINT32_MAX
			explicit inline operator long long int() const noexcept { return this->_id; }
			#endif
			#if ULLONG_MAX >= UINT32_MAX
			explicit inline operator long long unsigned int() const noexcept { return this->_id; }
			#endif
		};
	}
}

#endif /* _X_GPSDATA_OBJECTID_ */