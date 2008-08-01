# -*- Makefile -*-

lib_LTLIBRARIES += libxpgom.la

libxpgom_la_SOURCES :=
libxpgom_la_SOURCES += src/xpgom/xgDOMImplementation.cc
libxpgom_la_SOURCES += src/xpgom/xgGomModule.cc

libxpgom_la_CPPFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include $(XPGOM_CFLAGS)
libxpgom_la_CXXFLAGS = -fshort-wchar
libxpgom_la_LIBDADD := 
libxpgom_la_LDFLAGS := -avoid-version -export-dynamic libgom.a $(XPGOM_LIBS)