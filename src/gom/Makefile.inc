# -*- Makefile -*-

bin_PROGRAMS += gom

# this makes us link as C++
empty.cc:
	touch $@

gom_SOURCES := src/gom/gom.c empty.cc
gom_CPPFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include $(GOM_CFLAGS)
gom_CXXFLAGS = -fshort-wchar
gom_LDADD   := libgom.a $(GOM_LIBS)
