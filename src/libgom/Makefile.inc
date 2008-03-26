# -*- Makefile -*-

lib_LTLIBRARIES += libgom.la

libgom_la_SOURCES :=				\
	include/gommacros.h			\
	src/libgom/gomdoc.c			\
	src/libgom/gomdom.c			\
	src/libgom/gomdombuiltins.c		\
	src/libgom/gomdominterfaces.c		\
	src/libgom/gomevt.c			\
	src/libgom/gomglist.c			\
	src/libgom/gomjscontext.c		\
	src/libgom/gomjsdocument.c		\
	src/libgom/gomjselement.c		\
	src/libgom/gomjsexception.c		\
	src/libgom/gomjsnavigator.c		\
	src/libgom/gomjsnode.c			\
	src/libgom/gomjsobject.c		\
	src/libgom/gomjswindow.c		\
	src/libgom/gommouseevt.c		\
	src/libgom/gomobject.c			\
	src/libgom/gomuievt.c			\
	src/libgom/gomvalue.c			\
	src/libgom/gomwidget.c

libgom_la_CFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include $(GOM_CFLAGS) $(JS_CFLAGS)
libgom_la_LDFLAGS = $(GOM_LIBS)

src/libgom/gomdominterfaces.c: include/gom/dom/gomdombuiltins.h

src/libgom/gomwidget.c: gomwidgets.c

src/libgom/gomdombuiltins.c: $(dominclude_HEADERS) Makefile include/gom/dom/gomdombuiltins.h
	( cd $(srcdir) && $(GLIB_MKENUMS) \
			--fhead "#include \"gom/dom/gomdombuiltins.h\"\n" \
			--fprod "\n/* enumerations from \"@filename@\" */" \
			--vhead "GType\n@enum_name@_get_type (void)\n{\n  static GType etype = 0;\n  if (etype == 0) {\n    static const G@Type@Value values[] = {" \
			--vprod "      { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
			--vtail "      { 0, NULL, NULL }\n    };\n    etype = g_@type@_register_static (g_intern_static_string (\"@EnumName@\"), values);\n  }\n  return etype;\n}\n" \
		$(dominclude_HEADERS) ) > xgen-gdbc \
	&& cp xgen-gdbc $@  \
	&& rm -f xgen-gdbc

CLEANFILES += gomwidgets.c gomwidgets.c.tmp xgen-gdbc src/libgom/gomdombuiltins.c
