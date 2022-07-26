#ifndef _GPSDATA_TYPE_OBJECTID_
#define _GPSDATA_TYPE_OBJECTID_

#include <cstdint>
#include <climits>

namespace gpsdata {
	inline namespace types {

		class ObjectId final {
			template <typename B>
			friend void serialize (B&, ObjectId&);

		public:
			typedef uint32_t Type;

		private:
			Type _id;

		public:
			ObjectId (void) noexcept { this->_id = 0; }
			ObjectId (Type id) noexcept {
				this->_id = id;
			}

			explicit ObjectId (const ObjectId& other) : ObjectId(other._id) {} // copy constructor

			ObjectId (ObjectId&& other) { this->_id = other._id; other._id = 0; } // move constructor

			ObjectId& operator=(const ObjectId& other) {// copy assignment
				this->_id = other._id;
				return *this;
			}

			ObjectId& operator=(ObjectId&& other) { // move assignment
				this->_id = other._id;
				other._id = 0;
				return *this;
			}

			ObjectId& operator=(const uint64_t& id) noexcept {
				this->_id = id;
				return *this;
			}

			inline Type getId (void) const noexcept { return this->_id; }

			void* operator new (size_t) = delete;
			void operator delete (void*) = delete;

			inline bool operator==(const ObjectId& rhs) const noexcept { return (this->_id == rhs._id); }
			inline bool operator==(const Type& rhs) const noexcept { return (this->_id == rhs); }
			inline bool operator!=(const ObjectId& rhs) const noexcept { return (this->_id != rhs._id); }
			inline bool operator!=(const Type& rhs) const noexcept { return (this->_id != rhs); }

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

			inline operator Type() const noexcept { return this->_id; }
		};
	}
}

#endif /* _GPSDATA_TYPE_OBJECTID_ */