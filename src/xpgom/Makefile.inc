# -*- Makefile -*-

app_LIBRARIES += libxpgom.so

libxpgom_la_SOURCES :=
libxpgom_la_SOURCES += src/xpgom/xgGomElementFactory.cc
libxpgom_la_SOURCES += src/xpgom/xgGtkElement.cc
libxpgom_la_SOURCES += src/xpgom/xgGtkElementFactory.cc
libxpgom_la_SOURCES += src/xpgom/xgGomModule.cc
libxpgom_la_SOURCES += src/xpgom/xgScriptElement.cc

$(libxpgom_la_SOURCES): idl/xgPIWrapped.hh

src/xpgom/xgGomModule.cc: gomwidgets.c

dist_noinst_DATA += src/xpgom/xpgom.symbols

libxpgom_la_CPPFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include -I$(top_builddir)/idl $(XPGOM_CFLAGS)
libxpgom_la_LIBDADD := 
libxpgom_la_LDFLAGS := -avoid-version -export-symbols $(top_srcdir)/src/xpgom/xpgom.symbols -module -no-undefined -no-install $(XPGOM_LIBS)
