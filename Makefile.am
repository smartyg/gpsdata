## Place generated object files (.o) into the same directory as their source
## files, in order to avoid collisions when non-recursive make is used.
AUTOMAKE_OPTIONS = subdir-objects

## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

AM_CFLAGS =
AM_CXXFLAGS =

## Only use the compiler flags when we build a debug version.
if DEBUG
COMMON_DEBUG_OPTIONS = -Wall -Wextra -W -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align \
	-Wmissing-declarations -Wno-missing-braces -Wno-missing-field-initializers \
	-Wformat=1 -Wswitch-default -Wswitch-enum -Wstrict-overflow=5 -Winline \
	-Wundef -Wunreachable-code -Wlogical-op -Wfloat-equal -Wno-float-equal \
	-Wstrict-aliasing=2 -Wredundant-decls -Werror \
	-fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing \
	-fvar-tracking -fvar-tracking-assignments  -fconcepts -ggdb3 -O0
AM_CFLAGS += $(COMMON_DEBUG_OPTIONS) \
	-Wbad-function-cast -Wstrict-prototypes -Wmissing-prototypes -Wnested-externs \
	-Wold-style-definition
AM_CXXFLAGS += $(COMMON_DEBUG_OPTIONS) \
	-Wabi -Wconversion-null  -Wctor-dtor-privacy -Wdelete-non-virtual-dtor \
	-Wnarrowing -Wnoexcept -Wnon-virtual-dtor  -Wreorder -Wstrict-null-sentinel \
	-Wno-non-template-friend  -Wold-style-cast -Woverloaded-virtual  -Wno-pmf-conversions \
	-Wsign-promo
else
AM_CFLAGS += -flto
AM_CXXFLAGS += -flto -fconcepts
endif

export AM_CFLAGS
export AM_CXXFLAGS

## Install the generated pkg-config file (.pc) into the expected location for
## architecture-dependent package configuration information.  Occasionally,
## pkg-config files are also used for architecture-independent data packages,
## in which case the correct install location would be $(datadir)/pkgconfig.
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gpsdata.pc libgpsdata-utils.pc

lib_LTLIBRARIES = libgpsdata-utils.la
libgpsdata_utils_la_CPPFLAGS = -Iinclude -DLOCALEDIR=\"$(localedir)\" -DPKGLIBDIR=\"$(pkglibdir)\"

## Define the source file list for the "libexample-@EXAMPLE_API_VERSION@.la"
## target.  Note that @EXAMPLE_API_VERSION@ is not interpreted by Automake and
## will therefore be treated as if it were literally part of the target name,
## and the variable name derived from that.
## Note that it is not necessary to list header files
## which are already listed elsewhere in a _HEADERS variable assignment.
libgpsdata_utils_la_SOURCES = src/utils/Logger.cpp \
	src/utils/PointDate.cpp \
	src/utils/ZoneDate.cpp

libgpsdata_utils_la_LIBADD = @ZONEDETECT_LIBS@ @DATE_TZ_LIBS@

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libgpsdata_utils_la_LDFLAGS = -version-info @LIBRARY_CURRENT_VERSION@:@LIBRARY_REVISION_VERSION@:@LIBRARY_AGE_VERSION@

## Define the list of public header files and their install location.  The
## nobase_ prefix instructs Automake to not strip the directory part from each
## filename, in order to avoid the need to define separate file lists for each
## installation directory.  This only works if the directory hierarchy in the
## source tree matches the hierarchy at the install location, however.
gpsdata_includedir = $(includedir)
gpsdata_include_HEADERS = include/gpsdata.hpp \
	include/gpsdata-utils.hpp

gpsdata_sub_includedir = $(includedir)/gpsdata
gpsdata_sub_include_HEADERS = include/gpsdata/GpsDataValueType.hpp \
	include/gpsdata/GpsFactoryUserBase.hpp \
	include/gpsdata/GpsPoint.hpp \
	include/gpsdata/GpsSegment.hpp \
	include/gpsdata/GpsRoute.hpp \
	include/gpsdata/GpsStatistics.hpp \
	include/gpsdata/GpsValue.hpp

gpsdata_sub_comp_includedir = $(includedir)/gpsdata/comp
gpsdata_sub_comp_include_HEADERS = include/gpsdata/comp/GpsPoint.hpp \
	include/gpsdata/comp/GpsRoute.hpp \
	include/gpsdata/comp/GpsSegment.hpp \
	include/gpsdata/comp/GpsStatistics.hpp \
	include/gpsdata/comp/GpsValue.hpp

gpsdata_sub_traits_includedir = $(includedir)/gpsdata/traits
gpsdata_sub_traits_include_HEADERS = include/gpsdata/traits/common.hpp \
	include/gpsdata/traits/GpsFactory.hpp \
	include/gpsdata/traits/GpsPoint.hpp \
	include/gpsdata/traits/GpsRoute.hpp \
	include/gpsdata/traits/GpsSegment.hpp \
	include/gpsdata/traits/GpsStatistics.hpp \
	include/gpsdata/traits/is_instance.hpp

gpsdata_sub_types_includedir = $(includedir)/gpsdata/types
gpsdata_sub_types_include_HEADERS = include/gpsdata/types/Latitude.hpp \
	include/gpsdata/types/Longitude.hpp \
	include/gpsdata/types/ObjectId.hpp \
	include/gpsdata/types/ObjectTime.hpp

gpsdata_sub_utils_includedir = $(includedir)/gpsdata/utils
gpsdata_sub_utils_include_HEADERS = include/gpsdata/utils/Convert.hpp \
	include/gpsdata/utils/GpsDataFactoryBasic.hpp \
	include/gpsdata/utils/Logger.hpp \
	include/gpsdata/utils/PointDate.hpp \
	include/gpsdata/utils/ZoneDate.hpp

if EXAMPLE
bin_PROGRAMS = example1
example1_LDADD = libgpsdata-utils.la
example1_LDFLAGS = -Wl,-rpath=$(pkglibdir),--enable-new-dtags
example1_CPPFLAGS = -Iinclude -DLOCALEDIR=\"$(localedir)\" -DPKGLIBDIR=\"$(pkglibdir)\"
example1_SOURCES = examples/example1.cpp
endif

EXTRA_DIST = VERSION ChangeLog README AUTHORS LICENSE @PROGRAM_NAME@.spec

## Only try to update the ChangeLog file when git is availible to report all the changes.
if HAVE_GIT
## Force to recreate the ChangeLog file every time
.PHONY: ChangeLog

## Dynamicly create the changelog file when a distribution tarball is created.
ChangeLog:
	-$(GIT) log --decorate > ChangeLog

VERSION:
	-$(GIT) describe --tags --match=[0-9]* --long --dirty | $(SED) 's/^v//' > VERSION

distclean-local:
	@rm -f ChangeLog VERSION
endif

if GTEST
SUBDIRS = tests
endif
