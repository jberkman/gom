# -*- Makefile -*-

lib_LTLIBRARIES += libxpgom.la

libxpgom_la_SOURCES :=
libxpgom_la_SOURCES += src/xpgom/gomwrapped.cc
libxpgom_la_SOURCES += src/xpgom/gomwrappedattr.cc
libxpgom_la_SOURCES += src/xpgom/gomwrappeddocument.cc
libxpgom_la_SOURCES += src/xpgom/gomwrappeddomimplementation.cc
libxpgom_la_SOURCES += src/xpgom/gomwrappedelement.cc
libxpgom_la_SOURCES += src/xpgom/gomwrappednamednodemap.cc
libxpgom_la_SOURCES += src/xpgom/gomwrappednode.cc
libxpgom_la_SOURCES += src/xpgom/xgDocument.cc
libxpgom_la_SOURCES += src/xpgom/xgDOMImplementation.cc
libxpgom_la_SOURCES += src/xpgom/xgElement.cc
libxpgom_la_SOURCES += src/xpgom/xgGomModule.cc
libxpgom_la_SOURCES += src/xpgom/xgNode.cc
libxpgom_la_SOURCES += src/xpgom/xgWrapped.cc

src/xpgom/xgWrapped.cc: idl/xgPIWrapped.hh

libxpgom_la_CPPFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include -I$(top_builddir)/idl $(XPGOM_CFLAGS)
libxpgom_la_CXXFLAGS = -fshort-wchar
libxpgom_la_LIBDADD := 
libxpgom_la_LDFLAGS := -avoid-version -export-dynamic libgom.a $(XPGOM_LIBS)

libxpgom_la_DEPENDENCIES := libgom.a
