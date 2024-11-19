AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -isystem$(ROOTSYS)/include

AM_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OFFLINE_MAIN)/lib64

pkginclude_HEADERS = \
  tutorial.h

lib_LTLIBRARIES = \
  libtutorial.la

libtutorial_la_SOURCES = \
  tutorial.cc

# track_io was used, but it was changed on Oct/2024 (https://github.com/sPHENIX-Collaboration/coresoftware/commit/a7bda46964f52b53377ad8b6bc61cbc8df472a70)
libtutorial_la_LIBADD = \
  -lphool \
  -lSubsysReco \
  -lfun4allraw

BUILT_SOURCES = testexternals.cc

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = testexternals.cc
testexternals_LDADD   = libtutorial.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f $(BUILT_SOURCES)
