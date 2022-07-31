#ifndef _X_GPSDATA_LOGGER_
#define _X_GPSDATA_LOGGER_

#include <cstdint>
#include <exception>
#include <stdio.h>

#define CRITICAL_MSG(format, ...) (gpsdata::utils::Logger::Log (gpsdata::utils::Logger::CRIT, __FILE__, __func__, __LINE__, format, __VA_ARGS__))
#define ERROR_MSG(format, ...) (gpsdata::utils::Logger::Log (gpsdata::utils::Logger::ERR, __FILE__, __func__, __LINE__, format, __VA_ARGS__))
#define WARNING_MSG(format, ...) (gpsdata::utils::Logger::Log (gpsdata::utils::Logger::WARNING, __FILE__, __func__, __LINE__, format, __VA_ARGS__))
#define NOTICE_MSG(format, ...) (gpsdata::utils::Logger::Log (gpsdata::utils::Logger::NOTICE, __FILE__, __func__, __LINE__, format, __VA_ARGS__))
#define INFO_MSG(format, ...) (gpsdata::utils::Logger::Log (gpsdata::utils::Logger::INFO, __FILE__, __func__, __LINE__, format, __VA_ARGS__))

#ifdef _RELEASE
#define DEBUG_MSG(format, ...) (gpsdata::utils::empty_func())
namespace gpsdata::utils {
	inline const void empty_func (void) { return; };
}
#else
#define DEBUG_MSG(format, ...) (gpsdata::utils::Logger::Log (gpsdata::utils::Logger::DEBUG, __FILE__, __func__, __LINE__, format, __VA_ARGS__))
#endif

namespace gpsdata::utils {

	class Logger {
		Logger (void) = delete;
		~Logger (void) = delete;

	public:
		enum Loglevel : uint_fast8_t {
			EMERG   = 0, // System is unusable
			ALERT   = 1, // Action must be taken immediately
			CRIT    = 2, // Critical conditions
			ERR     = 3, // Error conditions
			WARNING = 4, // Warning conditions
			NOTICE  = 5, // Normal but significant condition
			INFO    = 6, // Informational
			DEBUG   = 7  // Debug-level messages
		};

		static void Log (const Loglevel&, const char *, const char *, const int, const char *, ...);
		static void Exception (const Loglevel&, const std::exception&);

		static FILE *getStream (void);
		static Loglevel getLoglevel (void);

		static bool setStream (FILE *);
		static bool setLoglevel (const Loglevel&);

	private:
		static FILE *stream;
		static Loglevel level;

		static const char *levelToString (const Loglevel&);
	};
}

#endif /* _X_LIBGPSFILE2_LOGGER_ */
