check_PROGRAMS = test-pointdate
test_pointdate_SOURCES = test-pointdate.cpp
test_pointdate_LDADD = @GTEST_LIBS@ @GMOCK_LIBS@ ../libgpsdata-utils.la
test_pointdate_LDFLAGS = -pthread
test_pointdate_CPPFLAGS = @GTEST_CFLAGS@ @GMOCK_CFLAGS@ -pthread -I../include -I../src

check_PROGRAMS += test-zonedate
test_zonedate_SOURCES = test-zonedate.cpp
test_zonedate_LDADD = @GTEST_LIBS@ @GMOCK_LIBS@ ../libgpsdata-utils.la
test_zonedate_LDFLAGS = -pthread
test_zonedate_CPPFLAGS = @GTEST_CFLAGS@ @GMOCK_CFLAGS@ -pthread -I../include -I../src

TESTS = test-zonedate test-pointdate

EXTRA_DIST = out_v1/country16.bin out_v1/country21.bin out_v1/timezone16.bin out_v1/timezone21.bin