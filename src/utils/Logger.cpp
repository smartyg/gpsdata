#include "config.h"

#include "gpsdata/utils/Logger.hpp"

#include <exception>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdio_ext.h>

using gpsdata::utils::Logger;

#ifdef _DEBUG
Logger::Loglevel Logger::level = Logger::DEBUG;
FILE *Logger::stream = stdout;
#else
Logger::Loglevel Logger::level = Logger::NOTICE;
FILE *Logger::stream = stderr;
#endif

void Logger::Exception (const Loglevel& level, const std::exception& e) {
	(void) level;
	(void) e;
}

void Logger::Log (const Loglevel& level, const char *filename, const char *function, const int line, const char *format, ...) {
	va_list ap;
#ifdef _RELEASE
	fprintf (Logger::stream, "%s: ", Logger::levelToString(level));
#else
	if (filename == nullptr && function == nullptr) fprintf (Logger::stream, "%s: ", Logger::levelToString(level));
	else if (filename == nullptr) fprintf (Logger::stream, "%s: %s: ", Logger::levelToString(level), function);
	else if (function == nullptr) fprintf (Logger::stream, "%s: %s:%d: ", Logger::levelToString(level), filename, line);
	else fprintf (Logger::stream, "%s: %s:%d %s: ", Logger::levelToString(level), filename, line, function);
#endif
	va_start (ap, format);
	vfprintf (Logger::stream, format, ap);
	va_end (ap);
}

FILE *Logger::getStream (void) {
	return Logger::stream;
}

Logger::Loglevel Logger::getLoglevel (void) {
	return Logger::level;
}

bool Logger::setStream (FILE *s) {
	if (__fwritable (s) == 0) return false;
	Logger::stream = s;
	return true;
}

bool Logger::setLoglevel (const Loglevel& l) {
	Logger::level = l;
	return true;
}

const char *Logger::levelToString (const Loglevel& level) {
	switch(level)
	{
		case Logger::EMERG   : return "EMERGENCY";
		case Logger::ALERT   : return "ALERT";
		case Logger::CRIT    : return "CRITICAL";
		case Logger::ERR     : return "ERROR";
		case Logger::WARNING : return "WARNING";
		case Logger::NOTICE  : return "NOTICE";
		case Logger::INFO    : return "INFO";
		case Logger::DEBUG   : return "DEBUG";
		default              : return "OTHER";
	}
}