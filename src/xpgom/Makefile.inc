# -*- Makefile -*-

lib_LTLIBRARIES += libxpgom.la

libxpgom_la_SOURCES :=
libxpgom_la_SOURCES += src/xpgom/xgGomElementFactory.cc
libxpgom_la_SOURCES += src/xpgom/xgGtkElement.cc
libxpgom_la_SOURCES += src/xpgom/xgGtkElementFactory.cc
libxpgom_la_SOURCES += src/xpgom/xgGomModule.cc
libxpgom_la_SOURCES += src/xpgom/xgScriptElement.cc

dist_noinst_DATA += src/xpgom/xgDocShellUnimplemented.cc

$(libxpgom_la_SOURCES): $(idl_headers)

libxpgom_la_CPPFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include -I$(top_builddir)/idl $(XPGOM_CFLAGS)
libxpgom_la_LIBDADD := 
libxpgom_la_LDFLAGS := -avoid-version -export-dynamic -no-undefined $(XPGOM_LIBS)

dist_noinst_DATA += src/xpgom/xgParser.js
